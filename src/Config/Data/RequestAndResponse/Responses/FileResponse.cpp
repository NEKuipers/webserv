#include "FileResponse.hpp"

FileResponse::FileResponse(const std::string& FileName, std::ifstream* Stream) : FileName(FileName), Stream(Stream)
{

}
FileResponse::~FileResponse()
{
	delete Stream;
}

const std::ifstream& FileResponse::GetStream() const { return *Stream; }

std::ostream& operator<<(std::ostream& Stream, const FileResponse& FileResponse)
{
	FileResponse.Print(Stream);
	return Stream;
}

void FileResponse::Print(std::ostream& PrintStream) const
{
	PrintStream << "FileResponse " << FileName;
}