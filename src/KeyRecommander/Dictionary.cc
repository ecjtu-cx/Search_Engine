#include "../../include/KeyRecommander/Dictionary.h"

Dictionary * Dictionary::_pdict = nullptr;
pthread_mutex_t Dictionary::_dmutex;

Dictionary::Dictionary()
{
    map<string, string> configs = Configuration::getInstance()->getConfigMap();
    init(configs["xmldict_path"], configs["index_path"]);
    // init("../../data/dict/dict.dat", "../../data/dict/dictindex.dat");
}

Dictionary * Dictionary::createInstance()
{
    if(nullptr == _pdict)
    {
        pthread_mutex_lock(&_dmutex);
        if(nullptr == _pdict)
        {
            _pdict = new Dictionary();
        }
        pthread_mutex_unlock(&_dmutex);
    }
    return _pdict;
}

void Dictionary::init(const string & dictpath, const string & indexpath)
{
    ifstream ifile(dictpath);
    string line;
    string word;
    int num;
    while(getline(ifile, line), !ifile.eof())
    {
        istringstream iss(line);
        iss >> word;
        iss >> num;
        _dict.push_back(make_pair(word, num));
    }
    ifile.close();
    ifile.open(indexpath);
    while(getline(ifile, line), !ifile.eof())
    {
        istringstream iss(line);
        iss >> word;
        while(iss >> num)
        {
            _index[word].insert(num);
        }
    }
    ifile.close();
}

vector<pair<string, int>>& Dictionary::getDict()
{
    return _dict;
}

map<string, set<int>> & Dictionary::getIndexTable()
{
    return _index;
}