#include "ConfigRequest.hpp"

//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>

ConfigRequest::ConfigRequest() {}
ConfigRequest::ConfigRequest(const ConfigRequest& From)
{
	this->operator=(From);
}
ConfigRequest::ConfigRequest(in_addr_t Addr, in_port_t Port, const std::string& ServerName, const std::string& Uri, size_t ContentLength, const std::string& Method) 
	: Addr(Addr), Port(Port), ServerName(ServerName), Uri(Uri),ContentLength(ContentLength), Method(Method)
{

}

ConfigRequest::~ConfigRequest()
{
	
}

ConfigRequest& ConfigRequest::operator = (const ConfigRequest& From)
{
	this->Port = From.Port;
	this->Addr = From.Addr;
	this->ServerName = From.ServerName;
	this->Uri = From.Uri;
	this->ContentLength = From.ContentLength;
	this->Method = From.Method;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigRequest& ConfigRequest)
{
	in_addr DumbHistoricalReasonsStruct;
	DumbHistoricalReasonsStruct.s_addr = ConfigRequest.Addr;

	Stream << ConfigRequest.ServerName << " (" << inet_ntoa(DumbHistoricalReasonsStruct) << ":" << ntohs(ConfigRequest.Port) << ") ";
	Stream << ConfigRequest.Method << " " << ConfigRequest.Uri;
	if (ConfigRequest.ContentLength != 0)
		Stream << " content-length: " << ConfigRequest.ContentLength;
	return Stream;
}


in_port_t ConfigRequest::GetPort() const { return Port; }
in_addr_t ConfigRequest::GetAddr() const { return Addr; }
const std::string& ConfigRequest::GetServerName() const { return ServerName; }

const std::string& ConfigRequest::GetUri() const { return Uri; }
size_t ConfigRequest::GetContentLength() const { return ContentLength; }
const std::string& ConfigRequest::GetMethod() const { return Method; }
