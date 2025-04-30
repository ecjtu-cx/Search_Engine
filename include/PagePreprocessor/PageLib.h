#ifndef _PAGELIB_H
#define _PAGELIB_H
#include "DirScanner.h"
#include "../SearchEngineServer/Configuration.h"
#include "../../lib/simhash/Simhasher.hpp"

struct HammingDistance_3
{
    bool operator()(const uint64_t &lhs, const uint64_t &rhs) const
    {   
        return simhash::Simhasher::isEqual(lhs, rhs);
    }   
};

class PageLib
{
public:
    PageLib();
    void create();
    void store();

private:
    Configuration &_config;
    DirScanner _dirScanner;
    std::vector<std::string> _pages;
    simhash::Simhasher _simhasher;
    std::unordered_set<uint64_t, std::hash<uint64_t>, HammingDistance_3> _simhashset;
    std::map<int, std::pair<int, int>> _offsetLib;
};
#endif //_PAGELIB_H
