#include "../../include/WebPageSearcher/WebPageSearcher.h"
#include "../../include/WebPageSearcher/WebPageQuery.h"
#include "../../include/Cache/CacheManager.h"

WebPageSearcher::WebPageSearcher()
: _webQuery()
{

}

void WebPageSearcher::doQuery(const string &sought, json &retjson)
{
    if(CacheManager::getCachemanager()->getWebPageCache(atoi(name)).get(sought, retjson))
        return;
    vector<string> retval;
    _webQuery.doQuery(sought, retval);
    
    retjson = ProtocolParser::vector2json(retval);//打包成json格式
    CacheManager::getCachemanager()->getWebPageCache(atoi(name)).put(sought, retjson);

    
}
