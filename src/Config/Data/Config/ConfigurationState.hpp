#pragma once

#include <string>
#include "ConfigLine.hpp"
#include "ConfigRequest.hpp"
#include "ConfigResponse.hpp"

// This class has all the configurations you could do, max body size, directory listing, root, etc etc
class ConfigurationState {
	public:
		static const int DEFAULT_MAX_BODY_SIZE = 1048576;	// 1 mb
		ConfigurationState();
		ConfigurationState(const ConfigurationState& From);

		~ConfigurationState();

		ConfigurationState& operator = (const ConfigurationState& From);

		bool IsValidWithRequest(const ConfigRequest& Request) const;

		bool EatLine(const ConfigLine& Line);

		ConfigResponse* Redirect(std::string Uri) const;
		ConfigResponse* Error() const;

		// Public functions
		// Todo: directory, accepted methods
		std::string Root;	// std::filesystem::path (I can't use fancy features, codam says i must use only the oldest of old stuff)
		std::string ErrorUri;
		size_t MaxBodySize;
	private:
		// Class variables
		
		// Class functions
		
};