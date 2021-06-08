#include "pstring.h"
#include "pair.h"
#include "jtypes.h"
//#include "xyzw.h"
//#include "wsocket.h"
#include "log.h"

#ifndef _JSON
#define _JSON

using namespace jtypes;

namespace json
{
    namespace parser
    {
        class json
        {
        public:
            virtual void clear() = 0;
            virtual bool add(custom::pair &source) = 0;

            virtual json *find(string name) { return NULL; }
            virtual bool terminate() { return true; }

        public:
            bool parse(string json);
        };
    };

    //class value;
    
    //template<typename T> ::json::value toJson(const T& object, wsocket::out *destination);
/*
    class value 
    {
    public:
        wsocket::out *destination;

    public:
        value(wsocket::out *destination)
        {
            this->destination = destination;
        }
    
        value& operator[](string name) 
        {
            string t("\"");
            t.concat(name);
            t.concat(string("\": "));
            
            destination->write(t);

            return *this;
        }

        value& operator=(std::vector<xyzw<char>> value)
        {
            destination->write(string("["));
            for(std::vector<xyzw<char>>::iterator it = value.begin(); it != value.end(); ++it) 
            {
                toJson(*it, destination);
                if(it != value.end() - 1) destination->write(string(", "));
            }
            destination->write(string("]"));

            return *this;
        }

        value& operator=(std::vector<xyzw<int>> value)
        {
            destination->write(string("["));
            for(std::vector<xyzw<int>>::iterator it = value.begin(); it != value.end(); ++it) 
            {
                toJson(*it, destination);
                if(it != value.end() - 1) destination->write(string(", "));
            }
            destination->write(string("]"));

            return *this;
        }

        value& operator=(std::vector<int> value)
        {
            destination->write(string("["));
            for(std::vector<int>::iterator it = value.begin(); it != value.end(); ++it) 
            {
                destination->write(string::fromInt(*it));
                if(it != value.end() - 1) destination->write(string(", "));
            }
            destination->write(string("]"));

            return *this;
        }
        */
/*
        value& operator=(std::vector<collide> value)
        {
            destination->write(string("["));
            for(std::vector<collide>::iterator it = value.begin(); it != value.end(); ++it) 
            {
                toJson(*it, destination);
                if(it != value.end() - 1) destination->write(string(", "));
            }
            destination->write(string("]"));

            return *this;
        }

        value& operator=(std::vector<sentence> value)
        {
            destination->write(string("["));
            for(std::vector<sentence>::iterator it = value.begin(); it != value.end(); ++it) 
            {
                toJson(*it, destination);
                if(it != value.end() - 1) destination->write(string(", "));
            }
            destination->write(string("]"));

            return *this;
        }

        value& operator=(std::vector<movement> value)
        {
            destination->write(string("["));
            for(std::vector<movement>::iterator it = value.begin(); it != value.end(); ++it) 
            {
                toJson(*it, destination);
                if(it != value.end() - 1) destination->write(string(", "));
            }			
            destination->write(string("]"));

            return *this;
        }

        value& operator=(string value) 
        {
            string v("\\\"");

            string t("\"");
            t.concat(value.replace('"',v));
            t.concat(string("\""));
            
            destination->write(t);

            return *this;
        }

        value& operator=(float value) 
        {
            destination->write(string::fromFloat(value));

            return *this;
        }

        value& operator=(long value) 
        {
            destination->write(string::fromLong(value));

            return *this;
        }

        value& operator=(int value) 
        {
            destination->write(string::fromInt(value));

            return *this;
        }

        value& operator=(char value) 
        {            
            destination->write(string::fromInt((int)value));
         
            return *this;
        }


        value& operator=(xyzw<int> value)
        {
            *this = toJson(value, destination);
            return *this;
        }

        value& operator=(xyzw<char> value)
        {
            *this = toJson(value, destination);
            return *this;
        }

        value& operator=(header value) 
        {
            *this = toJson(value, destination);
            return *this;
        }
    
        value& operator=(movement value) 
        {
            *this = toJson(value, destination);
            return *this;
        }

        value& operator=(collide value) 
        {
            *this = toJson(value, destination);
            return *this;
        }

        value& operator=(sentence value) 
        {
            *this = toJson(value, destination);
            return *this;
        }
        
    };    
*/
/*
    template<typename T> ::json::value toJson(const T& object, wsocket::out *destination)
	{        
        ::json::value data(destination);

        constexpr auto nbProperties = std::tuple_size<decltype(T::properties)>::value;

        destination->write(string("{"));
        for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) 
        {
            constexpr auto property = std::get<i>(T::properties);    
            
            data[property.name] = object.*(property.member);
            
            if(i!=nbProperties-1) destination->write(string(", "));
        });

        destination->write(string("}"));

        return data;
    }    
*/
    template<typename T> T asAny(string);
  //  template<typename T> T asAny(wsocket::in*);

//    template<typename T> bool fromJson(T& object, wsocket::in *source);

    template<> int asAny<int>(string value);
    template<> string asAny<string>(string value);
    template<> float asAny<float>(string value);
    template<> long asAny<long>(string value); 
    template<> char asAny<char>(string value);

/*
    template<> std::vector<collide> asAny<std::vector<collide>>(string value);
    template<> std::vector<sentence> asAny<std::vector<sentence>>(string value);
    template<> std::vector<movement> asAny<std::vector<movement>>(string value);
    template<> std::vector<xyzw<int>> asAny<std::vector<xyzw<int>>>(string value);
    template<> std::vector<xyzw<char>> asAny<std::vector<xyzw<char>>>(string value);
    template<> std::vector<value> asAny<std::vector<value>>(string value);
    template<> std::vector<instruction> asAny<std::vector<instruction>>(string value);

    template<> xyzw<int> asAny<xyzw<int>>(string value);
    template<> xyzw<char> asAny<xyzw<char>>(string value);
    template<> header asAny<header>(string value);
    template<> movement asAny<movement>(string value);
    template<> collide asAny<collide>(string value);
    template<> sentence asAny<sentence>(string value);
    template<> value asAny<value>(string value);
    template<> instruction asAny<instruction>(string value);

    template<> std::vector<collide> asAny<std::vector<collide>>(wsocket::in *value);
    template<> std::vector<sentence> asAny<std::vector<sentence>>(wsocket::in *value);
    template<> std::vector<movement> asAny<std::vector<movement>>(wsocket::in *value);
    template<> std::vector<xyzw<int>> asAny<std::vector<xyzw<int>>>(wsocket::in *value);
    template<> std::vector<xyzw<char>> asAny<std::vector<xyzw<char>>>(wsocket::in *value);
    template<> std::vector<value> asAny<std::vector<value>>(wsocket::in *value);
    template<> std::vector<instruction> asAny<std::vector<instruction>>(wsocket::in *value);
*/
/*
    template<> xyzw<int> asAny<xyzw<int>>(wsocket::in *value);
    template<> xyzw<char> asAny<xyzw<char>>(wsocket::in *value);
    template<> header asAny<header>(wsocket::in *value);
    template<> movement asAny<movement>(wsocket::in *value);
    template<> collide asAny<collide>(wsocket::in *value);
    template<> sentence asAny<sentence>(wsocket::in *value);
    template<> value asAny<value>(wsocket::in *value);
    template<> instruction asAny<instruction>(wsocket::in *value);
*/
/*
    template<> int asAny<int>(wsocket::in *value);
    template<> string asAny<string>(wsocket::in *value);
    template<> float asAny<float>(wsocket::in *value);
    template<> long asAny<long>(wsocket::in *value);
    template<> char asAny<char>(wsocket::in *value);
*/
/*
    template<typename T> bool fromJson(T& object, wsocket::in *source)
    {
        bool left = true, quotes = false;
        char previous = ' ';
        char c = ' ';

        string label, value;

        if(source->read(c))
        {
            if(c  == '}') source->next();
        }

        do
        {
            if(source->read(c))
            {
                if (c == '"')
                {
                    if(previous != '\\') quotes = !quotes;
                }
                else
                {
                    if (!quotes)
                    {
                        if (c == '{')
                        {
                            constexpr auto nbProperties = std::tuple_size<decltype(T::properties)>::value;

                            for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i)
                            {
                                constexpr auto property = std::get<i>(T::properties);
                            
                                if(label == string(property.name))
                                {
                                    using Type = typename decltype(property)::Type;
                                    object.*(property.member) = asAny<Type>(source);
                                }
                            });

                            label.clear();
                            value.clear();

                            left = true;
                        }
                        else if (c == '}')
                        {
                            constexpr auto nbProperties = std::tuple_size<decltype(T::properties)>::value;
                
                            for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i)
                            {        
                                constexpr auto property = std::get<i>(T::properties);
                            
                                if(label == string(property.name))
                                {
                                    using Type = typename decltype(property)::Type;
                                    object.*(property.member) = asAny<Type>(value);                            
                                }
                            });

                            left = true;

                            label.clear();
                            value.clear();
                            
                            return true;
                        }                    
                        else if (c == '[')
                        {
                            constexpr auto nbProperties = std::tuple_size<decltype(T::properties)>::value;
                                
                            for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i)
                            {        
                                constexpr auto property = std::get<i>(T::properties);
                                                    
                                if(label == string(property.name))
                                {
                                    source->next();

                                    using Type = typename decltype(property)::Type;
                                    object.*(property.member) = asAny<Type>(source);
                                }
                            });

                            label.clear();
                            value.clear();

                            left = true;
                        }
                        else if (c == ']')
                        {
                            left = true;

                            label.clear();
                            value.clear();
                                                
                            return false;                    
                        }
                        else if (c == ':')
                        {
                            left = false;  
                        }
                        else if (c == ',')
                        {
                            constexpr auto nbProperties = std::tuple_size<decltype(T::properties)>::value;
                        
                            for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i)
                            {        
                                constexpr auto property = std::get<i>(T::properties);
                                
                                if(label == string(property.name))
                                {                                
                                    using Type = typename decltype(property)::Type;
                                    object.*(property.member) = asAny<Type>(value);                    
                                }
                            });
                                        
                            left = true;

                            label.clear();
                            value.clear();
                        }
                        else if (((c >= '0') && (c <= '9'))||(c=='.')||(c=='-'))
                        {
                            if (!left)
                            {
                                value += c;
                            }
                        }
                    }
                    else
                    {
                        if ((quotes && left))
                        {
                            label += c;
                        }
                        else if ((quotes && !left))
                        {
                            value += c;
                        }
                    }
                }      

                previous = c;      
            }
        } while(source->next());

        return true;
    }*/
};

#endif