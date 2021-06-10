#include <iostream>
#include <tuple>
#include <map>
#include <vector>
#include <type_traits>
#include "pstring.h"
#include "properties.h"
//#include "xyzw.h"

#ifndef _JTYPES
#define _JTYPES

namespace jtypes
{
    /*
    class header
	{
	public:
		long version;
		long duration;
		long minimum;

        string url;
        string fitness;

    public:
        header() { clear(); }
        header(const header &source) { copy(source); }

        void clear()
        {
            version = 0L;
            duration = 0L;
            minimum = 0L;

            url.erase();
            fitness.erase();
        }

        void copy(const header &src)
        {
            version = src.version;
            duration = src.duration;
            minimum = src.minimum;
            url = src.url;
            fitness = src.fitness;
        }

    public:
        header operator=(const header &src) { copy(src); return *this; }

        bool operator==(const header& rhs) const
        {
            return std::tie(version, duration, minimum, url, fitness) == std::tie(rhs.version, rhs.duration, rhs.minimum, rhs.url, rhs.fitness);
        }

        constexpr static auto properties = std::make_tuple(
            json::property(&header::version, "version"),
            json::property(&header::duration, "duration"),
            json::property(&header::minimum, "minimum"),
            json::property(&header::url, "url"),
            json::property(&header::fitness, "fitness")
        );
	};

    class collide
    {
    public:
        long value;
        long idx;

        int x, y, z, w;

    public:
        collide() { }

    public:
        bool operator==(const collide& rhs) const
        {
            return std::tie(value, x, y, z, w, idx) == std::tie(rhs.value, rhs.x, rhs.y, rhs.z, rhs.w, rhs.idx);
        }

        constexpr static auto properties = std::make_tuple(
            json::property(&collide::value, "value"),
            json::property(&collide::x, "x"),
			json::property(&collide::y, "y"),
			json::property(&collide::z, "z"),
			json::property(&collide::w, "w"),
            json::property(&collide::idx, "idx")
        );
    };

    class movement
    {
    public:
        long idx;

        int x, y, z, w;

    public:
        movement() { }

    public:
        bool operator==(const movement& rhs) const
        {
            return std::tie(x, y, z, w, idx) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w, rhs.idx);
        }

        constexpr static auto properties = std::make_tuple(
            json::property(&movement::x, "x"),
			json::property(&movement::y, "y"),
			json::property(&movement::z, "z"),
			json::property(&movement::w, "w"),
            json::property(&movement::idx, "idx")
        );
    };

    class sentence
    {
    public:
        const static long LENGTH = 128L;

    public:
        float score;
        string data;

    public:
        sentence(const sentence &source) { copy(source); }

        void copy(const sentence &src)
        {
            score = src.score;
            data = src.data;
        }

    public:
        sentence operator=(const sentence &src) { copy(src); return *this; }

        bool operator==(const sentence& rhs) const
        {
            return std::tie(score, data) == std::tie(rhs.score, rhs.data);
        }

        constexpr static auto properties = std::make_tuple(
            json::property(&sentence::score, "score"),
            json::property(&sentence::data, "sentence")
        );

    public:
        sentence()
        {
            score = 0.0f;
            data.erase();
        }
    };

    class value
    {
    public:
		string text;

    public:
        value() { }
        value(const value &source) { copy(source); }

        void copy(const value &src)
        {
			text = src.text;
        }

    public:
        value operator=(const value &src) { copy(src); return *this; }

        bool operator==(const value& rhs) const
        {
            return std::tie(text) == std::tie(rhs.text);
        }

        constexpr static auto properties = std::make_tuple(
            json::property(&value::text, "text")
        );
    };

    class instruction
    {
    public:
		enum TYPE { INPUT = 0, OUTPUT = 1, RANDOM = 2 };

	public:
		std::vector<value> values;
		float persist, delay;
		long type;

    public:
        instruction() { }
        instruction(const instruction &source) { copy(source); }

		void clear()
		{
			values.clear();

			persist = 0.0f;
			delay = 0.0f;
			type = 0;
		}

        void copy(const instruction &src)
        {
			values.assign(src.values.begin(), src.values.end());
			delay = src.delay;
            persist = src.persist;
			type = src.type;
        }

    public:
        instruction operator=(const instruction &src) { copy(src); return *this; }

        bool operator==(const instruction& rhs) const
        {
            return std::tie(values, delay, type) == std::tie(rhs.values, rhs.delay, rhs.type);
        }

        constexpr static auto properties = std::make_tuple(
            json::property(&instruction::values, "values"),
			json::property(&instruction::delay, "delay"),
			json::property(&instruction::persist, "persist"),
			json::property(&instruction::type, "type")
        );
    };
    */
};

#endif