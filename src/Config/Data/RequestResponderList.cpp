#include "RequestResponderList.hpp"

RequestResponderList::RequestResponderList(const ConfigurationState& Configuration) : RequestResponder(Configuration) { }

RequestResponderList::~RequestResponderList()
{
	
}

const Response* RequestResponderList::GetResponse(const Request& Request) const
{
	for (std::vector<const RequestResponder*>::const_iterator It = Responders.begin(); It != Responders.end(); It++)
	{
		const Response* Result = It.operator*()->GetResponse(Request);
		if (Result)   
			return Result;
	}
	return NULL;
}