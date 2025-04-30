#include "../../include/SearchEngineServer/SplitTool.h"
#include "../../include/SearchEngineServer/Configuration.h"


Cppjieba::Cppjieba()
    : _configs(Configuration::getInstance()->getConfigMap())
    , _jieba(_configs["dict_path"],_configs["hmm_path"],_configs["user_dict_path"],_configs["idf_path"], _configs["stop_words_path"])
{}

SplitTool *Cppjieba::getSplitTool()
{
    if (splittool == nullptr)
    {
        pthread_mutex_lock(&_mutex);
        if (splittool == nullptr)
        {
            splittool = new Cppjieba();
        }
        pthread_mutex_unlock(&_mutex);
    }
    return splittool;
}

vector<string> Cppjieba::cut(const string &sentence, vector<string> &all_words)
{
    _jieba.CutForSearch(sentence, all_words);
    Configuration *config1 = Configuration::getInstance();
    unordered_set<string> stop_words = config1->getStopWordList();
    vector<string> words;
    for (auto &it : all_words)
    // auto it = find(all_words.begin(),all_words.end(),it)
    {
        if (stop_words.find(it) != stop_words.end())
        {
            continue;
            // cout << "stop" << endl;
            // cout << it << endl;
        }
        else
        {
            words.push_back(it);
            // cout << it << endl;
            // cout << stop_words.size() << endl;
        }
    }
    all_words.clear();
    for (auto &it : words)
    {
        unsigned char c = it[0];
        if (c < 128)
        {
            continue;
        }
        else
        {
            all_words.push_back(it);
            // cout << it << endl;
        }
    }
    return all_words;
}

void Cppjieba::destroy()
{
    if (splittool)
    {
        delete splittool;
    }
}

SplitTool *Cppjieba::splittool = nullptr;
pthread_mutex_t Cppjieba::_mutex;

#if 0
//使用方法
int main()
{
    chdir("/home/lee/project/search_-engine/bin");
    // string sentence = "美国作家菲茨杰拉德";
    string sentence;
    //char str[100];
    //while(scanf("%s", str) != EOF)
    while (cin >> sentence)
    {
        //string sentence(str);
        vector<string> all_words;
        SplitTool *cppjieba = Cppjieba::getSplitTool();
        cppjieba->cut(sentence, all_words);
        for (auto &elem : all_words)
        {
            cout << elem << endl;
        }
        return 1;
    }
}
#endif
