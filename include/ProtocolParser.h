#ifndef _PROTOCOLPARSE_
#define _PROTOCOLPARSE_
#include "json.h"
#include "myhead.h"
using nlohmann::json;
enum
{
KeyRecommand,
WebPageSearch
};

class ProtocolParser
{
    public:
        static string Json2string(json & j);
        static json doParse(const string  & data);
        static json vector2json(vector<string> & s);
        static void json2vector(json & j, vector<string> & v);
};

#endif