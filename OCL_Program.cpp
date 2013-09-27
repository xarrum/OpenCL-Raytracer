#include "OCL_Program.h"
#include <fstream>
#include <iostream>

#include "CLERR.h"

OCL_Program::OCL_Program( cl_context &context, cl_device_id &deviceId ) 
	: m_context(context), m_deviceId(deviceId)
{
	
}

OCL_Program::OCL_Program( const OCL_Program& o ) 
	: m_context(o.m_context), m_deviceId(o.m_deviceId)
{
}

OCL_Program::~OCL_Program()
{
	cl_int ret;
	ret = clReleaseProgram( m_program );
	CLERR(ret);

	for( int i = 0; i < m_kernels.size(); i++ )
	{
		ret = clReleaseKernel( m_kernels[i] );
		CLERR(ret);
	}
}

void OCL_Program::Init( std::string sourcePath )
{
	std::ifstream file( sourcePath );
	if( !file.is_open() )
	{
		std::cout << "Could not open file \'"<< sourcePath <<"\'\n";
	}
	else
	{
		std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		m_source = source.c_str();
		m_sourceLength = source.length();
		BuildProgram();
	}
}

void OCL_Program::AddKernel( const char* kernel_name )
{
	cl_int ret;
	m_kernels.push_back( clCreateKernel( m_program, kernel_name, &ret ) );
	CLERR(ret);
}

void ReleaseProgram(OCL_Program** program)
{
	(*program)->Release();
	delete *program;
	*program = 0;
}
