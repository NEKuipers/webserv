#include "ConfigFileResponse.hpp"
#include <map>
#include <cassert>	// linux assert()

// Why does c++98 NOT have a hashmap!?
std::map<std::string, std::string> g_extension_to_content_type;

void ConfigFileResponse::InitContentTypes()
{
	assert(g_extension_to_content_type.size() == 0);

	g_extension_to_content_type["abw"] = "application/x-abiword";
	g_extension_to_content_type["arc"] = "application/octet-stream";
	g_extension_to_content_type["avi"] = "video/x-msvideo";
	g_extension_to_content_type["azw"] = "application/vnd.amazon.ebook";
	g_extension_to_content_type["bin"] = "application/octet-stream";
	g_extension_to_content_type["bmp"] = "image/bmp";
	g_extension_to_content_type["bz"] = "application/x-bzip";
	g_extension_to_content_type["bz2"] = "application/x-bzip2";
	g_extension_to_content_type["csh"] = "application/x-csh";
	g_extension_to_content_type["css"] = "text/css";
	g_extension_to_content_type["csv"] = "text/csv";
	g_extension_to_content_type["doc"] = "application/msword";
	g_extension_to_content_type["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	g_extension_to_content_type["eot"] = "application/vnd.ms-fontobject";
	g_extension_to_content_type["epub"] = "application/epub+zip";
	g_extension_to_content_type["gif"] = "image/gif";
	g_extension_to_content_type["htm"] = "text/html";
	g_extension_to_content_type["html"] = "text/html";
	g_extension_to_content_type["ico"] = "image/x-icon";
	g_extension_to_content_type["ics"] = "text/calendar";
	g_extension_to_content_type["jar"] = "application/java-archive";
	g_extension_to_content_type["jpeg"] = "image/jpeg";
	g_extension_to_content_type["jpg"] = "image/jpeg";
	g_extension_to_content_type["js"] = "application/javascript";
	g_extension_to_content_type["json"] = "application/json";
	g_extension_to_content_type["mid"] = "audio/midi";
	g_extension_to_content_type["midi"] = "audio/midi";
	g_extension_to_content_type["mpeg"] = "video/mpeg";
	g_extension_to_content_type["mpkg"] = "application/vnd.apple.installer+xml";
	g_extension_to_content_type["odp"] = "application/vnd.oasis.opendocument.presentation";
	g_extension_to_content_type["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	g_extension_to_content_type["odt"] = "application/vnd.oasis.opendocument.text";
	g_extension_to_content_type["oga"] = "audio/ogg";
	g_extension_to_content_type["ogv"] = "video/ogg";
	g_extension_to_content_type["ogx"] = "application/ogg";
	g_extension_to_content_type["otf"] = "font/otf";
	g_extension_to_content_type["png"] = "image/png";
	g_extension_to_content_type["pdf"] = "application/pdf";
	g_extension_to_content_type["ppt"] = "application/vnd.ms-powerpoint";
	g_extension_to_content_type["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	g_extension_to_content_type["rar"] = "application/x-rar-compressed";
	g_extension_to_content_type["rtf"] = "application/rtf";
	g_extension_to_content_type["sh"] = "application/x-sh";
	g_extension_to_content_type["svg"] = "image/svg+xml";
	g_extension_to_content_type["swf"] = "application/x-shockwave-flash";
	g_extension_to_content_type["tar"] = "application/x-tar";
	g_extension_to_content_type["tif"] = "image/tiff";
	g_extension_to_content_type["tiff"] = "image/tiff";
	g_extension_to_content_type["ts"] = "application/typescript";
	g_extension_to_content_type["ttf"] = "font/ttf";
	g_extension_to_content_type["vsd"] = "application/vnd.visio";
	g_extension_to_content_type["wav"] = "audio/x-wav";
	g_extension_to_content_type["weba"] = "audio/webm";
	g_extension_to_content_type["webm"] = "video/webm";
	g_extension_to_content_type["webp"] = "image/webp";
	g_extension_to_content_type["woff"] = "font/woff";
	g_extension_to_content_type["woff2"] = "font/woff2";
	g_extension_to_content_type["xhtml"] = "application/xhtml+xml";
	g_extension_to_content_type["xls"] = "application/vnd.ms-excel";
	g_extension_to_content_type["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	g_extension_to_content_type["xml"] = "application/xml";
	g_extension_to_content_type["xul"] = "application/vnd.mozilla.xul+xml";
	g_extension_to_content_type["zip"] = "application/zip";
	g_extension_to_content_type["3gp"] = "audio/3gpp";
	g_extension_to_content_type["3g2"] = "audio/3gpp2";
	g_extension_to_content_type["7z"] = "application/x-7z-compressed";
}

ConfigFileResponse::ConfigFileResponse(const std::string& FileName, std::ifstream* Stream, const ConfigErrorReasons& ErrorReasons) : ConfigResponse(ErrorReasons), FileName(FileName), Stream(Stream)
{

}
ConfigFileResponse::~ConfigFileResponse()
{
	delete Stream;
}

const std::ifstream& ConfigFileResponse::GetStream() const { return *Stream; }

std::ostream& operator<<(std::ostream& Stream, const ConfigFileResponse& ConfigFileResponse)
{
	ConfigFileResponse.Print(Stream);
	return Stream;
}

void ConfigFileResponse::Print(std::ostream& PrintStream) const
{
	PrintStream << "FileResponse '" << FileName << "' Type: " << GetContentType();
}

const std::string& ConfigFileResponse::GetFileName()
{
	return FileName;
}

const static std::string DefaultContentType = "application/octet-stream";
const std::string& ConfigFileResponse::GetContentType() const
{
	assert(g_extension_to_content_type.size() != 0);

	size_t Dot = FileName.rfind(".");
	if (Dot != std::string::npos)
	{
		std::string Extension = FileName.substr(Dot + 1);
		std::map<std::string, std::string>::const_iterator it = g_extension_to_content_type.find(Extension);
		if (it != g_extension_to_content_type.end())
			return it->second;
	}
	return DefaultContentType;
}