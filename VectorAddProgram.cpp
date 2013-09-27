#include "VectorAddProgram.h"
#include "CLERR.h"

VectorAddProgram::VectorAddProgram( cl_context &context, cl_device_id &deviceId )
	: OCL_Program( context, deviceId )
{
	LIST_SIZE = 1024;
}

VectorAddProgram::~VectorAddProgram()
{
}

void VectorAddProgram::Execute( cl_command_queue commandQueue )
{
	cl_int ret;
	// Copy vectors to memory buffers
	ret = clEnqueueWriteBuffer( commandQueue, m_memObjA, CL_TRUE, 0, LIST_SIZE * sizeof( int ), A, 0, 0, 0 );
	CLERR(ret);
	ret = clEnqueueWriteBuffer( commandQueue, m_memObjB, CL_TRUE, 0, LIST_SIZE * sizeof( int ), B, 0, 0, 0 );
	CLERR(ret);

	// Execute kernel
	size_t globalItemSize = LIST_SIZE; // Process the entire lists
	size_t localItemSize = 64; // Divide work into chunks of 64
	ret = clEnqueueNDRangeKernel( commandQueue, m_kernels[0], 1, 0, &globalItemSize, &localItemSize, 0, 0, 0 );
	CLERR(ret);

	// Read memory from device to local variable C
	int *C = new int[LIST_SIZE];
	ret = clEnqueueReadBuffer( commandQueue, m_memObjC, CL_TRUE, 0, LIST_SIZE * sizeof( int ), C, 0, 0, 0 );
	CLERR(ret);

	//Display results
	for( int i = 0; i < LIST_SIZE; i+=128 )
		std::cout << A[i] << " + " << B[i] << " = " << C[i] << "\n";
}

void VectorAddProgram::BuildProgram()
{
	cl_int ret;

	A = new int[LIST_SIZE];
	B = new int[LIST_SIZE];
	for( int i = 0; i < LIST_SIZE; i++ )
	{
		A[i] = i;
		B[i] = LIST_SIZE - i;
	}

	// Create memory buffers on the device for each vector
	m_memObjA = clCreateBuffer( m_context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof( int ), 0, &ret );
	CLERR(ret);
	m_memObjB = clCreateBuffer( m_context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof( int ), 0, &ret );
	CLERR(ret); 
	m_memObjC = clCreateBuffer( m_context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof( int ), 0, &ret );
	CLERR(ret);

	// Create program from source code
	m_program = clCreateProgramWithSource( m_context, 1, &m_source, &m_sourceLength, &ret );
	CLERR(ret);

	// Build the OpenCL program
	ret = clBuildProgram( m_program, 1, &m_deviceId, 0, 0, 0 );
	CLERR(ret);

	//Create OpenCL kernels
	AddKernel("AddVector");

	// Set arguments for kernel
	ret = clSetKernelArg( m_kernels[0], 0, sizeof( cl_mem ), (void*)&m_memObjA );
	CLERR(ret);
	ret = clSetKernelArg( m_kernels[0], 1, sizeof( cl_mem ), (void*)&m_memObjB );
	CLERR(ret);
	ret = clSetKernelArg( m_kernels[0], 2, sizeof( cl_mem ), (void*)&m_memObjC );
	CLERR(ret);
}

void VectorAddProgram::Release()
{
	cl_int ret;
	ret = clReleaseMemObject( m_memObjA );
	CLERR(ret)
	ret = clReleaseMemObject( m_memObjB );
	CLERR(ret)
	ret = clReleaseMemObject( m_memObjC );
	CLERR(ret)
	delete[] A;
	delete[] B;
	delete[] C;
}