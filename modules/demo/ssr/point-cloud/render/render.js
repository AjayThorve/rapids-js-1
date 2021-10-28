// Copyright (c) 2021, NVIDIA CORPORATION.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

const {IpcMemory, Uint8Buffer} = require('@rapidsai/cuda');
const {RapidsJSDOM}            = require('@rapidsai/jsdom');
const copyFramebuffer          = require('./copy')();

class Renderer {
  constructor() {
    const onAnimationFrameRequested = immediateAnimationFrame(this);
    const jsdom                     = new RapidsJSDOM({module, onAnimationFrameRequested});

    const {deck, render} = jsdom.window.evalFn(makeDeck);

    this.deck    = deck;
    this.jsdom   = jsdom;
    this._render = render;
  }
  async render(props = {}, graph = {}, state = {}, events = [], frame = 0) {
    const window = this.jsdom.window;

    graph = openGraphIpcHandles(graph);
    props && this.deck.setProps(props);

    state?.deck && this.deck.restore(state.deck);
    state?.graph && Object.assign(graph, state.graph);
    state?.window && Object.assign(window, state.window);

    state?.selectedInfo && Object.assign(this.deck.selectedInfo, state.selectedInfo);
    state?.boxSelectCoordinates &&
      Object.assign(this.deck.boxSelectCoordinates, state.boxSelectCoordinates);

    (events || []).forEach((event) => window.dispatchEvent(event));

    await this._render(graph,
                       this.deck.boxSelectCoordinates.rectdata,
                       state.pickingMode === 'boxSelect' ? {controller: {dragPan: false}}
                                                         : {controller: {dragPan: true}});

    closeIpcHandles(graph.data.nodes);
    closeIpcHandles(graph.data.edges);
    return {
      frame: copyFramebuffer(this.deck.animationLoop, frame),
      state: {
        deck: this.deck.serialize(),
        graph: this.deck.layerManager.getLayers()
                 ?.find((layer) => layer.id === 'laz-point-cloud-layer')
                 .graph,
        window: {
          x: window.x,
          y: window.y,
          title: window.title,
          width: window.width,
          height: window.height,
          cursor: window.cursor,
          mouseX: window.mouseX,
          mouseY: window.mouseY,
          buttons: window.buttons,
          scrollX: window.scrollX,
          scrollY: window.scrollY,
          modifiers: window.modifiers,
          mouseInWindow: window.mouseInWindow,
        },
        boxSelectCoordinates: this.deck.boxSelectCoordinates,
        selectedInfo: this.deck.selectedInfo
      }
    };
  }
}

module.exports.Renderer = Renderer;

function immediateAnimationFrame(renderer) {
  let request  = null;
  let flushing = false;
  const flush = () => {
    flushing = true;
    while (request && request.active) {
      const f = request.flush;
      request = null;
      f();
    }
    flushing = false;
  };
  return (r) => {
    if (flushing) { return request = r; }
    if (renderer?.deck?.animationLoop?._initialized) {  //
      return flush(request = r);
    }
    if (!request && (request = r)) { setImmediate(flush); }
  };
}

function makeDeck() {
  const {log: deckLog} = require('@deck.gl/core');
  deckLog.level        = 0;
  deckLog.enable(false);

  const {OrbitView, COORDINATE_SYSTEM} = require('@deck.gl/core');
  const {PointCloudLayer}              = require('@deck.gl/layers');
  const {DeckSSR}                      = require('@rapidsai/deck.gl');

  // Data source: kaarta.com
  const LAZ_SAMPLE =
    'https://raw.githubusercontent.com/uber-common/deck.gl-data/master/examples/point-cloud-laz/indoor.0.1.laz';

  const makeLayers = () => {
    return [
      new PointCloudLayer({
        id: 'laz-point-cloud-layer',
        data: LAZ_SAMPLE,
        coordinateSystem: COORDINATE_SYSTEM.CARTESIAN,
        getNormal: [0, 1, 0],
        getColor: [255, 255, 255],
        opacity: 0.5,
        pointSize: 0.5,
      }),
    ];
  };

  const deck = new DeckSSR({
    createFramebuffer: true,
    initialViewState: {
      zoom: 1,
      target: [0, 0, 0],
      minZoom: Number.NEGATIVE_INFINITY,
      maxZoom: Number.POSITIVE_INFINITY,
    },
    layers: [makeLayers()],
    views: [
      new OrbitView(),
    ],
    onAfterAnimationFrameRender({_loop}) { _loop.pause(); },
  });

  deck.selectedInfo         = {selectedCoordinates: {}, selected: []};
  deck.boxSelectCoordinates = {rectdata: [{polygon: [[]], show: false}], startPos: null};

  return {
    deck,
    render(graph, rectdata, cb_props = {}) {
      const done = deck.animationLoop.waitForRender();
      deck.setProps({layers: makeLayers(deck, graph).concat([]), ...cb_props});
      deck.animationLoop.start();
      return done;
    },
  };
}

function openGraphIpcHandles({nodes, edges, ...graphLayerProps} = {}) {
  const data = {
    nodes: openNodeIpcHandles(nodes),
    edges: openEdgeIpcHandles(edges),
  };
  return {
    pickable: true,
    edgeOpacity: .5,
    edgeStrokeWidth: 2,
    nodesStroked: true,
    nodeFillOpacity: .5,
    nodeStrokeOpacity: .9,
    nodeRadiusScale: 1 / 75,
    nodeRadiusMinPixels: 5,
    nodeRadiusMaxPixels: 150,
    ...graphLayerProps,
    data,
    numNodes: data.nodes.length,
    numEdges: data.edges.length,
  };
}

function openNodeIpcHandles(attrs = {}) {
  const attributes = {
    nodeRadius: openIpcHandle(attrs.nodeRadius),
    nodeXPositions: openIpcHandle(attrs.nodeXPositions),
    nodeYPositions: openIpcHandle(attrs.nodeYPositions),
    nodeFillColors: openIpcHandle(attrs.nodeFillColors),
    nodeElementIndices: openIpcHandle(attrs.nodeElementIndices),
  };
  return {offset: 0, length: attrs.length ?? (attributes.nodeRadius?.byteLength || 0), attributes};
}

function openEdgeIpcHandles(attrs = {}) {
  const attributes = {
    edgeList: openIpcHandle(attrs.edgeList),
    edgeColors: openIpcHandle(attrs.edgeColors),
    edgeBundles: openIpcHandle(attrs.edgeBundles),
  };
  return {
    offset: 0,
    length: attrs.length ?? (attributes.edgeList?.byteLength || 0) / 8,
    attributes
  };
}

function openIpcHandle(obj) {
  if (typeof obj === 'string') { obj = JSON.parse(obj); }
  if (obj) {
    const {byteOffset = 0} = obj;
    const handle           = Uint8Array.from(obj.handle.map(Number));
    return new Uint8Buffer(new IpcMemory(handle)).subarray(byteOffset);
  }
  return null;
}

function closeIpcHandles(obj) {
  for (const key in obj) {
    const {buffer} = obj[key] || {};
    if (buffer && (buffer instanceof IpcMemory)) {  //
      buffer.close();
    }
  }
}
