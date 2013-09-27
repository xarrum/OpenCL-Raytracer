#include "OCL_Environment.h"

#include <iostream>
#include "VectorAddProgram.h"

#include "CLERR.h"


void OCL_Environment::Initialize()
{
	InitOpenCL();

	// Add programs
	AddProgram(
		CreateProgram<VectorAddProgram> (m_context, m_deviceIds[0], "vector_add_kernel.cl")
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
	ret = clGetPlatformIDs( 0, nullptr, &m_numPlatforms );
	CLERR(ret);
	m_platformIds = std::vector<cl_platform_id>(m_numPlatforms);
	ret = clGetPlatformIDs( m_numPlatforms, m_platformIds.data(), nullptr );
	CLERR(ret);

	
	for(int i = 0; i < m_platformIds.size(); i++)
	{
		// Print platform info
		std::cout << GetPlatformInfo( m_platformIds[i], CL_PLATFORM_VENDOR ) << "\n";
		std::cout << GetPlatformInfo( m_platformIds[i], CL_PLATFORM_NAME ) << "\n";
		std::cout << GetPlatformInfo( m_platformIds[i], CL_PLATFORM_VERSION ) << "\n";

		// Get devices
		ret = clGetDeviceIDs( m_platformIds[i], CL_DEVICE_TYPE_ALL, 0, nullptr, &m_numDevices );
		CLERR(ret);

		m_deviceIds = std::vector<cl_device_id>(m_numDevices);
		ret = clGetDeviceIDs( m_platformIds[i], CL_DEVICE_TYPE_ALL, m_numDevices, m_deviceIds.data(), nullptr );
		CLERR(ret);

		// Print device info
		for( int e = 0; e < m_deviceIds.size(); e++ )
		{
			std::cout 
				<< "\t" << GetDeviceInfo( m_deviceIds[e], CL_DEVICE_VENDOR ) << "\n"
				<< "\t" << GetDeviceInfo( m_deviceIds[e], CL_DEVICE_NAME ) << "\n"
				<< "\tDriver version " << GetDeviceInfo( m_deviceIds[e], CL_DRIVER_VERSION ) << "\n"
				;
		}

	}

	// Create OpenCL context
	const cl_context_properties contextProperties [] =
	{
		CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties> (m_platformIds[0]),
		CL_GL_CONTEXT_KHR, reinterpret_cast<cl_context_properties> ( wglGetCurrentContext() ),
		CL_WGL_HDC_KHR, reinterpret_cast<cl_context_properties> ( wglGetCurrentDC() ),
		0, 0
	};

	m_context = clCreateContext( 
		contextProperties, m_numDevices, 
		m_deviceIds.data(), nullptr, nullptr, &ret );
	CLERR(ret);

	// Create command queue
	m_commandQueue = clCreateCommandQueue( m_context, m_deviceIds[0], 0, &ret );
	CLERR(ret);
}

void OCL_Environment::AddProgram( OCL_Program* program )
{
	programs.push_back( program );
}

std::string OCL_Environment::GetPlatformInfo( cl_platform_id platform_id, cl_platform_info info )
{
	cl_int ret;
	char buffer[1024];
	size_t actualSize;
	ret = clGetPlatformInfo( platform_id, info, 1024, buffer, &actualSize); 
	CLERR(ret);

	return std::string(buffer);

}

std::string OCL_Environment::GetDeviceInfo( cl_device_id device_id, cl_device_info info )
{
	cl_int ret;
	char buffer[1024];
	size_t actualSize;
	ret = clGetDeviceInfo( device_id, info, 1024, buffer, &actualSize); 
	CLERR(ret);

	return std::string(buffer);
}


