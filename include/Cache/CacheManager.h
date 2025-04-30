#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H
#include "../myhead.h"
#include "LRUCache.h"
#include "../SearchEngineServer/Configuration.h"
#include <pthread.h>
#include <string.h>

class CacheManager 
{
public: 
    static CacheManager* getCachemanager();
    static void init();
    static void destroy();
    void initCache(size_t threadNum, string filename1, string filename2);
        
    LRUCache& getKeyWordCache(int);      //从磁盘文件中读取缓存信息
    LRUCache& getWebPageCache(int);      //从磁盘文件中读取缓存信息

    void periodicUpdateCaches();  //定时更新所有的缓存
private:
    CacheManager();
    ~CacheManager(){}
    static CacheManager * _cmanager;
    static pthread_once_t _once;
    vector<LRUCache> _keyWordCaches;
    vector<LRUCache> _webPageCaches;

};

#endif //_CACHEMANAGER_H