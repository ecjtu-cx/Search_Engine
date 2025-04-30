#include "../../include/Cache/CacheTimerTask.h"
#include "../../include/Cache/CacheManager.h"

void CacheTimerTask::process() //缓存更新
{
    CacheManager *cm = CacheManager::getCachemanager();
    cm->periodicUpdateCaches();
}
