# BasicHTTPServer

## Overview

This program is a restricted implementation of a HTTP server.
After the connection step passed successfully, this server load an YAML configuration file OpenApi 3.0 and wait for an HTTP request.
When a request arrive, the server search in the configuration file if the request is present in the file.
If the request is find, the server respond : HTTP_VERSION 200 OK otherwise it respond HTTP_VERSION 404 Not Found.
The HTTP version depends on the request's version received.
After the response sent, the server close the connection.

## External dependencies

The program use the libyaml-cpp library to parse the configuration file. So this library should be present in a directory named ExternalsLibs in the project root path. This library is present on GitHub.

The first step is to create the ExternalsLibs directory:

`$mkdir ExternalsLibs`

`$cd ExternalsLibs`

And now clone the yaml-cpp repository:

`$git clone https://github.com/jbeder/yaml-cpp.git`

And compile the library :

`$mkdir build`

`$cd build`

`$cmake ..`

`$make`

## Compilation

At this step, we are now able to build the project.
At the root of the project yo have to create the build directory:

`$mkdir build`

`$cd build`

`$cmake ..`

`$make`

## Execution

At this step, you are ready to launch the server. To launch the server, you have to pass in argument the path to the configuration file.
For your information, you have to execute with the super user privilegies:

`$sudo ./BasicHTTPServer /path/to/config/file/file.yaml`

or 

`#./BasicHTTPServer /path/to/config/file/file.yaml`
