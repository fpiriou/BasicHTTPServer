#include "HTTPServer.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

HTTPServer::HTTPServer(const std::string& newConfFile):
myPortNumber(TCP_PORT),
isInitOk(false)
{
	mySplittedMessage.clear();
	if(initConnection() < 0)
		std::cout << "Problem to init connection" << std::endl;
	else
		loadConfigFile(newConfFile);
}

HTTPServer::~HTTPServer()
{
	closeConnection();
}

bool HTTPServer::loadConfigFile(const std::string& configFile)
{
	std::cout << "Loading "<< configFile  << " file"  << std::endl;
	
	// Load the yaml file
	myConfigFile  = YAML::LoadFile(configFile.c_str());

	return true;
}

// Search in the file what type of Node we found
int searchTypeNode(const char* string, YAML::Node& node)
{
	if(node[string].IsDefined())
	{
		if(node[string].IsMap())
		{	
			return 1;
		}
		if(node[string].IsNull())
		{
			return 2;
		}
		if(node[string].IsSequence())
		{
			return 3;
		}
		if(node[string].IsScalar())
		{
			return 4;
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

std::string HTTPServer::searchRequest(const std::string& command, const std::string& url, const std::string& version)
{
	bool isFound = false;

	// Check if url is find
	for (YAML::iterator it = myConfigFile.begin() ; it!=myConfigFile.end(); it++)
	{
		if(it->first.as<std::string>() == url)
		{
			isFound = true; 
		}
		else
		{
			int nodeType = searchTypeNode(it->first.as<std::string>().c_str(), myConfigFile);
			switch(nodeType)
			{
				// The node is defined
				case 0: break;
				// The node is a map
				case 1:
					{
						// Explore the map
						const std::string& secondLevel = it->first.as<std::string>();
						for(YAML::iterator it2Level = myConfigFile[secondLevel.c_str()].begin() ; it2Level!=myConfigFile[secondLevel.c_str()].end(); it2Level++)
						{
							if(it2Level->first.as<std::string>() == url)
								isFound = true;
							else
							{
								// Explore the third level 
							}
						}
					}
					break;
				// The node is null	
				case 2: break;
				// The node is a sequence
				case 3: break;
				// The node is a scalar
				case 4:	break;
				default: break;
			}
		}
	}

	if(isFound)
	{
		std::cout << url  << " is present"  << std::endl;
		return ("200 OK");
	}
	else
	{
		std::cout << url  << " is not found"  << std::endl;
		return std::string("404 Not Found");
	}
}


int HTTPServer::initConnection()
{	
	struct sockaddr_in servAddr;
	
	// Creation of the socket
	myBindSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (myBindSockFd < 0)
	{     
		std::cout << "ERROR opening socket" << std::endl;
		return -1;
	}

	bzero((char *) &servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(myPortNumber);
	// Here we bind a socket to the server address
	if (bind(myBindSockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	{
		std::cout << "ERROR coudn't bind the socket" << std::endl;
		return -1;
	}
	
	isInitOk = true;
	return 0;
}

void getSplittedMessage(std::map<int,std::string>& map, std::string& message)
{
	std::string delimiter = " ";
	size_t pos = 0;
	int mapPos = 0;
	// We get the first three strings who are the command ,the url and the http version
	while ((pos = message.find(delimiter)) != std::string::npos || mapPos < 4) 
	{
		map[mapPos] = message.substr(0, pos);
		message.erase(0, pos + delimiter.length());
		mapPos++;
	}

	// Remove the \n in the line and keep the first string
	std::string strCRLF = "\n";
	std::string tmp;
	size_t nLocCRLF = map[2].find(strCRLF);
	tmp = map[2].substr(0,nLocCRLF-1);
	map[2] = tmp;
}

int HTTPServer::waitAndTreatRequest()
{
	int n = 0;
	char buffer[256];
	struct sockaddr_in cliAddr;
	socklen_t cliLen;
	
	if(isInitOk)
	{
		// This code acccept the client connection 
		listen(myBindSockFd,5);
		cliLen = sizeof(cliAddr);
		myAcceptSockFd = accept(myBindSockFd, (struct sockaddr *) &cliAddr, &cliLen);
		if (myAcceptSockFd < 0)
		{
			std::cout << "ERROR on accept"<< std::endl;
			return -1;
		}

		bzero(buffer,256);
		n = read(myAcceptSockFd,buffer,255);

		if (n < 0) 
		{
			std::cout << "ERROR reading from socket" << std::endl;
			return -1;
		}

		// Search in the config file if the command is present
		std::string messageReceived(buffer);
		getSplittedMessage(mySplittedMessage, messageReceived);

		std::string response = searchRequest(mySplittedMessage[0], mySplittedMessage[1], mySplittedMessage[2]);
		std::cout <<"The response sent is : "<< response << std::endl;
		
		// Write the response to the client
		n = write(myAcceptSockFd,response.c_str(),response.size());
        
		//Verify if the message could be sent
		if (n < 0) 
		{
			std::cout << "ERROR writing to socket" << std::endl;
			return -1;
		}

	}
	return 0;
}

void HTTPServer::closeConnection()
{
	if(isInitOk)
	{
		// Close the sockets
		close(myAcceptSockFd);
		close(myBindSockFd);
	}	
}