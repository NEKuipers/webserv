class Selector;	// For include loops

#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include <ostream>
#include <vector>
#include <sys/socket.h>

class Selector {
	public: 
		// All these should return true when the FD should be cleared
		typedef bool (*OnAcceptFunction)(void* Arg, int ClientFD, const struct sockaddr& Address, socklen_t AddressLen);
		typedef bool (*OnReadFunction)(void* Arg, bool LastRead, const std::string& Read);
		typedef bool (*OnWriteFunction)(void* Arg, bool LastWrite, int StartByte, int NumBytes);
		typedef void (*OnTickFunction)(void* Arg);

		Selector();
		~Selector();

		void OnAccept(int fd, void* Arg, OnAcceptFunction OnAccept);
		void OnRead(int fd, void* Arg, OnReadFunction OnRead);
		void Write(int fd, const std::string& ToWrite, void* Arg, OnWriteFunction OnWrite);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const Selector& Selector);
		
		int Start(void* Arg, OnTickFunction OnTick, struct timeval *timeout);
	private:

		Selector(const Selector& From);
		Selector& operator = (const Selector& From);
		void DecrementMaxFD();

		// Class variables
		fd_set ReadSet;
		fd_set WriteSet;
		int MaxFd;

		struct AcceptData
		{
			int fd;
			void* Arg;
			OnAcceptFunction AcceptFunc;

			AcceptData(int fd, void* Arg, OnAcceptFunction AcceptFunc);
		};
		struct ReadData
		{
			int fd;
			void* Arg;
			OnReadFunction ReadFunc;

			ReadData(int fd, void* Arg, OnReadFunction ReadFunc);
		};
		struct WriteData
		{
			int fd;
			std::string ToWrite;
			void* Arg;
			OnWriteFunction WriteFunc;
			size_t Written;

			WriteData(int fd, std::string ToWrite, void* Arg, OnWriteFunction ReadFunc);

			WriteData* Next;
		};

		std::vector<AcceptData> AcceptVector;
		std::vector<ReadData> ReadVector;
		std::vector<WriteData> WriteVector;
		
		
		// Class functions

		// If only there where parameter packs, i'd only have 2 functions!
		static bool DefaultOnAcceptFunction(void* Arg, int ClientFD, const struct sockaddr& Address, socklen_t AddressLen);
		static bool DefaultOnReadFunction(void* Arg, bool LastRead, const std::string& Read);
		static bool DefaultOnWriteFunction(void* Arg, bool LastWrite, int StartByte, int NumBytes);

		static bool RunAccept(const AcceptData& AcceptData, int ClientFD, const struct sockaddr& Address, socklen_t AddressLen);
		static bool RunRead(const ReadData& ReadData, bool LastRead, const std::string& Read);
		static bool RunWrite(const WriteData& WriteData, bool LastWrite, int StartByte, int NumBytes);
};

#endif
