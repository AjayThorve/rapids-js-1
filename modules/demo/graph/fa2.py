# Copyright (c) 2020, NVIDIA CORPORATION.
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
warnings.filterwarnings('ignore', category=UserWarning)
warnings.filterwarnings('ignore', category=DeprecationWarning)

from python.shaping import shape_hypergraph
from python.callback import GraphZmqCallback
from python.test_data import (
    make_large_dataframe,
    make_small_dataframe,
    make_complex_dataframe,
)

import zmq
import cudf
import cugraph
import asyncio
import zmq.asyncio

# df, kwargs = make_small_dataframe(direct=True)
# df, kwargs = make_complex_dataframe(direct=True)
df, kwargs = make_large_dataframe(direct=False)

xs = cugraph.hypergraph(df, **kwargs)

del xs['events']
del xs['entities']

print('num_nodes:', xs['graph'].number_of_nodes())
print('num_edges:', xs['graph'].number_of_edges())

graph, nodes, edges = shape_hypergraph(
    **xs, **kwargs, symmetrize=False
)

async def main(zmq_ctx):

    def map_positions(pos):
        return cudf.DataFrame.from_gpu_matrix(pos, columns=['x', 'y']).astype('float32')

    callback = GraphZmqCallback(
        zmq_ctx=zmq_ctx,
        map_positions=map_positions,
        nodes=nodes[['id', 'color', 'size']],
        edges=edges[['edge', 'bundle', 'color']],
        edge_col_names=['edge', 'color', 'bundle'],
        node_col_names=['id', 'color', 'size', 'x', 'y'],
    )
    cugraph.force_atlas2(
        graph,
        max_iter=200,
        callback=callback,
    )
    callback.update(msg=b'close')
    callback.close()

asyncio.run(main(zmq.Context.instance()))
