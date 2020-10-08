// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "orttraining/training_ops/cpu/loss/reduction_type.h"
#include "core/providers/hip/reduction/reduction_ops.h"

namespace onnxruntime {
namespace rocm {

template <typename T>
void SoftMaxCrossEntropyImpl(
    const T* log_prob,
    const T* label,
    size_t normalize_factor,
    T* output_data,
    size_t count);

template <typename T>
void SoftMaxCrossEntropyGradImpl(
    const T* dY,
    const T* log_prob,
    const T* label,
    size_t normalize_factor,
    T* output_data,
    size_t count);

template <typename T, typename Tin>
void SparseSoftmaxCrossEntropyImpl(
    const T* log_prob,
    const Tin* label,
    const T* weight,
    const T* normalize_factor,
    T* output_data,
    size_t count,
    size_t label_depth);

template <typename T, typename Tin>
void SparseSoftmaxCrossEntropyGradImpl(
    const T* dY,
    const T* log_prob,
    const Tin* label,
    const T* weight,
    const T* normalize_factor,
    T* output_data,
    size_t count,
    size_t label_depth);

class LossBase : public ReduceKernel<true> {
 public:
  explicit LossBase(const OpKernelInfo& info)
      : ReduceKernel<true>(info, /*keep_dims_override*/ int64_t(0)) {
    std::string reduction;
    ORT_ENFORCE(info.GetAttr<std::string>("reduction", &reduction).IsOK());
    reduction_ = StringToReductionType(reduction);
  }

 protected:
  ReductionType reduction_;
};

template <typename T>
class SoftmaxCrossEntropy final : public LossBase {
 public:
  SoftmaxCrossEntropy(const OpKernelInfo& info) : LossBase(info) {
    // TODO: implement reduction type of NONE
    ORT_ENFORCE(reduction_ != ReductionType::NONE, "Loss with reduction 'none' is not implemented.");
  }

  Status ComputeInternal(OpKernelContext* context) const override;
};

template <typename T>
class SoftmaxCrossEntropyGrad final : public LossBase {
 public:
  SoftmaxCrossEntropyGrad(const OpKernelInfo& info) : LossBase(info) {
    // TODO: implement reduction type of NONE
    ORT_ENFORCE(reduction_ != ReductionType::NONE, "Loss with reduction 'none' is not implemented.");
  }

  Status ComputeInternal(OpKernelContext* context) const override;
};

template <typename T, typename Tin>
class SparseSoftmaxCrossEntropy final : public LossBase {
 public:
  SparseSoftmaxCrossEntropy(const OpKernelInfo& info) : LossBase(info) {
    // TODO: implement reduction type of NONE
    ORT_ENFORCE(reduction_ != ReductionType::NONE, "Loss with reduction 'none' is not implemented.");
  }

  Status ComputeInternal(OpKernelContext* context) const override;
};

template <typename T, typename Tin>
class SparseSoftmaxCrossEntropyGrad final : public LossBase {
 public:
  SparseSoftmaxCrossEntropyGrad(const OpKernelInfo& info) : LossBase(info) {
    // TODO: implement reduction type of NONE
    ORT_ENFORCE(reduction_ != ReductionType::NONE, "Loss with reduction 'none' is not implemented.");
  }

  Status ComputeInternal(OpKernelContext* context) const override;
};


}  // namespace rocm
}  // namespace onnxruntime