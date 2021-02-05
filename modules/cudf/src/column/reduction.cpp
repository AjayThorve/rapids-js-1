// Copyright (c) 2020, NVIDIA CORPORATION.
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

#include <node_cudf/column.hpp>
#include <node_cudf/utilities/dtypes.hpp>
#include <node_cudf/utilities/napi_to_cpp.hpp>

#include <cudf/reduction.hpp>
#include <cudf/table/table_view.hpp>

#include <memory>

namespace nv {

std::pair<ObjectUnwrap<Scalar>, ObjectUnwrap<Scalar>> Column::minmax() const {
  auto result = cudf::minmax(*this);
  return {Scalar::New(std::move(result.first)),  //
          Scalar::New(std::move(result.second))};
}

Napi::Value Column::min(Napi::CallbackInfo const& info) { return minmax().first; }

Napi::Value Column::max(Napi::CallbackInfo const& info) { return minmax().second; }

ObjectUnwrap<Scalar> Column::reduce(std::unique_ptr<cudf::aggregation> const& agg,
                                    cudf::data_type const& output_dtype,
                                    rmm::mr::device_memory_resource* mr) const {
  return Scalar::New(cudf::reduce(*this, agg, output_dtype, mr));
}

ObjectUnwrap<Scalar> Column::sum(rmm::mr::device_memory_resource* mr) const {
  return reduce(cudf::make_sum_aggregation(),
                cudf::data_type{[](cudf::type_id id) {
                  if (id == cudf::type_id::INT64) return id;
                  if (id == cudf::type_id::UINT64) return id;
                  return cudf::type_id::FLOAT64;
                }(this->type().id())},
                mr);
}

Napi::Value Column::sum(Napi::CallbackInfo const& info) {
  return sum(NapiToCPP(info[0]).operator rmm::mr::device_memory_resource*());
}

}  // namespace nv
