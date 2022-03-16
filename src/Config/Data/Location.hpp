#pragma once

#include "ConfigLine.hpp"
#include <string>
#include <vector>

// For a location block
class Location {
    public:
        static const std::string DESCRIPTOR;

        Location();
        Location(const ConfigLine& Line);
        Location(const Location& From);

        virtual ~Location();

        virtual Location& operator = (const Location& From);

        // Public functions
        
        // Returns: 0 (No match), 1 or more: (Weight to this match, use location with highest weight for a request)
        // Note: MatchAgainst should be a normalized URL
        int GetMatchWeight(const std::string& MatchAgainst) const;

        // TODO: A function to handle a request after it has been selected (Note: This function takes in the packet info)
    private:
        // Class variables
        std::string Match;

        // Class functions
        
};
