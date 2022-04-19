#include "ConfigRequest.hpp"

//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>

ConfigRequest::ConfigRequest() {}
ConfigRequest::ConfigRequest(const ConfigRequest& From)
{
	this->operator=(From);
}
ConfigRequest::ConfigRequest(in_addr_t Addr, in_port_t Port, const std::string& ServerName, const std::string& Path, size_t ContentLength, const std::string& Method) 
	: Addr(Addr), Port(Port), ServerName(ServerName), Path(Path),ContentLength(ContentLength), Method(Method), PreviousPaths()
{
	if (this->Path.length() > 0 && this->Path.at(0) == '/')
		this->Path = this->Path.substr(1);	// Remove / at the start
}

ConfigRequest::ConfigRequest(const ConfigRequest& From, const std::string& NewPath) 
	: Addr(From.Addr), Port(From.Port), ServerName(From.ServerName), Path(NewPath), ContentLength(From.ContentLength), Method(From.Method), PreviousPaths(From.PreviousPaths)
{
	PreviousPaths.push_back(NewPath);
}

ConfigRequest::~ConfigRequest()
{
	
}

ConfigRequest& ConfigRequest::operator = (const ConfigRequest& From)
{
	this->Port = From.Port;
	this->Addr = From.Addr;
	this->ServerName = From.ServerName;
	this->Path = From.Path;
	this->ContentLength = From.ContentLength;
	this->Method = From.Method;
	this->PreviousPaths = From.PreviousPaths;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigRequest& ConfigRequest)
{
	in_addr DumbHistoricalReasonsStruct;
	DumbHistoricalReasonsStruct.s_addr = ConfigRequest.Addr;

	Stream << ConfigRequest.ServerName << " (" << inet_ntoa(DumbHistoricalReasonsStruct) << ":" << ntohs(ConfigRequest.Port) << ") ";
	Stream << ConfigRequest.Method << " " << ConfigRequest.Path;
	if (ConfigRequest.ContentLength != 0)
		Stream << " content-length: " << ConfigRequest.ContentLength;
	return Stream;
}


in_port_t ConfigRequest::GetPort() const { return Port; }
in_addr_t ConfigRequest::GetAddr() const { return Addr; }
const std::string& ConfigRequest::GetServerName() const { return ServerName; }

const std::string& ConfigRequest::GetPath() const { return Path; }
size_t ConfigRequest::GetContentLength() const { return ContentLength; }
const std::string& ConfigRequest::GetMethod() const { return Method; }

ConfigRequest* ConfigRequest::RedirectPath(std::string NewPath) const
{
	// Make sure we are not in a loop
	for (std::vector<std::string>::const_iterator It = PreviousPaths.begin(); It != PreviousPaths.end(); It++)
		if (*It == NewPath)
			return NULL;
	return new ConfigRequest(*this, NewPath);
}
