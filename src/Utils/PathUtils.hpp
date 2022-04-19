class PathUtils;	// For include loops

#ifndef PATHUTILS_HPP
#define PATHUTILS_HPP

#include <ostream>
#include <string>

class PathUtils {
	public:
		enum FileType
		{
			DOES_NOT_EXIST = 1 << 1,
			FILE = 1 << 2,
			DIRECTORY = 1 << 3
		};

		static bool 		IsDirectory(const std::string& Path);
		static bool 		IsFile(const std::string& Path);
		static FileType		pathType(const std::string& Path);
		static std::string	combine(const std::string& WorkingDir, const std::string& Relative);

	private:
		PathUtils();
		~PathUtils();
		PathUtils(const PathUtils& From);
		PathUtils& operator = (const PathUtils& From);
		// Class variables

		// Class functions

};

#endif
