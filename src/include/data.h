#include "pstring.h"

#ifndef _DATA
#define _DATA

namespace data
{
	namespace fifo
	{
		template <class X> class source
		{
		public:
			virtual bool get(X &destination) = 0;
			virtual bool isempty() = 0;

			virtual string identifier() = 0;

			virtual unsigned long entries() = 0;
		};

		template <class X> class destination
		{
		public:
			virtual bool set(X &source) = 0;
			virtual bool isfull() = 0;

			virtual string identifier() = 0;
		};

		template <class X> class bidirectional : public source<X>, public destination<X>
		{

		};
	};

	namespace array
	{
		template <class X> class source
		{
		public:
			virtual X* get(unsigned long index) = 0;

			virtual bool resize(unsigned long length) = 0;

			virtual unsigned long entries() = 0;
		};
	};
};

#endif