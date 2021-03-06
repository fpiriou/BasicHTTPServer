#include "HTTPServer.h"
#include <iostream>
#include <cstring>

// This program read a config file pass in argument
int main(int argc, char* argv[])
{

	if(argc < 2)
	{
		std::cout <<"Nothing to do"<< std::endl;
		return -1;
	}
	
	if(std::strcmp(argv[1],"") != 0)
	{
		std::string configFile = std::string(argv[1]);
		HTTPServer server(configFile);
		int res = server.waitAndTreatRequest();
		if(res < 0)
			std::cout << "ERROR in Wait and Treat Request" << std::endl;
	}
	return 0;
}
