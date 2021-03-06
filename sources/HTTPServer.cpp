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
//myPortNumber(80),
isInitOk(false)
{
	myPortNumber = 80;
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
	
	myConfigFile  = YAML::LoadFile(configFile.c_str());

	return true;
}

std::string HTTPServer::searchRequest(const char* command, const char* url)
{
	if(myConfigFile[command])
        {
                std::cout << command  << " is present"  << std::endl;
		return myConfigFile[command].as<std::string>();
        }
        else
        {
		return std::string("404 Not found !");
        }
}


int HTTPServer::initConnection()
{	
	struct sockaddr_in servAddr;
	
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

        if (bind(myBindSockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	{
		std::cout << "ERROR coudn't bind the socket" << std::endl;
		return -1;
	}
	
	isInitOk = true;
	return 0;
	
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

        	// Get The command and give it to the search command
        	if (n < 0) 
		{
			std::cout << "ERROR reading from socket" << std::endl;
        		return -1;
		}

		// Search in the config file if the command is present
		//
		
		std::string delimiter = " ";
		std::string messageReceived(buffer);
		size_t pos = 0;
		std::string token;
		while ((pos = messageReceived.find(delimiter)) != std::string::npos) {
    			token = messageReceived.substr(0, pos);
    			std::cout << token << std::endl;
    			messageReceived.erase(0, pos + delimiter.length());
		}
		std::cout << messageReceived << std::endl;

		//std::string response = searchRequest();
		
		printf("Here is the message: %s\n",buffer);

        	// Write the response to the client
        	n = write(myAcceptSockFd,"404 Not Found",18);
        
		//Verify is the message could be sent
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
