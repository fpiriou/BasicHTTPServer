#pragma once

#include "yaml-cpp/yaml.h"
#include <string>
#include <map>

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
	std::string searchRequest(const std::string& command, const std::string& url, const std::string& version);

	YAML::Node myConfigFile;
	int myBindSockFd;
	int myAcceptSockFd;
	int myPortNumber;
	bool isInitOk;
	std::map<int,std::string> mySplittedMessage;

};
