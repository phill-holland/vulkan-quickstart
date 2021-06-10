#include "json.h"
//#include "crumbs.h"
//#include "charbuf.h"
#include <vector>

bool json::parser::json::parse(string json)
{
	/*
	bool left = true, quotes = false;
	long brackets = 0L, squares = 0L;

	global::charbuf label, value;
	crumbs::crumbs parents;

	for (long i = 0L; i < json.count(); ++i)
	{
		if (json[i] == '"')
		{
			quotes = !quotes;
		}
		else
		{
			if (!quotes)
			{
				if (json[i] == '{')
				{
					if (!parents.push((string)label))
					{
						return false;
					}

					label.clear();
					value.clear();

					left = true;

					++brackets;
				}
				else if (json[i] == '}')
				{
					if (!parents.isempty())
					{
						::json::parser::json *current = find(parents.FQDN());
						if (current != NULL)
						{
							custom::pair tp(label, value);
							current->add(tp);
							//if (!current->terminate()) return false;
						}
					}

					left = true;

					label.clear();
					value.clear();

					if (!parents.pop())
					{
						return false;
					}

					// ***
					if (!parents.isempty())
					{
						::json::parser::json *current = find(parents.FQDN());
						if (current != NULL)
						{
							if (!current->terminate()) return false;
						}
					}
					// ***

					--brackets;
					if (brackets < 0)
					{
						return false;
					}
				}
				else if (json[i] == '[')
				{
					if (!parents.push(label))
					{
						return false;
					}

					label.clear();
					value.clear();

					left = true;
					++squares;
				}
				else if (json[i] == ']')
				{
					left = true;

					label.clear();
					value.clear();

					if (!parents.pop())
					{
						return false;
					}

					--squares;
					if (squares < 0)
					{
						return false;
					}
				}
				else if (json[i] == ':')
				{
					left = false;
				}
				else if (json[i] == ',')
				{
					if (!parents.isempty())
					{
						::json::parser::json *current = find(parents.FQDN());
						if (current != NULL)
						{
							custom::pair tp(label, value);
							current->add(tp);
						}
					}

					left = true;

					label.clear();
					value.clear();
				}
				else if ((json[i] >= '0') && (json[i] <= '9'))
				{
					if (!left)
					{
						if (!value.push(json[i]))
						{
							return false;
						}
					}
				}
			}
			else
			{
				if (brackets >= 1)
				{
					if ((quotes && left))
					{
						if (!label.push(json[i]))
						{
							return false;
						}
					}
					else if ((quotes && !left))
					{
						if (!value.push(json[i]))
						{
							return false;
						}
					}
				}
			}
		}
	}
*/
	return true;
}

template<> int json::asAny<int>(string value) 
{
	return value.toInteger();
}

template<> string json::asAny<string>(string value)
{
   string temp("\\\"");
   return value.replace('"', temp);
}

template<> float json::asAny<float>(string value)    
{
	return value.toFloat();            
}

template<> long json::asAny<long>(string value)    
{
	return value.toLong();    
}

template<> char json::asAny<char>(string value)    
{
	return (char)value.toInteger();    
}

/*
template<> std::vector<collide> json::asAny<std::vector<collide>>(string value) { throw; }
template<> std::vector<sentence> json::asAny<std::vector<sentence>>(string value) { throw; }
template<> std::vector<movement> json::asAny<std::vector<movement>>(string value) { throw; }
template<> std::vector<xyzw<int>> json::asAny<std::vector<xyzw<int>>>(string value) { throw; }
template<> std::vector<xyzw<char>> json::asAny<std::vector<xyzw<char>>>(string value) { throw; }
template<> std::vector<value> json::asAny<std::vector<value>>(string value) { throw; }
template<> std::vector<instruction> json::asAny<std::vector<instruction>>(string value) { throw; }

template<> xyzw<int> json::asAny<xyzw<int>>(string value) { throw; }
template<> xyzw<char> json::asAny<xyzw<char>>(string value) { throw; }
template<> header json::asAny<header>(string value) { throw; }
template<> movement json::asAny<movement>(string value) { throw; }
template<> collide json::asAny<collide>(string value) { throw; }
template<> sentence json::asAny<sentence>(string value)  { throw; }
template<> value json::asAny<value>(string value) { throw; }
template<> instruction json::asAny<instruction>(string value) { throw; }

template<> std::vector<collide> json::asAny<std::vector<collide>>(wsocket::in *value)
{
	std::vector<collide> result;
	bool r = false;

	do
	{
		collide temp;
		r = fromJson(temp, value);
		if(r == true) result.push_back(temp);
	}while(r == true);

	return result;
}

template<> std::vector<sentence> json::asAny<std::vector<sentence>>(wsocket::in *value)
{
	std::vector<sentence> result;
	bool r = false;

	do
	{
		sentence temp;
		r = fromJson(temp, value);
		if(r == true) result.push_back(temp);
	}while(r == true);

	return result;
}

template<> std::vector<movement> json::asAny<std::vector<movement>>(wsocket::in *value)
{
	std::vector<movement> result;
	bool r = false;
	do
	{
		movement temp;
		r = fromJson(temp, value);
		if(r == true) result.push_back(temp);
	}while(r == true);

	return result;
}

template<> std::vector<xyzw<int>> json::asAny<std::vector<xyzw<int>>>(wsocket::in *value)
{
	std::vector<xyzw<int>> result;
	bool r = false;

	do
	{
		xyzw<int> temp;
		r = fromJson(temp, value);
		if(r == true) result.push_back(temp);
	}while(r == true);

	return result;
}

template<> std::vector<xyzw<char>> json::asAny<std::vector<xyzw<char>>>(wsocket::in *value)
{
	std::vector<xyzw<char>> result;
	bool r = false;

	do
	{
		xyzw<char> temp;
		r = fromJson(temp, value);
		if(r == true) result.push_back(temp);
	}while(r == true);

	return result;
}

template<> std::vector<value> json::asAny<std::vector<value>>(wsocket::in *value)
{
	std::vector<jtypes::value> result;
	bool r = false;

	do
	{
		jtypes::value temp;
		r = fromJson(temp, value);
		if(r == true) result.push_back(temp);
	}while(r == true);

	return result;
}

template<> std::vector<instruction> json::asAny<std::vector<instruction>>(wsocket::in *value)
{
	std::vector<instruction> result;
	bool r = false;

	do
	{
		instruction temp;
		r = fromJson(temp, value);
		if(r == true) result.push_back(temp);
	}while(r == true);

	return result;
}

template<> xyzw<int> json::asAny<xyzw<int>>(wsocket::in *value)
{
	xyzw<int> result;
	fromJson(result, value);
	return result;
}

template<> xyzw<char> json::asAny<xyzw<char>>(wsocket::in *value)
{
	xyzw<char> result;
	fromJson(result, value);
	return result;
}

template<> header json::asAny<header>(wsocket::in *value)
{
	header result;
	fromJson(result, value);
	return result;
}

template<> movement json::asAny<movement>(wsocket::in *value)
{
	movement result;
	fromJson(result, value);
	return result;
}

template<> collide json::asAny<collide>(wsocket::in *value)
{
	collide result;
	fromJson(result, value);
	return result;
}

template<> sentence json::asAny<sentence>(wsocket::in *value)
{
	sentence result;
	fromJson(result, value);
	return result;
}

template<> value json::asAny<value>(wsocket::in *value)
{
	jtypes::value result;
	fromJson(result, value);
	return result;
}

template<> instruction json::asAny<instruction>(wsocket::in *value)
{
	instruction result;
	fromJson(result, value);
	return result;
}

template<> int json::asAny<int>(wsocket::in *value) { throw; }
template<> string json::asAny<string>(wsocket::in *value) { throw; }
template<> float json::asAny<float>(wsocket::in *value) { throw; }
template<> long json::asAny<long>(wsocket::in *value) { throw; }
template<> char json::asAny<char>(wsocket::in *value) { throw; }
*/