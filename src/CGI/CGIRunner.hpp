class CGIRunner;	// For include loops

#ifndef CGIRUNNER_HPP
#define CGIRUNNER_HPP

#include <ostream>
#include <map>

class CGIRunner {
	public:
		CGIRunner(const std::string& PathName, const std::map<std::string, std::string>& ExtraEnv);
		
		~CGIRunner();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const CGIRunner& CGIRunner);

		const int InputFD;
		const int OutputFD;
		const pid_t CGIPid;
	private:
		CGIRunner();
		CGIRunner(const CGIRunner& From);
		CGIRunner& operator = (const CGIRunner& From);
};

#endif
