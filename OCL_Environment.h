#ifndef OCL_ENVIRONMENT_H
#define OCL_ENVIRONMENT_H

#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <GLFWInclude.h>

#include <vector>
#include <string>
#include "OCL_Program.h"

#pragma OPENCL EXTENSION cl_khr_gl_sharing : enable
//http://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/gl_sharing.html
//http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/cl_khr_gl_sharing.html

//http://www.gamedev.net/blog/1241/entry-2254210-realtime-raytracing-with-opencl-ii/

class OCL_Environment
{
public:
	OCL_Environment();
	OCL_Environment(const OCL_Environment&);
	~OCL_Environment();
	void Initialize();
	void Release();

	// This is where all the action happens
	void Execute();


private:
	void InitOpenCL();
	void AddProgram( OCL_Program* program );
	
	std::string GetPlatformInfo( cl_platform_id platform_id, cl_platform_info info );
	std::string GetDeviceInfo( cl_device_id device_id, cl_device_info info );
	
	cl_uint				m_numDevices;
	cl_uint				m_numPlatforms;

	std::vector<cl_platform_id>	m_platformIds;
	std::vector<cl_device_id> m_deviceIds;

	cl_context			m_context;
	cl_command_queue	m_commandQueue;

	std::vector<OCL_Program*> programs;
};

#endif