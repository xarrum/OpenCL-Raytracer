#ifndef OCL_ENVIRONMENT_H
#define OCL_ENVIRONMENT_H

#include <CL/cl_old.h>
#include <vector>
#include "OCL_Program.h"

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

	cl_platform_id		m_platformId;
	cl_device_id		m_deviceId;
	cl_uint				m_numDevices;
	cl_uint				m_numPlatforms;
	cl_context			m_context;
	cl_command_queue	m_commandQueue;

	std::vector<OCL_Program*> programs;
};

#endif