#pragma once

#include "RequestResponder.hpp"
#include <vector>

class RequestResponderList : public RequestResponder {
	public: 
		RequestResponderList(const ConfigurationState& State);

		~RequestResponderList();

		// Public functions
		virtual const Response* GetResponse(const Request& Request) const;
	private:
		// No copying!
		RequestResponderList(const RequestResponderList& From);
		RequestResponderList& operator = (const RequestResponderList& From);

		std::vector<const RequestResponder*> Responders;
		// Class variables
		
		// Class functions
		
};