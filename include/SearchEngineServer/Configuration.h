#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_
#include "../myhead.h"

//auto &config = *Configuration::getInstance()
//config["server_ip"] = 127.0.0.1
//配置文件格式：server_ip 127.0.0.1
//可随意添加


class Configuration
{
    Configuration(const Configuration &) = delete;
    Configuration(Configuration &&) = delete;
    Configuration &operator=(const Configuration &) = delete;
    Configuration &operator=(Configuration &&) = delete;

public:
    static Configuration *getInstance();
    std::map<std::string, std::string> &getConfigMap();
    unordered_set<string> &getStopWordList();//获取停用词集合内容
    static void init();
    static void destory();
    const std::string &operator[](const std::string &key);

private:
    Configuration(const std::string &configFilePath);
    ~Configuration() = default;
    void stopWordListInit();

private:
    static Configuration *_pConfig;
    static pthread_once_t _once;
    static std::string _configFilePath;
    std::map<std::string, std::string> _configs;
    unordered_set<string> _stopWordList;
};
#endif
