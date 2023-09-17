#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __GNUC__
#define AOT_INDUCTOR_EXPORT __attribute__((__visibility__("default")))
#else // !__GNUC__
#ifdef _WIN32
#define AOT_INDUCTOR_EXPORT __declspec(dllexport)
#else // !_WIN32
#define AOT_INDUCTOR_EXPORT
#endif // _WIN32
#endif // __GNUC__

using AOTInductorError = int32_t;
#define AOTI_RUNTIME_SUCCESS 0
#define AOTI_RUNTIME_FAILURE 1

#define AOT_INDUCTOR_ERROR_CHECK(call)                                     \
  if ((call) != AOTI_RUNTIME_SUCCESS) {                                    \
    throw std::runtime_error(                                              \
        std::string(#call " API call failed at ") + __FILE__ + ", line " + \
        std::to_string(__LINE__));                                         \
  }

extern "C" {
struct AOTInductorModelContainerOpaque;
using AOTInductorModelContainerHandle = AOTInductorModelContainerOpaque*;

struct AOTInductorStreamOpaque;
using AOTInductorStreamHandle = AOTInductorStreamOpaque*;

struct AOTInductorTensorOpaque;
using AOTInductorTensorHandle = AOTInductorTensorOpaque*;

struct AOTInductorProxyExecutorOpaque;
using AOTInductorProxyExecutorHandle = AOTInductorProxyExecutorOpaque*;

// Creates an AOTInductor model container. The parameter num_models
// specifies the number of model instances that may be run concurrently for
// the same input model.
AOTInductorError AOTInductorModelContainerCreate(
    AOTInductorModelContainerHandle* container_handle,
    size_t num_models,
    bool is_cpu = false,
    const char* cubin_dir = nullptr);

// Deletes the AOTInductor model container.
AOTInductorError AOTInductorModelContainerDelete(
    AOTInductorModelContainerHandle container_handle);

// Runs the inference.
AOTInductorError AOTInductorModelContainerRun(
    AOTInductorModelContainerHandle container_handle,
    AOTInductorTensorHandle input_handles,
    size_t num_inputs,
    AOTInductorTensorHandle output_handles,
    size_t num_outputs,
    AOTInductorStreamHandle stream_handle,
    AOTInductorProxyExecutorHandle proxy_executor_handle,
    const int64_t** ret_output_sizes,
    int64_t* ret_output_ndims);

// Retrieves the number of inputs for the model.
AOTInductorError AOTInductorModelContainerGetNumInputs(
    AOTInductorModelContainerHandle container_handle,
    size_t* ret_num_inputs);

// Retrieves the input name at the given index.
AOTInductorError AOTInductorModelContainerGetInputName(
    AOTInductorModelContainerHandle container_handle,
    size_t input_idx,
    const char** ret_input_names);

// Retrieves the input dtype at the given index.
AOTInductorError AOTInductorModelContainerGetInputDtype(
    AOTInductorModelContainerHandle container_handle,
    size_t input_idx,
    const char** ret_input_dtypes);

// Retrieves the number of outputs for the model.
AOTInductorError AOTInductorModelContainerGetNumOutputs(
    AOTInductorModelContainerHandle container_handle,
    size_t* ret_num_outputs);

// Retrieves the output name at the given index.
AOTInductorError AOTInductorModelContainerGetOutputName(
    AOTInductorModelContainerHandle container_handle,
    size_t output_idx,
    const char** ret_output_names);

// Retrieves the output dtype at the given index.
AOTInductorError AOTInductorModelContainerGetOutputDtype(
    AOTInductorModelContainerHandle container_handle,
    size_t output_idx,
    const char** ret_output_dtypes);

// Retieves the input shape with the maximum dimension size for each dimension.
AOTInductorError AOTInductorModelContainerGetMaxInputShape(
    AOTInductorModelContainerHandle container_handle,
    size_t input_idx,
    const int64_t** ret_input_sizes,
    int64_t* ret_input_ndim);

// Retieves the output shape with the maximum dimension size for each dimension.
AOTInductorError AOTInductorModelContainerGetMaxOutputShape(
    AOTInductorModelContainerHandle container_handle,
    size_t output_idx,
    const int64_t** ret_output_sizes,
    int64_t* ret_output_ndim);

} // extern "C"
