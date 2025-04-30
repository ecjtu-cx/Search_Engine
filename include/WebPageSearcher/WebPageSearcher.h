#ifndef __WEBPAGESEARCHER_H__
#define __WEBPAGESEARCHER_H__

#include "WebPageQuery.h"
#include "../ProtocolParser.h"

extern __thread const char * name;

class WebPageSearcher {
public:
    WebPageSearcher();
    ~WebPageSearcher() {}
    void doQuery(const string &sought, json &retjson);

private:
    WebPageQuery _webQuery;
};

#endif //_WEBPAGESEARCHER_H
