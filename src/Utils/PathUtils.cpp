#include "PathUtils.hpp"
#include <sys/stat.h>
#include <iostream>

bool PathUtils::IsDirectory(const std::string& Path)
{
	return pathType(Path) & DIRECTORY;
}

bool PathUtils::IsFile(const std::string& Path)
{
	return pathType(Path) & FILE;
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
	{
		if (Path.length() == 0 || Path.at(Path.length() - 1) == '/')
			return (FileType)(DOES_NOT_EXIST | DIRECTORY);
		return (FileType)(DOES_NOT_EXIST | FILE);
	}
}

std::string	PathUtils::combine(const std::string& WorkingDir, const std::string& Relative)
{
	if (Relative.length() == 0)
		return WorkingDir;
	if (Relative.at(0) == '/')
		return Relative;
	if (WorkingDir.length() == 0)
		return Relative;
	
	if (WorkingDir.at(WorkingDir.length() - 1) == '/')
		return WorkingDir + Relative;
	else
		return WorkingDir + "/" + Relative;
}