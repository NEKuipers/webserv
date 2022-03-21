#pragma once

#include <string>
#include "ConfigLine.hpp"
#include "ConfigRequest.hpp"
#include "ConfigResponse.hpp"

//#include "ConfigBase.hpp"
class ConfigBase;

// This class has all the configurations you could do, max body size, directory listing, root, etc etc
class ConfigurationState {
	public:
		static const int DEFAULT_MAX_BODY_SIZE = 1048576;	// 1 mb
		ConfigurationState();
		ConfigurationState(ConfigBase* RedirectBase);
		ConfigurationState(const ConfigurationState& From);

		~ConfigurationState();

		ConfigurationState& operator = (const ConfigurationState& From);

		bool IsValidWithRequest(const ConfigRequest& Request) const;

		bool EatLine(const ConfigLine& Line);

		ConfigResponse* Redirect(const ConfigRequest& Request, std::string NewUri) const;
		ConfigResponse* Error(const ConfigRequest& Request) const;

		// Public functions
		// TODO: directory, accepted methods
		std::string Root;	// std::filesystem::path (I can't use fancy features, codam says i must use only the oldest of old stuff)
		std::string ErrorUri;
		size_t MaxBodySize;

		std::string InterperetEnvVariable(const std::string& String, const ConfigRequest& Request) const;	// Ill probably want to use some state to change the env stuff, so its here even tho it does not use anything of the state for now
	private:

		ConfigBase* RedirectBase;
		// Class variables
		
		// Class functions
		
};