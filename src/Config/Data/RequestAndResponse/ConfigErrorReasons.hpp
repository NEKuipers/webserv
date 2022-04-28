class ConfigErrorReasons;	// For include loops

#ifndef CONFIGERRORREASONS_HPP
#define CONFIGERRORREASONS_HPP

#include <ostream>
#include <vector>
#include <string>

class ConfigErrorReasons {
	public: 
		ConfigErrorReasons();
		ConfigErrorReasons(const ConfigErrorReasons& From);

		~ConfigErrorReasons();

		ConfigErrorReasons& operator = (const ConfigErrorReasons& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigErrorReasons& ConfigErrorReasons);
		
		const std::vector<std::string>& GetAllowedMethods() const;
		void AddAllowedMethod(const std::string& Method);
		void AddAllowedMethods(const std::vector<std::string>& Methods);	// TODO: When calling AddAllowedMethods(Configuration.AcceptedMethods), The AcceptedMethods can be "Empty", indicating that every method is accepted, Giving a in-accurate AllowedMethods list

		bool GetAllowsAllMethods() const;

		void Err_WrongMethod();
		void Err_BodyTooBig();
		void Err_ErrorPage();

		bool GetWasWrongMethod() const;
		bool GetWasBodyTooBig() const;
		bool GetWasErrorPage() const;
	private:
		// Class variables
		std::vector<std::string> AllowedMethods;
		bool AllowsAllMethods;

		bool WasWrongMethod;
		bool WasBodyTooBig;
		bool WasErrorPage;
		// Class functions
		
};

#endif
