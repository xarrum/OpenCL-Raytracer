#ifndef OCL_PROGRAM_H
#define OCL_PROGRAM_H

#include <CL/cl.h>
#include <string>
#include <vector>

class OCL_Program
{
public:
	virtual void Execute( cl_command_queue commandQueue ) = 0;

protected:
	virtual void BuildProgram() = 0;
	virtual void Release() = 0;

protected:
	OCL_Program( cl_context &context, cl_device_id &deviceId );
	OCL_Program( const OCL_Program& );
	~OCL_Program();

	void Init( std::string sourcePath );

	void AddKernel( const char* kernel_name );

protected:
	template <class T>
	friend T* CreateProgram(cl_context &context, cl_device_id &deviceId, std::string sourcePath);

	friend void ReleaseProgram(OCL_Program** program);

protected:
	cl_program		m_program;
	cl_context		&m_context;
	cl_device_id	&m_deviceId;

	std::vector<cl_kernel> m_kernels;

	const char	*m_source;
	size_t		m_sourceLength;


};

template <class T>
T* CreateProgram(cl_context &context, cl_device_id &deviceId, std::string sourcePath)
{
	T* obj = new T(context, deviceId);
	obj->Init(sourcePath);
	return obj;
}

#endif