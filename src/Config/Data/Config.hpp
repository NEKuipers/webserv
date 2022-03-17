#pragma once

#include "ConfigFile.hpp"
#include "RequestResponderList.hpp"

class Config : public RequestResponderList {
	public: 
		Config(const ConfigFile& File);
		Config(const Config& From);

		~Config();

		Config& operator = (const Config& From);

		// Public functions
	private:
		// Class variables
		
		// Class functions
		
};