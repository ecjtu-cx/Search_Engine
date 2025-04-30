#ifndef _DICTPRODUCER_H
#define _DICTPRODUCER_H
#include "SplitTool.h"
#include "../myhead.h"
#include "../PagePreprocessor/PageLibPreprocessor.h"


class DictProducer
{
public:
    DictProducer();
    DictProducer(SplitTool *cuttor);
    void buildEnDict();
    void buildCnDict();
    void createIndex();
    void storeDict(const char *path);
    void storeIndex(const char *path);

private:
    vector<pair<string, int>> _dict;//词 词频->词典
    unordered_map<string,int> _dict_index;//词 词的下标
    map<string, set<int>> _index;//字 含有字的词的下标->词典索引
    SplitTool *_cuttor;
    map<string, string> &_config;
};

#endif //_DICTPRODUCER_H