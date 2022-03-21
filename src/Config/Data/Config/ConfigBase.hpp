#pragma once

#include "ConfigurationState.hpp"
#include "ConfigRequest.hpp"
#include "ConfigResponse.hpp"

class ConfigBase {
	public:
		ConfigBase();
		ConfigBase(const ConfigBase& From);
		ConfigBase(const ConfigurationState& Configuration);

		ConfigBase& operator = (const ConfigBase& From);

		virtual ~ConfigBase();

		// Public functions
		ConfigResponse* GetResponse(const ConfigRequest& Request) const;	// Basically checks the Configuration if its valid with this request, if it is valid, returns GetBaseResponse(), otherwise returns null

		friend std::ostream& operator<<(std::ostream& Stream, const ConfigBase& ConfigBase);
		virtual void Print(std::ostream& Stream) const = 0;	// Apparently this is how you do virtual logging?
	protected:

		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request) const = 0;
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