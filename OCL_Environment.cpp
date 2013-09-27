#include "OCL_Environment.h"

#include <iostream>
#include "VectorAddProgram.h"

#include "CLERR.h"


void OCL_Environment::Initialize()
{
	InitOpenCL();

	// Add programs
	AddProgram(
		CreateProgram<VectorAddProgram> (m_context, m_deviceId, "vector_add_kernel.cl")
	);

}
void OCL_Environment::Execute()
{
	for( int i = 0; i < programs.size(); i++ )
	{
		programs[i]->Execute(m_commandQueue);
	}
}

void OCL_Environment::Release()
{
	cl_int ret;

	// Release OpenCL Resources
	ret = clFlush( m_commandQueue );
	CLERR(ret);
	ret = clFinish( m_commandQueue );
	CLERR(ret);
	ret = clReleaseCommandQueue( m_commandQueue );
	CLERR(ret);
	ret = clReleaseContext( m_context );
	CLERR(ret);
	
	for( int i = 0; i < programs.size(); i++ )
	{
		ReleaseProgram( &programs[i] );
	}
	
}

OCL_Environment::OCL_Environment()
{
	m_platformId = 0;
	m_deviceId = 0;
}

OCL_Environment::OCL_Environment(const OCL_Environment&)
{
}

OCL_Environment::~OCL_Environment()
{
}

void OCL_Environment::InitOpenCL()
{
	cl_int ret;

	// Get platform info
	ret = clGetPlatformIDs( 1, &m_platformId, &m_numPlatforms );
	CLERR(ret)

	// Get device info
	ret = clGetDeviceIDs( m_platformId, CL_DEVICE_TYPE_GPU, 1, &m_deviceId, &m_numDevices );
	CLERR(ret)

	// Create OpenCL context
	m_context = clCreateContext( 0, 1, &m_deviceId, 0, 0, &ret );
	CLERR(ret)

	// Create command queue
	m_commandQueue = clCreateCommandQueue( m_context, m_deviceId, 0, &ret );
	CLERR(ret)
}

void OCL_Environment::AddProgram( OCL_Program* program )
{
	programs.push_back( program );
}




