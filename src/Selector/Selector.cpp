#include "Selector.hpp"
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

#include <cassert>	// linux assert()

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

		//_LIBCPP_UNREACHABLE();
		throw "unreachable";
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

bool Selector::RunAccept(const AcceptData& AcceptData, int ClientFD, const struct sockaddr& Address, socklen_t AddressLen)
{
	try {
		return AcceptData.AcceptFunc(AcceptData.Arg, ClientFD, Address, AddressLen);
	} catch (const std::exception& Exception) {
		std::cerr << "OnAccept(" << AcceptData.fd << ") threw an exception: " << Exception.what();
	} catch (const std::string& Message) {
		std::cerr << "OnAccept(" << AcceptData.fd << ") threw an exception string: " << Message;
	} catch (...) {
		std::cerr << "OnAccept(" << AcceptData.fd << ") threw an exception of unknown type!";	// Too bad i can't use std::current_exception BECAUSE THATS TOO FANCY FOR CODAM (C++11)
	}
	return true;
}
bool Selector::RunRead(const ReadData& ReadData, bool LastRead, const std::string& Read)
{
	try {
		return ReadData.ReadFunc(ReadData.Arg, LastRead, Read);
	} catch (const std::exception& Exception) {
		std::cerr << "OnRead(" << ReadData.fd << ") threw an exception: " << Exception.what();
	} catch (const std::string& Message) {
		std::cerr << "OnRead(" << ReadData.fd << ") threw an exception string: " << Message;
	} catch (...) {
		std::cerr << "OnRead(" << ReadData.fd << ") threw an exception of unknown type!";	// Too bad i can't use std::current_exception BECAUSE THATS TOO FANCY FOR CODAM (C++11)
	}
	return true;
}
bool Selector::RunWrite(const WriteData& WriteData, bool LastWrite, int StartByte, int NumBytes)
{
	try {
		return WriteData.WriteFunc(WriteData.Arg, LastWrite, StartByte, NumBytes);
	} catch (const std::exception& Exception) {
		std::cerr << "OnWrite(" << WriteData.fd << ") threw an exception: " << Exception.what();
	} catch (const std::string& Message) {
		std::cerr << "OnWrite(" << WriteData.fd << ") threw an exception string: " << Message;
	} catch (...) {
		std::cerr << "OnWrite(" << WriteData.fd << ") threw an exception of unknown type!";	// Too bad i can't use std::current_exception BECAUSE THATS TOO FANCY FOR CODAM (C++11)
	}
	return true;
}

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
		//std::cout << "There are " << Ret << " FD's ready!" << std::endl;

		for (size_t i = 0; i < AcceptVector.size(); i++)
		{
			const AcceptData& Curr = AcceptVector[i];

			if (FD_ISSET(Curr.fd, &CurrReadSet))
			{
				//std::cout << "\taccept fd: " << Curr.fd << std::endl;
				struct sockaddr Address;
				socklen_t AddressLen = sizeof(Address);
				int ClientFD = accept(Curr.fd, &Address, &AddressLen);
				//std::cout << "\t\tNew client: " << ClientFD << std::endl;
				if (RunAccept(Curr, ClientFD, Address, AddressLen))
				{
					const AcceptData& Curr = AcceptVector[i];

					FD_CLR(Curr.fd, &ReadSet);
					AcceptVector.erase(AcceptVector.begin() + i--);
					continue;
				}
			}
		}

		for (size_t i = 0; i < ReadVector.size(); i++)
		{
			const ReadData& Curr = ReadVector[i];
			if (FD_ISSET(Curr.fd, &CurrReadSet))
			{
				//std::cout << "\tread fd: " << Curr.fd << std::endl;
				ssize_t ReadChars = read(Curr.fd, Buffer, BUFFER_SIZE);
				bool End = ReadChars <= 0;
				if (RunRead(Curr, ReadChars <= 0, std::string(Buffer, End ? 0 : ReadChars)) || End)
				{
					const ReadData& Curr = ReadVector[i];

					FD_CLR(Curr.fd, &ReadSet);
					ReadVector.erase(ReadVector.begin() + i--);
					continue;
				}
			}
		}

		for (size_t i = 0; i < WriteVector.size(); i++)
		{
			WriteData& Curr = WriteVector[i];
			if (FD_ISSET(Curr.fd, &CurrWriteSet))
			{
				//std::cout << "\twrite fd: " << Curr.fd << std::endl;

				ssize_t Start = Curr.Written;

				ssize_t WroteBytes = write(Curr.fd, Curr.ToWrite.c_str() + Curr.Written, Curr.ToWrite.length() - Curr.Written);
				Curr.Written += WroteBytes;

				bool End = Curr.Written >= Curr.ToWrite.length() || WroteBytes < 0;
				if (RunWrite(Curr, End, Start, WroteBytes) || End)
				{
					WriteData& Curr = WriteVector[i];

					WriteData* Next = Curr.Next;
					if (!Next)
					{
						FD_CLR(Curr.fd, &WriteSet);
						WriteVector.erase(WriteVector.begin() + i--);
						continue;
					}

					Curr = *Next;
					delete Next;
				}
			}
		}

		DecrementMaxFD();
	}
}

bool Selector::DefaultOnAcceptFunction(void* Arg, int ClientFD, const struct sockaddr& Address, socklen_t AddressLen) { (void)Arg; (void)ClientFD; (void)Address; (void)AddressLen; return false; }
bool Selector::DefaultOnReadFunction(void* Arg, bool LastRead, const std::string& Read) { (void)Arg; (void)LastRead; (void)Read; return false; }
bool Selector::DefaultOnWriteFunction(void* Arg, bool LastWrite, int StartByte, int NumBytes) { (void)Arg; (void)LastWrite; (void)StartByte; (void)NumBytes; return false; }