#include "error.h"
#include <stdlib.h>

string errors::error::get()
{	
	string result;

	if(code != NO_ERR)
	{
		result.concat(string("ERR("));
		result.concat(string::fromLong((long)code));
		result.concat(string(","));
		result.concat(class_str);
		
		if(error_str.length() > 0)
		{
			result.concat(string(","));
			result.concat(error_str);
		}

		result.concat(string(")"));
	}
	else result = string("ERR(0)");

	return result;
}

string errors::errors::get()
{
	semaphore lock(token);

	string result;

	while (errors.size() > 0)
	{
		::errors::error temp = errors.top();

		result.concat(temp.get());

		#ifdef WIN32
		result.concat(string("\r\n"));
		#elif defined __linux__
		result.concat(string("\n"));
		#endif

		errors.pop();
	};

	return result;
}

errors::errors ErrorLogging;

string getLastErrorString() 
{ 
	return ErrorLogging.get();
}
