/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "eval/evaluator.h"
#include "chess/position.h"
#include "eval/evaluation.h"
#include "utils.h"
#include <stdint.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

struct Kernels
{
	cl_kernel network_train;
	cl_kernel network_run;
};

struct Evaluator
{
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_program program;
	cl_command_queue queue;
	cl_int i, err;
	cl_mem network;
	char *error;
	struct Kernels kernels;
};

int
evaluator_create_kernels(struct Evaluator *evaluator)
{
	int error;
	evaluator->kernels.network_train =
	  clCreateKernel(evaluator->program, "network_train", &error);
	evaluator->kernels.network_run =
	  clCreateKernel(evaluator->program, "network_run", &error);
	return error;
}

struct Evaluator *
evaluator_new(void)
{
	struct Evaluator *evaluator = malloc_or_exit(sizeof(struct Evaluator));
	clGetPlatformIDs(1, &evaluator->platform, NULL);
	switch (clGetDeviceIDs(
	  evaluator->platform, CL_DEVICE_TYPE_ALL, 1, &evaluator->device, NULL)) {
		case CL_SUCCESS:
			break;
		case CL_INVALID_PLATFORM:
			evaluator->error = "Invalid platform";
			return evaluator;
		case CL_DEVICE_NOT_FOUND:
			evaluator->error = "Device not found.";
			return evaluator;
		default:
			evaluator->error = "?";
			return evaluator;
	}
	int cl_error = CL_SUCCESS;
	evaluator->program =
	  clCreateProgramWithSource(evaluator->context, 1, "FIXME", &cl_error, &cl_error);
	if (cl_error != CL_SUCCESS) {
		evaluator->error = "Program init. error";
		return evaluator;
	}
	clBuildProgram(evaluator->program, 0, NULL, NULL, NULL, NULL);
	evaluator->queue =
	  clCreateCommandQueue(evaluator->context, evaluator->device, 0, &cl_error);
	evaluator_create_kernels(evaluator);
	return evaluator;
}

void
evaluator_delete(struct Evaluator *evaluator)
{
	if (!evaluator) {
		return;
	}
	clReleaseKernel(evaluator->kernels.network_train);
	clReleaseKernel(evaluator->kernels.network_run);
	clReleaseCommandQueue(evaluator->queue);
	clReleaseProgram(evaluator->program);
	clReleaseContext(evaluator->context);
	free(evaluator);
}

int
evaluator_run(struct Evaluator *evaluator,
              struct Position *position,
              struct Evaluation *ptr)
{
	clFinish(evaluator->queue);
	return 0;
}
