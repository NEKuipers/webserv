#include "ConfigLine_upload.hpp"
#include "ConvertException.hpp"
#include "MethodException.hpp"
#include "ConfigUploadFileResponse.hpp"

#include "PathUtils.hpp"
#include "ToString.hpp"

ConfigLine_upload::ConfigLine_upload() {}
ConfigLine_upload::ConfigLine_upload(const ConfigLine_upload& From)
{
	this->operator=(From);
}
ConfigLine_upload::ConfigLine_upload(const std::string& TargetLocation, const ConfigurationState& Configuration) : ConfigBase(Configuration), TargetLocation(TargetLocation)
{
	if (!Configuration.AcceptsMethod("POST"))
		throw MethodException("upload", "POST");
	
	this->Configuration.AcceptedMethods.clear();
	this->Configuration.AcceptedMethods.push_back("POST");
}

ConfigLine_upload::~ConfigLine_upload()
{

}

ConfigLine_upload& ConfigLine_upload::operator = (const ConfigLine_upload& From)
{
	static_cast<ConfigBase*>(this)->operator=(From);
	TargetLocation = From.TargetLocation;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_upload& ConfigLine_upload)
{
	ConfigLine_upload.Print(Stream);
	return Stream;
}

void ConfigLine_upload::Print(std::ostream& Stream) const
{
	Stream << "upload " << TargetLocation;
}

ConfigLine_upload* ConfigLine_upload::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "upload")
		return NULL;

	if (Args.size() != 2)
		throw ConvertException("ConfigLine", "ConfigLine_upload", "Bad argument count! Expected 2, Got " + to_string(Args.size()));

	return new ConfigLine_upload(Args.at(1), Configuration);
}

ConfigResponse* ConfigLine_upload::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	bool MustValidate = false;
	std::string File = PathUtils::combine(Configuration.GetCombinedRoot(), Configuration.InterperetEnvVariableUserVariables(TargetLocation, &Request, MustValidate));
	if (!PathUtils::IsFile(File))
		return NULL;
	
	ConfigurationState::PathType PathType = Configuration.IsPathValid(File, Request, &File);
	if (MustValidate && (PathType & ConfigurationState::PathType_Invalid))
		return NULL;

	ErrorReasons.AddAllowedMethods(Configuration.AcceptedMethods);	
	return new ConfigUploadFileResponse(File, ErrorReasons);
}

bool ConfigLine_upload::WouldHaveResponded(const ConfigRequest& Request) const
{
	(void)Request;
	return true;
}