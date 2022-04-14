#pragma once

#include <string>
#include "ConfigLine.hpp"
#include "ConfigRequest.hpp"
#include "ConfigResponse.hpp"
#include "ConfigErrorReasons.hpp"
#include <vector>

//#include "ConfigBase.hpp"
class ConfigBase;

// This class has all the configurations you could do, max body size, directory listing, root, etc etc
class ConfigurationState {

	public:
		typedef enum
		{
			Valid = 0,
			BodyTooBig = 1 << 0,
			WrongMethod = 1 << 1
		} ValidRequestReason;

		static const int DEFAULT_MAX_BODY_SIZE = 1048576;	// 1 mb
		ConfigurationState();
		ConfigurationState(ConfigBase* RedirectBase);
		ConfigurationState(const ConfigurationState& From);

		~ConfigurationState();

		ConfigurationState& operator = (const ConfigurationState& From);

		bool AcceptsMethod(const std::string& Method) const;
		ValidRequestReason IsValidWithRequest(const ConfigRequest& Request) const;

		bool EatLine(const ConfigLine& Line);

		void AppendLocationRoot(const std::string& Location);

		ConfigResponse* Redirect(const ConfigRequest& Request, std::string NewUri, ConfigErrorReasons& ErrorReasons) const;
		ConfigResponse* Error(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const;

		const std::string& GetRoot() const;
		const std::string& GetCombinedRoot() const;
		const std::string& GetRawLocationRoot() const;
		const std::string& GetLocationRoot() const;

		std::string RemoveLocationRoot(const std::string& Uri) const;

		// Public functions
		std::vector<std::string> AcceptedMethods;

		std::string ErrorUri;
		size_t MaxBodySize;

		bool IsFileValid(const std::string& FilePath, const ConfigRequest& Request) const;

		std::string InterperetEnvVariable(const std::string& String) const;
		std::string InterperetEnvVariableUserVariables(const std::string& String, const ConfigRequest* Request, bool& MustValidate) const;
	private:
		void UpdateCombinedRoot();

		// std::filesystem::path (I can't use fancy features, codam says i must use only the oldest of old stuff)
		std::string Root;
		std::string RawLocationRoot;	// This is the actual location in the config
		std::string LocationRoot;	// This can be modified by the location_root directive
		std::string CombinedRoot;

		ConfigBase* RedirectBase;
		// Class variables
		
		// Class functions
		
};