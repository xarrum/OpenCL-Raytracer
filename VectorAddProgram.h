#ifndef VECTOR_ADD_PROGRAM_H
#define VECTOR_ADD_PROGRAM_H
#include "OCL_Program.h"

class VectorAddProgram : public OCL_Program
{
public:
	VectorAddProgram( cl_context &context, cl_device_id &deviceId );
	~VectorAddProgram();

	void Execute( cl_command_queue commandQueue );

private:
	void BuildProgram();
	void Release();

private:
	
	int* A;
	int* B;
	int* C;

	cl_mem m_memObjA;
	cl_mem m_memObjB;
	cl_mem m_memObjC;

	int LIST_SIZE;
};

#endif