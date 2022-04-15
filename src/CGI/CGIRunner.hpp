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

		// Enqueues a bit of the body to write
		// Whenever this is called make sure the InputFD is set in your select() function
		void QueuePartialBodyForWrite(const std::string& PartialBody);

		// Should be called when select() says that can write something
		// Returns wheter the current partial body was completely written, So if it returns true remove InputFD from your select() function
		bool Write();
		// Should be called when select() says it can read something
		// Returns wheter the entire CGI output was read
		// Appends what it read to AppendRead
		bool Read(std::string& AppendRead);

		const int InputFD;
		const int OutputFD;
		const pid_t CGIPid;
	private:

		CGIRunner();
		CGIRunner(const CGIRunner& From);
		CGIRunner& operator = (const CGIRunner& From);

		// Class variables
		std::string ToWritePartialBody;
		// Class functions
		
};

#endif
