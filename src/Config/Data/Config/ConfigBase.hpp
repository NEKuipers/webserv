class ConfigBase;	// For include loops

#ifndef ConfigBase_HPP
#define ConfigBase_HPP

#include "ConfigurationState.hpp"
#include "ConfigRequest.hpp"
#include "ConfigResponse.hpp"
#include "ConfigCombinedResponse.hpp"

class ConfigBase {
	public:
		ConfigBase();
		ConfigBase(const ConfigBase& From);
		ConfigBase(const ConfigurationState& Configuration);

		ConfigBase& operator = (const ConfigBase& From);

		virtual ~ConfigBase();

		// Public functions
		ConfigResponse* GetResponse(const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const;	// Basically checks the Configuration if its valid with this request, if it is valid, returns GetBaseResponse(), otherwise returns null
		// Note: When calling *It must be equal to this
		virtual ConfigResponse* GetIteratorResponse(std::vector<ConfigBase*>::const_iterator& It, const std::vector<ConfigBase*>::const_iterator& ItEnd, const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const;	// I would have liked this to be a templated iterator type, but virtuals and templates dont mix, and i dont really feel like going down the rabbit hole of type erasure

		friend std::ostream& operator<<(std::ostream& Stream, const ConfigBase& ConfigBase);
		virtual void Print(std::ostream& Stream) const = 0;	// Apparently this is how you do virtual logging?

	protected:		
		virtual bool ChecksConfiguration() const;
		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const = 0;
		virtual void AddCombinedResponseIfNoResponse(const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const;
		ConfigurationState Configuration;
	private:
		// Class variables
		
		// Class functions
		
};

// Config lines:
//V	listen host:port
//V	server_names
//V	default error page
//V	limit client body size
//V	routes (aka locations):
//V		Accepted HTTP methods
//V		HTTP redirection
//V		Define directory or file from where the file should be searched
//V		Turn on or off directory listing
//V	default file if file is directory
// 	execute CGI based on file extension (eg .php)
//V	accept uploaded file & configure where they should be saved



// listen (host:[port ?? 80])[1..];		listens to this host and port combination
// server_name [name][1..];				When multiple servers are listening to the same host&port, use the server name header to choose what server will process the request
// client_max_body_size [size];			The maximum content length in a header
// redirect [new_uri];					Changes the URI of the request
// return [code] [text];				Returns code with text
// directory [what];					Sets directory listing on/off/file
// error_page [new_uri]					If we did not find a response, redirect to new_uri


#endif
