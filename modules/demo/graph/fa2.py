# Copyright (c) 2020-2021, NVIDIA CORPORATION.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


import warnings
warnings.filterwarnings("ignore", category=UserWarning)
warnings.filterwarnings("ignore", category=DeprecationWarning)

import python.test_data as datasets
from python.callback import GraphZmqCallback

import zmq
import cudf
import cugraph
import asyncio
import zmq.asyncio

# graph, nodes, edges = make_complex_dataset(direct=False)
# graph, nodes, edges = make_small_dataset(direct=True)
# graph, nodes, edges = make_large_dataset(direct=True)
# graph, nodes, edges = make_cit_patents_dataset()

graph, nodes, edges = datasets.make_synthetic_dataset()
# nodes[["name", "id", "color", "size"]].to_csv("data/nodes.csv", index=False)
# edges[["name", "src", "dst", "edge", "color", "bundle"]].to_csv("data/edges.csv", index=False)

# graph, nodes, edges = datasets.make_capwin_dataset()
# nodes[["name", "id", "color", "size"]].to_csv("data/capwin-nodes.csv", index=False)
# edges[["name", "src", "dst", "edge", "color", "bundle"]].to_csv("data/capwin-edges.csv", index=False)

print("num_nodes:", graph.number_of_nodes())
print("num_edges:", graph.number_of_edges())

async def main(zmq_ctx):

    def map_positions(pos):
        return cudf.DataFrame(pos, columns=["x", "y"]).astype("float32")

    callback = GraphZmqCallback(
        zmq_ctx=zmq_ctx,
        map_positions=map_positions,
        nodes=nodes[["id", "color", "size"]],
        edges=edges[["edge", "bundle", "color"]],
        edge_col_names=["edge", "color", "bundle"],
        node_col_names=["id", "color", "size", "x", "y"],
    )
    cugraph.force_atlas2(
        graph,
        max_iter=500,
        callback=callback,
    )
    callback.update(msg=b"close")
    callback.close()

# asyncio.run(main(zmq.Context.instance()))
