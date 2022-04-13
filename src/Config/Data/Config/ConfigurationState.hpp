#pragma once

#include <string>
#include "ConfigLine.hpp"
#include "ConfigRequest.hpp"
#include "ConfigResponse.hpp"
#include <vector>

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

		bool AcceptsMethod(const std::string& Method) const;
		bool IsValidWithRequest(const ConfigRequest& Request) const;

		bool EatLine(const ConfigLine& Line);

		ConfigResponse* Redirect(const ConfigRequest& Request, std::string NewUri) const;
		ConfigResponse* Error(const ConfigRequest& Request) const;

		// Public functions
		// TODO: directory
		std::vector<std::string> AcceptedMethods;

		std::string Root;	// std::filesystem::path (I can't use fancy features, codam says i must use only the oldest of old stuff)
		std::string ExpectedRootExtension;	// when searching for a file, it will be $root/$URI, and will only be served if its inside the directory $root/$expectedRootExtension, the location block writes to this variable
		std::string ErrorUri;
		size_t MaxBodySize;

		bool IsFileValid(const std::string& FilePath, const ConfigRequest& Request) const;

		std::string InterperetEnvVariable(const std::string& String) const;
		std::string InterperetEnvVariableUserVariables(const std::string& String, const ConfigRequest* Request, bool& MustValidate) const;
	private:
		ConfigBase* RedirectBase;
		// Class variables
		
		// Class functions
		
};