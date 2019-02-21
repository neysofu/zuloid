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

extern const char network_cl[];

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
	char *error;
	struct Kernels kernels;
};

int
evaluator_init_context(struct Evaluator *evaluator)
{
	cl_context_properties context_properties[2][2] = { 0 };
	context_properties[0][0] = CL_CONTEXT_PLATFORM;
	context_properties[0][1] = evaluator->platform;
	evaluator->context =
	  clCreateContext(context_properties, 1, &evaluator->device, NULL, NULL, NULL);
	return CL_SUCCESS;
}

int
evaluator_init_kernels(struct Evaluator *evaluator)
{
	int error;
	evaluator->kernels.network_train =
	  clCreateKernel(evaluator->program, "network_train", &error);
	evaluator->kernels.network_run =
	  clCreateKernel(evaluator->program, "network_run", &error);
	return error;
}

int
evaluator_init_program(struct Evaluator *evaluator)
{
	int error;
	char *sources[1] = { network_cl };
	evaluator->program = clCreateProgramWithSource(
	  evaluator->context, 1, sources, NULL, &error);
	clBuildProgram(evaluator->program, 0, NULL, NULL, NULL, NULL);
	return error;
}

struct Evaluator *
evaluator_new(void)
{
	struct Evaluator *evaluator = malloc_or_exit(sizeof(struct Evaluator));
	*evaluator = (struct Evaluator){ 0 };
	clGetPlatformIDs(1, &evaluator->platform, NULL);
	switch (clGetDeviceIDs(
	  evaluator->platform, CL_DEVICE_TYPE_ALL, 1, &evaluator->device, NULL)) {
		case CL_SUCCESS:
			break;
		case CL_DEVICE_NOT_FOUND:
			evaluator->error = "Device not found.";
			return evaluator;
		default:
			evaluator->error = "Unexpected error. This is a bug.";
			return evaluator;
	}
	int cl_error = CL_SUCCESS;
	evaluator_init_context(evaluator);
	evaluator_init_program(evaluator);
	evaluator->queue =
	  clCreateCommandQueue(evaluator->context, evaluator->device, 0, &cl_error);
	evaluator_init_kernels(evaluator);
	return evaluator;
}

void
evaluator_delete(struct Evaluator *evaluator)
{
	if (!evaluator) {
		return;
	}
	clReleaseContext(evaluator->context);
	clReleaseProgram(evaluator->program);
	clReleaseCommandQueue(evaluator->queue);
	clReleaseKernel(evaluator->kernels.network_train);
	clReleaseKernel(evaluator->kernels.network_run);
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
