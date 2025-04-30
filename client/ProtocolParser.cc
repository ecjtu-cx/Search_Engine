#include "ProtocolParser.h"

string ProtocolParser::Json2string(json & j)
{
    return j.dump();
}

json ProtocolParser::doParse(const string & data)
{
    return json::parse(data);
}

json ProtocolParser::vector2json(vector<string> & s)
{
    return json(s);
}

void ProtocolParser::json2vector(json & j, vector<string> & v)
{
    v.clear();
    v = j.get<vector<string>>();
}

#if 0
使用方法
j["pi"] = 3.141;

// add a Boolean that is stored as bool
j["happy"] = true;

// add a string that is stored as std::string
j["name"] = "Niels";

// add another null object by passing nullptr
j["nothing"] = nullptr;

// add an object inside the object
j["answer"]["everything"] = 42;
#endif