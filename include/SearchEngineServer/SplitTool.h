#ifndef _SPLITTOOL_H_
#define _SPLITTOOL_H_
#include "../../lib/cppjieba/include/cppjieba/Jieba.hpp"
#include "../myhead.h"

class SplitTool
{
public:
    virtual vector<string> cut(const string &sentence, vector<string> &vec) = 0;
    virtual ~SplitTool(){}
};

class Cppjieba
: public SplitTool
{
public:
    static SplitTool *getSplitTool();
    vector<string> cut(const string &sentence, vector<string> &vec);
    static void destroy();
private:
    class AutoRelease
    {
    public:
        ~AutoRelease()
        {
            if(splittool)
                delete splittool;
        }  
    };
    Cppjieba();
    ~Cppjieba(){} 
    map<string, string> &_configs;
    cppjieba::Jieba _jieba;
    static SplitTool *splittool;
    static pthread_mutex_t _mutex;
};



#endif
