#include <iostream>
#include <tuple>
#include <string>
#include <map>
#include <vector>
#include <type_traits>

#ifndef _PROPERTIES
#define _PROPERTIES

namespace json
{
    template<typename Class, typename T> struct PropertyImpl 
    {
        constexpr PropertyImpl(T Class::*aMember, const char* aName) : member{aMember}, name{aName} {}

        using Type = T;

        T Class::*member;
        const char* name;
    };

    template<typename Class, typename T> constexpr auto property(T Class::*member, const char* name) 
    {
        return PropertyImpl<Class, T>{member, name};
    }

    template <typename T, T... S, typename F> 
    constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) 
    {
        using unpack_t = int[];
        (void)unpack_t{(static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0};
    }
};

#endif