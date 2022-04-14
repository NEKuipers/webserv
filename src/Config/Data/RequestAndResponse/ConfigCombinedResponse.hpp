class ConfigCombinedResponse;	// For include loops

#ifndef CONFIGCOMBINEDRESPONSE_HPP
#define CONFIGCOMBINEDRESPONSE_HPP

#include <ostream>
#include <vector>
#include <string>

class ConfigCombinedResponse {
	public: 
		ConfigCombinedResponse();
		ConfigCombinedResponse(const ConfigCombinedResponse& From);

		~ConfigCombinedResponse();

		ConfigCombinedResponse& operator = (const ConfigCombinedResponse& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigCombinedResponse& ConfigCombinedResponse);
		
		const std::vector<std::string>& GetAllowedMethods() const;
		void AddAllowedMethod(const std::string& Method);
		void AddAllowedMethods(const std::vector<std::string>& Methods);
	private:
		// Class variables
		std::vector<std::string> AllowedMethods;
		// Class functions
		
};

#endif
