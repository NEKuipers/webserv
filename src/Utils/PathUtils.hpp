class PathUtils;	// For include loops

#ifndef PATHUTILS_HPP
#define PATHUTILS_HPP

#include <ostream>
#include <string>

class PathUtils {
	public:
		enum FileType
		{
			DOES_NOT_EXIST,
			FILE,
			DIRECTORY
		};

		static bool 	IsDirectory(const std::string& Path);
		static bool 	IsFile(const std::string& Path);
		static FileType	pathType(const std::string& Path);

	private:
		PathUtils();
		~PathUtils();
		PathUtils(const PathUtils& From);
		PathUtils& operator = (const PathUtils& From);
		// Class variables

		// Class functions

};

#endif
