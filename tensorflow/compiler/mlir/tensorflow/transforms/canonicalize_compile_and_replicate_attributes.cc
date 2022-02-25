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

// This transformation pass converts existing compilation and replication
// attributes into unified attributes. For example, A _tpu_replicate=X
// should be replaced with _xla_compile_device_type=TPU and
// _replication_info=X attributes by the conversion.

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Debug.h"
#include "mlir/IR/Builders.h"  // from @llvm-project
#include "mlir/IR/Operation.h"  // from @llvm-project
#include "mlir/Pass/Pass.h"  // from @llvm-project
#include "mlir/Support/LLVM.h"  // from @llvm-project
#include "tensorflow/compiler/mlir/tensorflow/ir/tf_ops.h"
#include "tensorflow/compiler/mlir/tensorflow/transforms/passes.h"
#include "tensorflow/compiler/mlir/tensorflow/transforms/passes_detail.h"

#define DEBUG_TYPE "tf-canonicalize-compile-and-replicate-attributes"

namespace mlir {
namespace TFTPU {

namespace {

constexpr llvm::StringRef kTPUReplicateAttr = "_tpu_replicate";
constexpr llvm::StringRef kCompileDeviceTypeAttr = "_xla_compile_device_type";
constexpr llvm::StringRef kReplicationInfoAttr = "_replication_info";

struct CanonicalizeCompileAndReplicateAttributesPass
    : public TF::CanonicalizeCompileAndReplicateAttributesPassBase<
          CanonicalizeCompileAndReplicateAttributesPass> {
  void runOnOperation() override;
};

void CanonicalizeCompileAndReplicateAttributesPass::runOnOperation() {
  auto module_op = getOperation();
  mlir::OpBuilder builder(module_op.getContext());
  module_op->walk([&](mlir::Operation* op) {
    if (op->hasAttr(kTPUReplicateAttr)) {
      op->setAttr(kReplicationInfoAttr, op->getAttr(kTPUReplicateAttr));
      op->removeAttr(kTPUReplicateAttr);
      if (!llvm::isa<mlir::TF::TPUReplicateMetadataOp>(op))
        op->setAttr(kCompileDeviceTypeAttr, builder.getStringAttr("TPU"));
    }
    return mlir::WalkResult::advance();
  });
}

}  // namespace

std::unique_ptr<OperationPass<ModuleOp>>
CreateCanonicalizeCompileAndReplicateAttributesPass() {
  return std::make_unique<CanonicalizeCompileAndReplicateAttributesPass>();
}

}  // namespace TFTPU
}  // namespace mlir
