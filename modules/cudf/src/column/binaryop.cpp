// Copyright (c) 2020-2021, NVIDIA CORPORATION.
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

#include <node_rmm/memory_resource.hpp>

#include <cudf/binaryop.hpp>
#include <cudf/scalar/scalar_factories.hpp>

#include <rmm/mr/device/per_device_resource.hpp>

namespace nv {

namespace {

ObjectUnwrap<Column> auto_binary_operation(
  Column const& lhs,
  Column const& rhs,
  cudf::binary_operator op,
  rmm::mr::device_memory_resource* mr = rmm::mr::get_current_device_resource()) {
  try {
    return lhs.binary_operation(rhs, op, get_common_type(lhs.type(), rhs.type()), mr);
  } catch (cudf::logic_error const& err) { NAPI_THROW(Napi::Error::New(rhs.Env(), err.what())); }
}

ObjectUnwrap<Column> auto_binary_operation(
  Column const& lhs,
  Scalar const& rhs,
  cudf::binary_operator op,
  rmm::mr::device_memory_resource* mr = rmm::mr::get_current_device_resource()) {
  try {
    return lhs.binary_operation(rhs, op, get_common_type(lhs.type(), rhs.type()), mr);
  } catch (cudf::logic_error const& err) { NAPI_THROW(Napi::Error::New(rhs.Env(), err.what())); }
}

}  // namespace

ObjectUnwrap<Column> Column::binary_operation(Column const& rhs,
                                              cudf::binary_operator op,
                                              cudf::type_id output_type,
                                              rmm::mr::device_memory_resource* mr) const {
  return Column::New(cudf::binary_operation(*this, rhs, op, cudf::data_type{output_type}, mr));
}

ObjectUnwrap<Column> Column::binary_operation(Scalar const& rhs,
                                              cudf::binary_operator op,
                                              cudf::type_id output_type,
                                              rmm::mr::device_memory_resource* mr) const {
  return Column::New(cudf::binary_operation(*this, rhs, op, cudf::data_type{output_type}, mr));
}

ObjectUnwrap<Column> Column::operator+(Column const& other) const { return add(other); }
ObjectUnwrap<Column> Column::operator+(Scalar const& other) const { return add(other); }

ObjectUnwrap<Column> Column::add(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::ADD, mr);
}

ObjectUnwrap<Column> Column::add(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::ADD, mr);
}

ObjectUnwrap<Column> Column::operator-(Column const& other) const { return sub(other); }
ObjectUnwrap<Column> Column::operator-(Scalar const& other) const { return sub(other); }

ObjectUnwrap<Column> Column::sub(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::SUB, mr);
}

ObjectUnwrap<Column> Column::sub(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::SUB, mr);
}

ObjectUnwrap<Column> Column::operator*(Column const& other) const { return mul(other); }
ObjectUnwrap<Column> Column::operator*(Scalar const& other) const { return mul(other); }

ObjectUnwrap<Column> Column::mul(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::MUL, mr);
}

ObjectUnwrap<Column> Column::mul(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::MUL, mr);
}

ObjectUnwrap<Column> Column::operator/(Column const& other) const { return div(other); }
ObjectUnwrap<Column> Column::operator/(Scalar const& other) const { return div(other); }

ObjectUnwrap<Column> Column::div(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::DIV, mr);
}

ObjectUnwrap<Column> Column::div(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::DIV, mr);
}

ObjectUnwrap<Column> Column::true_div(Column const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::TRUE_DIV, mr);
}

ObjectUnwrap<Column> Column::true_div(Scalar const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::TRUE_DIV, mr);
}

ObjectUnwrap<Column> Column::floor_div(Column const& other,
                                       rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::FLOOR_DIV, mr);
}

ObjectUnwrap<Column> Column::floor_div(Scalar const& other,
                                       rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::FLOOR_DIV, mr);
}

ObjectUnwrap<Column> Column::operator%(Column const& other) const { return mod(other); }
ObjectUnwrap<Column> Column::operator%(Scalar const& other) const { return mod(other); }

ObjectUnwrap<Column> Column::mod(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::MOD, mr);
}

ObjectUnwrap<Column> Column::mod(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::MOD, mr);
}

ObjectUnwrap<Column> Column::pow(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::POW, mr);
}

ObjectUnwrap<Column> Column::pow(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::POW, mr);
}

ObjectUnwrap<Column> Column::operator==(Column const& other) const { return eq(other); }
ObjectUnwrap<Column> Column::operator==(Scalar const& other) const { return eq(other); }

ObjectUnwrap<Column> Column::eq(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::EQUAL, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::eq(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::EQUAL, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::operator!=(Column const& other) const { return ne(other); }
ObjectUnwrap<Column> Column::operator!=(Scalar const& other) const { return ne(other); }

ObjectUnwrap<Column> Column::ne(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::NOT_EQUAL, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::ne(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::NOT_EQUAL, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::operator<(Column const& other) const { return lt(other); }
ObjectUnwrap<Column> Column::operator<(Scalar const& other) const { return lt(other); }

ObjectUnwrap<Column> Column::lt(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::LESS, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::lt(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::LESS, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::operator<=(Column const& other) const { return le(other); }
ObjectUnwrap<Column> Column::operator<=(Scalar const& other) const { return le(other); }

ObjectUnwrap<Column> Column::le(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::LESS_EQUAL, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::le(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::LESS_EQUAL, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::operator>(Column const& other) const { return gt(other); }
ObjectUnwrap<Column> Column::operator>(Scalar const& other) const { return gt(other); }

ObjectUnwrap<Column> Column::gt(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::GREATER, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::gt(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(other, cudf::binary_operator::GREATER, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::operator>=(Column const& other) const { return ge(other); }
ObjectUnwrap<Column> Column::operator>=(Scalar const& other) const { return ge(other); }

ObjectUnwrap<Column> Column::ge(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(
    other, cudf::binary_operator::GREATER_EQUAL, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::ge(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(
    other, cudf::binary_operator::GREATER_EQUAL, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::operator&(Column const& other) const { return bitwise_and(other); }
ObjectUnwrap<Column> Column::operator&(Scalar const& other) const { return bitwise_and(other); }

ObjectUnwrap<Column> Column::bitwise_and(Column const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::BITWISE_AND, mr);
}

ObjectUnwrap<Column> Column::bitwise_and(Scalar const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::BITWISE_AND, mr);
}

ObjectUnwrap<Column> Column::operator|(Column const& other) const { return bitwise_or(other); }
ObjectUnwrap<Column> Column::operator|(Scalar const& other) const { return bitwise_or(other); }

ObjectUnwrap<Column> Column::bitwise_or(Column const& other,
                                        rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::BITWISE_OR, mr);
}

ObjectUnwrap<Column> Column::bitwise_or(Scalar const& other,
                                        rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::BITWISE_OR, mr);
}

ObjectUnwrap<Column> Column::operator^(Column const& other) const { return bitwise_xor(other); }
ObjectUnwrap<Column> Column::operator^(Scalar const& other) const { return bitwise_xor(other); }

ObjectUnwrap<Column> Column::bitwise_xor(Column const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::BITWISE_XOR, mr);
}

ObjectUnwrap<Column> Column::bitwise_xor(Scalar const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::BITWISE_XOR, mr);
}

ObjectUnwrap<Column> Column::operator&&(Column const& other) const { return logical_and(other); }
ObjectUnwrap<Column> Column::operator&&(Scalar const& other) const { return logical_and(other); }

ObjectUnwrap<Column> Column::logical_and(Column const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::LOGICAL_AND, mr);
}

ObjectUnwrap<Column> Column::logical_and(Scalar const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::LOGICAL_AND, mr);
}

ObjectUnwrap<Column> Column::operator||(Column const& other) const { return logical_or(other); }
ObjectUnwrap<Column> Column::operator||(Scalar const& other) const { return logical_or(other); }

ObjectUnwrap<Column> Column::logical_or(Column const& other,
                                        rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::LOGICAL_OR, mr);
}

ObjectUnwrap<Column> Column::logical_or(Scalar const& other,
                                        rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::LOGICAL_OR, mr);
}

ObjectUnwrap<Column> Column::coalesce(Column const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::COALESCE, mr);
}

ObjectUnwrap<Column> Column::coalesce(Scalar const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::COALESCE, mr);
}

ObjectUnwrap<Column> Column::operator<<(Column const& other) const { return shift_left(other); }
ObjectUnwrap<Column> Column::operator<<(Scalar const& other) const { return shift_left(other); }

ObjectUnwrap<Column> Column::shift_left(Column const& other,
                                        rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::SHIFT_LEFT, mr);
}

ObjectUnwrap<Column> Column::shift_left(Scalar const& other,
                                        rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::SHIFT_LEFT, mr);
}

ObjectUnwrap<Column> Column::operator>>(Column const& other) const { return shift_right(other); }
ObjectUnwrap<Column> Column::operator>>(Scalar const& other) const { return shift_right(other); }

ObjectUnwrap<Column> Column::shift_right(Column const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::SHIFT_RIGHT, mr);
}

ObjectUnwrap<Column> Column::shift_right(Scalar const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::SHIFT_RIGHT, mr);
}

ObjectUnwrap<Column> Column::shift_right_unsigned(Column const& other,
                                                  rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::SHIFT_RIGHT_UNSIGNED, mr);
}

ObjectUnwrap<Column> Column::shift_right_unsigned(Scalar const& other,
                                                  rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::SHIFT_RIGHT_UNSIGNED, mr);
}

ObjectUnwrap<Column> Column::log_base(Column const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::LOG_BASE, mr);
}

ObjectUnwrap<Column> Column::log_base(Scalar const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::LOG_BASE, mr);
}

ObjectUnwrap<Column> Column::atan2(Column const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::ATAN2, mr);
}

ObjectUnwrap<Column> Column::atan2(Scalar const& other, rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::ATAN2, mr);
}

ObjectUnwrap<Column> Column::null_equals(Column const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(
    other, cudf::binary_operator::NULL_EQUALS, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::null_equals(Scalar const& other,
                                         rmm::mr::device_memory_resource* mr) const {
  return this->binary_operation(
    other, cudf::binary_operator::NULL_EQUALS, cudf::type_id::BOOL8, mr);
}

ObjectUnwrap<Column> Column::null_max(Column const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::NULL_MAX, mr);
}

ObjectUnwrap<Column> Column::null_max(Scalar const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::NULL_MAX, mr);
}

ObjectUnwrap<Column> Column::null_min(Column const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::NULL_MIN, mr);
}

ObjectUnwrap<Column> Column::null_min(Scalar const& other,
                                      rmm::mr::device_memory_resource* mr) const {
  return auto_binary_operation(*this, other, cudf::binary_operator::NULL_MIN, mr);
}

// Private (JS-facing) impls

Napi::Value Column::add(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return add(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return add(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return add(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return add(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.add expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::sub(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return sub(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return sub(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return sub(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return sub(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.sub expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::mul(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return mul(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return mul(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return mul(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return mul(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.mul expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::div(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return div(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return div(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return div(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return div(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.div expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::true_div(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return true_div(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return true_div(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return true_div(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return true_div(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.true_div expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::floor_div(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return floor_div(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return floor_div(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return floor_div(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return floor_div(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.floor_div expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::mod(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return mod(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return mod(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return mod(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return mod(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.mod expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::pow(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return pow(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return pow(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return pow(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return pow(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.pow expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::eq(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return eq(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return eq(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return eq(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return eq(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.eq expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::ne(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return ne(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return ne(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return ne(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return ne(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.ne expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::lt(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return lt(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return lt(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return lt(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return lt(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.lt expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::le(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return le(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return le(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return le(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return le(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.le expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::gt(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return gt(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return gt(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return gt(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return gt(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.gt expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::ge(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return ge(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return ge(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return ge(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return ge(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.ge expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::bitwise_and(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return bitwise_and(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return bitwise_and(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return bitwise_and(*Scalar::New(rhs, type()), mr); }
  if (rhs.IsNumber()) { return bitwise_and(*Scalar::New(rhs, type()), mr); }
  NAPI_THROW(Napi::Error::New(info.Env(),
                              "Column.bitwise_and expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::bitwise_or(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return bitwise_or(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return bitwise_or(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return bitwise_or(*Scalar::New(rhs, type()), mr); }
  if (rhs.IsNumber()) { return bitwise_or(*Scalar::New(rhs, type()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.bitwise_or expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::bitwise_xor(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return bitwise_xor(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return bitwise_xor(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return bitwise_xor(*Scalar::New(rhs, type()), mr); }
  if (rhs.IsNumber()) { return bitwise_xor(*Scalar::New(rhs, type()), mr); }
  NAPI_THROW(Napi::Error::New(info.Env(),
                              "Column.bitwise_xor expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::logical_and(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return logical_and(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return logical_and(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return logical_and(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return logical_and(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(Napi::Error::New(info.Env(),
                              "Column.logical_and expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::logical_or(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return logical_or(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return logical_or(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return logical_or(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return logical_or(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.logical_or expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::coalesce(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return coalesce(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return coalesce(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return coalesce(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return coalesce(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.coalesce expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::shift_left(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return shift_left(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return shift_left(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return shift_left(*Scalar::New(rhs, type()), mr); }
  if (rhs.IsNumber()) { return shift_left(*Scalar::New(rhs, type()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.shift_left expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::shift_right(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return shift_right(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return shift_right(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return shift_right(*Scalar::New(rhs, type()), mr); }
  if (rhs.IsNumber()) { return shift_right(*Scalar::New(rhs, type()), mr); }
  NAPI_THROW(Napi::Error::New(info.Env(),
                              "Column.shift_right expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::shift_right_unsigned(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) {
    return shift_right_unsigned(*Column::Unwrap(rhs.ToObject()), mr);
  }
  if (Scalar::is_instance(rhs)) {
    return shift_right_unsigned(*Scalar::Unwrap(rhs.ToObject()), mr);
  }
  if (rhs.IsBigInt()) { return shift_right_unsigned(*Scalar::New(rhs, type()), mr); }
  if (rhs.IsNumber()) { return shift_right_unsigned(*Scalar::New(rhs, type()), mr); }
  NAPI_THROW(Napi::Error::New(
    info.Env(), "Column.shift_right_unsigned expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::log_base(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return log_base(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return log_base(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return log_base(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return log_base(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.log_base expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::atan2(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return atan2(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return atan2(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return atan2(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return atan2(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.atan2 expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::null_equals(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return null_equals(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return null_equals(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return null_equals(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return null_equals(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(Napi::Error::New(info.Env(),
                              "Column.null_equals expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::null_max(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return null_max(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return null_max(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return null_max(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return null_max(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.null_max expects a Column, Scalar, bigint, or number."));
}

Napi::Value Column::null_min(Napi::CallbackInfo const& info) {
  auto rhs = info[0];
  rmm::mr::device_memory_resource* mr{NapiToCPP(info[1])};
  if (Column::is_instance(rhs)) { return null_min(*Column::Unwrap(rhs.ToObject()), mr); }
  if (Scalar::is_instance(rhs)) { return null_min(*Scalar::Unwrap(rhs.ToObject()), mr); }
  if (rhs.IsBigInt()) { return null_min(*Scalar::New(rhs.As<Napi::BigInt>()), mr); }
  if (rhs.IsNumber()) { return null_min(*Scalar::New(rhs.As<Napi::Number>()), mr); }
  NAPI_THROW(
    Napi::Error::New(info.Env(), "Column.null_min expects a Column, Scalar, bigint, or number."));
}

}  // namespace nv
