class ConfigRequest;	// For include loops

#ifndef CONFIGREQUEST_HPP
#define CONFIGREQUEST_HPP

#include <ostream>
#include <vector>

class ConfigRequest {
	public: 
		ConfigRequest();
		ConfigRequest(const ConfigRequest& From);
		ConfigRequest(in_addr_t Addr, in_port_t Port, const std::string& ServerName, const std::string& Uri, size_t ContentLength, const std::string& Method);

		~ConfigRequest();

		ConfigRequest& operator = (const ConfigRequest& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigRequest& ConfigRequest);

		ConfigRequest* RedirectUri(std::string Uri) const;
		
		in_addr_t GetAddr() const;
		in_port_t GetPort() const;
		const std::string& GetServerName() const;

		const std::string& GetUri() const;
		size_t GetContentLength() const;
		const std::string& GetMethod() const;
	private:
		ConfigRequest(const ConfigRequest& From, const std::string& Uri);

		// Class variables
		in_addr_t Addr;
		in_port_t Port;
		std::string ServerName;

		std::string Uri;
		size_t ContentLength;
		std::string Method;
		// Class functions
		
		std::vector<std::string> PreviousUris;	// To detect redirect loops
};

#endif