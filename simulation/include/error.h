#include <typeinfo>
#include <stack>
#include "pstring.h"
#include "semaphore.h"
#include "log.h"

#ifndef _ERROR
#define _ERROR

namespace errors
{
	namespace interface
	{
		class error
		{
		public:
			virtual bool isError() = 0;
			virtual void makeError(long code) = 0;
			virtual void resetError() = 0;
			virtual long lastError() = 0;
		};
	};

	typedef long errorCode;

	const errorCode NO_ERR = 0L;
	const errorCode UNKNOWN_ERR = 1L;
	const errorCode ALLOCATION_ERR = 2L;
	const errorCode GENERAL_ERR = 3L;
	const errorCode XML_ERR = 4L;
	const errorCode UNDEFINED_ERR = 5L;

	class error
	{
	public:
		string class_str;
		string error_str;

		errorCode code;

		error() 
		{ 
			code = NO_ERR;
		}

		string get();
	};

	class errors
	{
		semaphore::token token;
		std::stack<error> errors;

	public:
		template <class X> void set(string description, errorCode err = UNDEFINED_ERR, bool output = true)
		{
			semaphore lock(token);

			error temp;

			temp.class_str = string(typeid(X).name());
			temp.error_str = description;
			temp.code = err;

			errors.push(temp);

			if (output) 
			{
				string out = temp.get();				
				
				#ifdef WIN32
				out.concat(string("\n"));
				#elif defined __linux__
				out.concat(string("\r\n"));
				#endif
				
				Log << out;
			}
		}

		template <class X> void set(errorCode err = UNDEFINED_ERR, bool output = true)
		{
			semaphore lock(token);

			error temp;

			temp.class_str = string(typeid(X).name());
			temp.code = err;

			errors.push(temp);

			if (output) 
			{
				string out = temp.get();				
				
				#ifdef WIN32
				out.concat(string("\n"));
				#elif defined __linux__
				out.concat(string("\r\n"));
				#endif
				
				Log << out;
			}
		}

		string get();

		bool isempty() { semaphore lock(token); return errors.size() == 0; }
	};
};

extern errors::errors ErrorLogging;

template <class X> void setLastError(string description, errors::errorCode err = errors::UNDEFINED_ERR)
{ 
	ErrorLogging.set<X>(description, err);
}

template <class X> void setLastError(errors::errorCode err = errors::UNDEFINED_ERR)
{
	ErrorLogging.set<X>(err);
}

extern string getLastErrorString();

#endif