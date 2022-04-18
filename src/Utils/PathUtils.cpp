#include "PathUtils.hpp"
#include <sys/stat.h>

bool PathUtils::IsDirectory(const std::string& Path)
{
	if (Path.length() == 0)
		return true;

	char endChar = Path.at(Path.length() - 1);
	if (endChar == '\\' || endChar == '/')
		return true;


	struct stat buf;
	if (stat(Path.c_str(), &buf) == 0)
	{
		if (buf.st_mode & S_IFDIR)
			return true;
	}
	else
	{
		// Not a valid file? So no directory then?
	}



	// Hmmm, probably not a directory then?

	return false;
}

bool PathUtils::IsFile(const std::string& Path)
{
	return !IsDirectory(Path);
}

PathUtils::FileType PathUtils::pathType(const std::string& Path)
{
	struct stat buffer;

	if (stat(Path.c_str(), &buffer) == 0)
	{
		if (S_ISREG(buffer.st_mode))
			return (FILE);
		else
			return (DIRECTORY);
	}
	else
		return (DOES_NOT_EXIST);
}
