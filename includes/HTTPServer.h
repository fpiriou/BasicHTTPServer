#pragma once

#include "yaml-cpp/yaml.h"
#include <string>

#define TCP_PORT 80

class HTTPServer
{
public:
	HTTPServer(const std::string& confFile);
	~HTTPServer();
	int waitAndTreatRequest();
private:

	bool loadConfigFile(const std::string& configFile);	
	int initConnection();
	void closeConnection();
	std::string searchRequest(const char* command, const char* url);

	YAML::Node myConfigFile;
	int myBindSockFd;
	int myAcceptSockFd;
	int myPortNumber;
	bool isInitOk;

};
