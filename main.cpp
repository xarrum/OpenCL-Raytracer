#include <iostream>
#include <fstream>
#include <CL\cl.h>

#include "OCL_Environment.h"

int main( int argc, char* argv[] )
{
	// Create and initialize openCL environment
	OCL_Environment env;
	env.Initialize();
	
	env.Execute();
	std::cin.get();

	// Release the openCL environment
	env.Release();
	return 0;
}
