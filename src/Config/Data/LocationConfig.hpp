#pragma once

#include "ConfigLine.hpp"
#include <string>
#include <vector>

// For a location block
class LocationConfig {
    public:
        static const std::string DESCRIPTOR;

        LocationConfig();
        LocationConfig(const ConfigLine& Line);
        LocationConfig(const LocationConfig& From);

        virtual ~LocationConfig();

        virtual LocationConfig& operator = (const LocationConfig& From);

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
