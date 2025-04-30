#ifndef _DICTIONARY_H
#define _DICTIONARY_H
#include "../myhead.h"
#include "../../include/SearchEngineServer/Configuration.h"

class Dictionary {
public: 
    static Dictionary * createInstance();
    void init(const string & dictpath,const string & indexpath);
    vector<pair<string, int>>& getDict();
    map<string, set<int>> & getIndexTable();
    
private:
    class AutoRelease
    {
    public:
        ~AutoRelease()
        {
            if(_pdict)
                delete _pdict;
        }
    };
    Dictionary();
    ~Dictionary(){}
    vector<pair<string,int>> _dict;
    map<string, set<int>> _index;
    static AutoRelease _ar;
    static Dictionary * _pdict;
    static pthread_mutex_t _dmutex;
};

//Dictionary * Dictionary::_pdict = nullptr;
//pthread_mutex_t Dictionary::_dmutex;

#endif //_DICTIONARY_H