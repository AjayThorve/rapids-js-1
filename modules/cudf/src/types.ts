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

export interface DataType<T extends TypeId = any> {
  readonly id: T;
  readonly valueType: any;
}
export class DataType<T extends TypeId = any> {
  constructor(public readonly id: T) {}
}

export enum TypeId
{
  EMPTY,
  INT8,
  INT16,
  INT32,
  INT64,
  UINT8,
  UINT16,
  UINT32,
  UINT64,
  FLOAT32,
  FLOAT64,
  BOOL8,
  TIMESTAMP_DAYS,
  TIMESTAMP_SECONDS,
  TIMESTAMP_MILLISECONDS,
  TIMESTAMP_MICROSECONDS,
  TIMESTAMP_NANOSECONDS,
  DURATION_DAYS,
  DURATION_SECONDS,
  DURATION_MILLISECONDS,
  DURATION_MICROSECONDS,
  DURATION_NANOSECONDS,
  DICTIONARY32,
  STRING,
  LIST,
  DECIMAL32,
  DECIMAL64,
}

export interface Int8 extends DataType<TypeId.INT8> {
  valueType: number;
}
export class Int8 extends DataType<TypeId.INT8> {
  constructor() { super(TypeId.INT8); }
}

export interface Int16 extends DataType<TypeId.INT16> {
  valueType: number;
}
export class Int16 extends DataType<TypeId.INT16> {
  constructor() { super(TypeId.INT16); }
}

export interface Int32 extends DataType<TypeId.INT32> {
  valueType: number;
}
export class Int32 extends DataType<TypeId.INT32> {
  constructor() { super(TypeId.INT32); }
}

export interface Int64 extends DataType<TypeId.INT64> {
  valueType: number;
}
export class Int64 extends DataType<TypeId.INT64> {
  constructor() { super(TypeId.INT64); }
}

export interface UInt8 extends DataType<TypeId.UINT8> {
  valueType: number;
}
export class UInt8 extends DataType<TypeId.UINT8> {
  constructor() { super(TypeId.UINT8); }
}

export interface UInt16 extends DataType<TypeId.UINT16> {
  valueType: number;
}
export class UInt16 extends DataType<TypeId.UINT16> {
  constructor() { super(TypeId.UINT16); }
}

export interface UInt32 extends DataType<TypeId.UINT32> {
  valueType: number;
}
export class UInt32 extends DataType<TypeId.UINT32> {
  constructor() { super(TypeId.UINT32); }
}

export interface UInt64 extends DataType<TypeId.UINT64> {
  valueType: number;
}
export class UInt64 extends DataType<TypeId.UINT64> {
  constructor() { super(TypeId.UINT64); }
}

export interface Float32 extends DataType<TypeId.FLOAT32> {
  valueType: number;
}
export class Float32 extends DataType<TypeId.FLOAT32> {
  constructor() { super(TypeId.FLOAT32); }
}

export interface Float64 extends DataType<TypeId.FLOAT64> {
  valueType: number;
}
export class Float64 extends DataType<TypeId.FLOAT64> {
  constructor() { super(TypeId.FLOAT64); }
}

export interface Bool8 extends DataType<TypeId.BOOL8> {
  valueType: boolean;
}
export class Bool8 extends DataType<TypeId.BOOL8> {
  constructor() { super(TypeId.BOOL8); }
}
