/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <random>

#include <gtest/gtest.h>
#include "tensorflow/lite/delegates/xnnpack/concatenation_tester.h"
#include "tensorflow/lite/delegates/xnnpack/xnnpack_delegate.h"

namespace tflite {
namespace xnnpack {

TEST(Concatenation, 1D) {
  std::unique_ptr<TfLiteDelegate, decltype(&TfLiteXNNPackDelegateDelete)>
      xnnpack_delegate(TfLiteXNNPackDelegateCreate(nullptr),
                       TfLiteXNNPackDelegateDelete);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto shape_rng =
      std::bind(std::uniform_int_distribution<int32_t>(2, 10), std::ref(rng));
  const std::vector<int32_t> shape1({shape_rng()});
  const std::vector<int32_t> shape2({shape_rng()});

  for (int i = -1; i < 1; i++) {
    ConcatenationTester().Input1Shape(shape1).Input2Shape(shape2).Axis(i).Test(
        TensorType_INT8, xnnpack_delegate.get());
  }
}

TEST(Concatenation, 2D) {
  std::unique_ptr<TfLiteDelegate, decltype(&TfLiteXNNPackDelegateDelete)>
      xnnpack_delegate(TfLiteXNNPackDelegateCreate(nullptr),
                       TfLiteXNNPackDelegateDelete);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto shape_rng =
      std::bind(std::uniform_int_distribution<int32_t>(2, 10), std::ref(rng));

  for (int i = -1; i < 2; i++) {
    // All dimensions must be the same, except for axis.
    const std::vector<int32_t> shape1({shape_rng(), shape_rng()});
    std::vector<int32_t> shape2{shape1};
    shape2[i < 0 ? i + shape1.size() : i] = shape_rng();

    ConcatenationTester().Input1Shape(shape1).Input2Shape(shape2).Axis(i).Test(
        TensorType_INT8, xnnpack_delegate.get());
  }
}

TEST(Concatenation, 3D) {
  std::unique_ptr<TfLiteDelegate, decltype(&TfLiteXNNPackDelegateDelete)>
      xnnpack_delegate(TfLiteXNNPackDelegateCreate(nullptr),
                       TfLiteXNNPackDelegateDelete);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto shape_rng =
      std::bind(std::uniform_int_distribution<int32_t>(2, 10), std::ref(rng));

  for (int i = -1; i < 3; i++) {
    // All dimensions must be the same, except for axis.
    const std::vector<int32_t> shape1({shape_rng(), shape_rng(), shape_rng()});
    std::vector<int32_t> shape2{shape1};
    shape2[i < 0 ? i + shape1.size() : i] = shape_rng();

    ConcatenationTester().Input1Shape(shape1).Input2Shape(shape2).Axis(i).Test(
        TensorType_INT8, xnnpack_delegate.get());
  }
}

TEST(Concatenation, 4D) {
  std::unique_ptr<TfLiteDelegate, decltype(&TfLiteXNNPackDelegateDelete)>
      xnnpack_delegate(TfLiteXNNPackDelegateCreate(nullptr),
                       TfLiteXNNPackDelegateDelete);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto shape_rng =
      std::bind(std::uniform_int_distribution<int32_t>(2, 10), std::ref(rng));

  for (int i = -1; i < 4; i++) {
    // All dimensions must be the same, except for axis.
    const std::vector<int32_t> shape1(
        {shape_rng(), shape_rng(), shape_rng(), shape_rng()});
    std::vector<int32_t> shape2{shape1};
    shape2[i < 0 ? i + shape1.size() : i] = shape_rng();

    ConcatenationTester().Input1Shape(shape1).Input2Shape(shape2).Axis(i).Test(
        TensorType_INT8, xnnpack_delegate.get());
  }
}

}  // namespace xnnpack
}  // namespace tflite
