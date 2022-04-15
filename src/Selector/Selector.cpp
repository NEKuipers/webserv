#include "Selector.hpp"
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

Selector::AcceptData::AcceptData(int fd, void* Arg, OnAcceptFunction AcceptFunc) : fd(fd), Arg(Arg), AcceptFunc(AcceptFunc ? AcceptFunc : DefaultOnAcceptFunction) {}
Selector::ReadData::ReadData(int fd, void* Arg, OnReadFunction ReadFunc) : fd(fd), Arg(Arg), ReadFunc(ReadFunc ? ReadFunc : DefaultOnReadFunction) {}
Selector::WriteData::WriteData(int fd, std::string ToWrite, void* Arg, OnWriteFunction WriteFunc) : fd(fd), ToWrite(ToWrite), Arg(Arg), WriteFunc(WriteFunc ? WriteFunc : DefaultOnWriteFunction), Written(0), Next() {}

Selector::Selector() : MaxFd(0)
{
	FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);
}

Selector::~Selector()
{
	
}

std::ostream& operator<<(std::ostream& Stream, const Selector& Selector)
{
	Stream << "Selector:";
	if (Selector.AcceptVector.size() > 0)
	{
		Stream << "\nAccepts: ";
		for (std::vector<Selector::AcceptData>::const_iterator it = Selector.AcceptVector.begin(); it != Selector.AcceptVector.end(); it++)
			Stream << it->fd;
	}
	if (Selector.ReadVector.size() > 0)
	{
		Stream << "\nReads: ";
		for (std::vector<Selector::ReadData>::const_iterator it = Selector.ReadVector.begin(); it != Selector.ReadVector.end(); it++)
			Stream << it->fd;
	}
	if (Selector.WriteVector.size() > 0)
	{
		Stream << "\nWrites: ";
		for (std::vector<Selector::WriteData>::const_iterator it = Selector.WriteVector.begin(); it != Selector.WriteVector.end(); it++)
			Stream << it->fd;
	}
	return Stream;
}

void Selector::OnAccept(int fd, void* Arg, OnAcceptFunction OnAccept)
{
	assert(FD_ISSET(fd, &ReadSet) == false);
	AcceptVector.push_back(AcceptData(fd, Arg, OnAccept));

	FD_SET(fd, &ReadSet);
	MaxFd = std::max(MaxFd, fd);
}
void Selector::OnRead(int fd, void* Arg, OnReadFunction OnRead)
{
	assert (FD_ISSET(fd, &ReadSet) == false);
	ReadVector.push_back(ReadData(fd, Arg, OnRead));

	FD_SET(fd, &ReadSet);
	MaxFd = std::max(MaxFd, fd);
}
void Selector::Write(int fd, const std::string& ToWrite, void* Arg, OnWriteFunction OnWrite)
{
	if (FD_ISSET(fd, &WriteSet))
	{
		for (std::vector<WriteData>::iterator it = WriteVector.begin(); it != WriteVector.end(); it++)
			if (it->fd == fd)
			{
				WriteData* Data = &(*it);
				while (Data->Next)
					Data = Data->Next;
				Data->Next = new WriteData(fd, ToWrite, Arg, OnWrite);
				return;
			}
		
		_LIBCPP_UNREACHABLE();
	}

	WriteVector.push_back(WriteData(fd, ToWrite, Arg, OnWrite));

	FD_SET(fd, &WriteSet);
	MaxFd = std::max(MaxFd, fd);
}

void Selector::DecrementMaxFD()
{
	while (MaxFd > 1 && !FD_ISSET(MaxFd, &ReadSet) && !FD_ISSET(MaxFd, &WriteSet))
		MaxFd--;
}

#include <iostream>	// a
int Selector::Start()
{
	char Buffer[BUFFER_SIZE];

	while (true)
	{
		fd_set CurrReadSet = ReadSet;
		fd_set CurrWriteSet = WriteSet;
		int Ret = select(MaxFd + 1, &CurrReadSet, &CurrWriteSet, NULL, NULL);
		if (Ret < 0)
			return Ret;
		std::cout << "There are " << Ret << " FD's ready!" << std::endl;
		
		for (std::vector<AcceptData>::iterator it = AcceptVector.begin(); it != AcceptVector.end();)
		{
			if (FD_ISSET(it->fd, &CurrReadSet))
			{
				std::cout << "\taccept fd: " << it->fd << std::endl;
				struct sockaddr Address;
				socklen_t AddressLen = sizeof(Address);
				int ClientFD = accept(it->fd, &Address, &AddressLen);
				std::cout << "\t\tNew client: " << ClientFD << std::endl;
				if (it->AcceptFunc(it->Arg, ClientFD, Address, AddressLen))
				{
					FD_CLR(it->fd, &ReadSet);
					it = AcceptVector.erase(it);
					continue;
				}
			}
			it++;
		}

		for (std::vector<ReadData>::iterator it = ReadVector.begin(); it != ReadVector.end();)
		{
			if (FD_ISSET(it->fd, &CurrReadSet))
			{
				std::cout << "\tread fd: " << it->fd << std::endl;
				ssize_t ReadChars = read(it->fd, Buffer, BUFFER_SIZE);
				bool End = ReadChars <= 0;
				if (it->ReadFunc(it->Arg, ReadChars <= 0, std::string(Buffer, End ? 0 : ReadChars)) || End)
				{
					FD_CLR(it->fd, &ReadSet);
					it = ReadVector.erase(it);
					continue;
				}
			}
			it++;
		}
		
		for (std::vector<WriteData>::iterator it = WriteVector.begin(); it != WriteVector.end();)
		{
			if (FD_ISSET(it->fd, &CurrWriteSet))
			{
				std::cout << "\twrite fd: " << it->fd << std::endl;

				ssize_t Start = it->Written;
				
				ssize_t WroteBytes = write(it->fd, it->ToWrite.c_str() + it->Written, it->ToWrite.length() - it->Written);
				it->Written += WroteBytes;

				bool End = it->Written >= it->ToWrite.length() || WroteBytes < 0;
				if (it->WriteFunc(it->Arg, End, Start, WroteBytes) || End)
				{
					WriteData* Next = it->Next;
					if (!Next)
					{
						FD_CLR(it->fd, &WriteSet);
						it = WriteVector.erase(it);
						continue;
					}

					*it = *Next;
					delete Next;
				}
			}
			it++;
		}

		DecrementMaxFD();
	}
}

bool Selector::DefaultOnAcceptFunction(void* Arg, int ClientFD, struct sockaddr Address, socklen_t AddressLen) { (void)Arg; (void)ClientFD; (void)Address; (void)AddressLen; return false; }
bool Selector::DefaultOnReadFunction(void* Arg, bool LastRead, const std::string& Read) { (void)Arg; (void)LastRead; (void)Read; return false; }
bool Selector::DefaultOnWriteFunction(void* Arg, bool LastWrite, int StartByte, int NumBytes) { (void)Arg; (void)LastWrite; (void)StartByte; (void)NumBytes; return false; }