#include "../../include/SearchEngineServer/Configuration.h"

Configuration::Configuration(const std::string &configFilePath)
{
    
    std::ifstream ifs(configFilePath);
    std::string first, second;
    if(!ifs)
    {
        std::cerr << configFilePath << " not exist" << std::endl;
        ::exit(1);
    }
    while(ifs >> first >> second)
    {
        _configs.insert({first, second});
    }
    ifs.close();
}

Configuration *Configuration::getInstance()
{
    pthread_once(&_once, init);
    return _pConfig;
}

void Configuration::init()
{
    _pConfig = new Configuration(_configFilePath);
    atexit(destory);
}

void Configuration::destory()
{
    if(_pConfig)
    {
        delete _pConfig;
        _pConfig = nullptr;
    }
}

std::map<std::string, std::string> &Configuration::getConfigMap()
{
    return _configs;
}

const std::string &Configuration::operator[](const std::string &key)
{
    return _configs.at(key);
}

unordered_set<string>& Configuration::getStopWordList()
{
    stopWordListInit();
    return _stopWordList;
}

void Configuration::stopWordListInit()
{
    string StopWordFilePath(_configs["stop"]);

    ifstream ifs(StopWordFilePath);
    if(!ifs.is_open())
    {
        cerr << "StopWordFilePath Open error !" << endl;
        return;
    }
    ifs.seekg(std::ios_base::beg);
    string stopWord;
    
    while(getline(ifs,stopWord))
    {
        if(stopWord.back() == '\r')
        {
            stopWord.pop_back();
        }
        _stopWordList.insert(stopWord);
    }
    ifs.close();
}
                                                                                                                                                                                                 
Configuration *Configuration::_pConfig = nullptr;
pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;
std::string Configuration::_configFilePath = "../conf/server.conf";
