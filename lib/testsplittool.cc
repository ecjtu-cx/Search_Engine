#include "cppjieba/include/cppjieba/Jieba.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
using std::string;
using std::endl;
using std::cout;
using std::vector;
const char *const dict_path = "../lib/cppjieba/dict/jieba.dict.utf8";
const char *const hmm_path = "../lib/cppjieba/dict/hmm_model.utf8";
const char *const user_dict_path = "../lib/cppjieba/dict/user.dict.utf8";
const char *const idf_path = "../lib/cppjieba/dict/idf.utf8";
const char *const stop_words_path = "../lib/cppjieba/dict/stop_words.utf8";

class SplitTool
{
public:
    virtual void cut(const string &sentence, vector<string> &vec) = 0;
};

class Cppjieba
: public SplitTool
{
public:
    static SplitTool *getSplitTool();
    void cut(const string &sentence, vector<string> &vec);
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
    cppjieba::Jieba _jieba;
    static SplitTool *splittool;
    static pthread_mutex_t _mutex;
};

SplitTool *Cppjieba::splittool = nullptr;
pthread_mutex_t Cppjieba::_mutex;

Cppjieba::Cppjieba()
    : _jieba(dict_path, hmm_path, user_dict_path, idf_path, stop_words_path)
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

void Cppjieba::cut(const string & sentence, vector<string>& words)
{
    _jieba.CutForSearch(sentence, words);
}

void Cppjieba::destroy()
{
    if (splittool)
    {
        delete splittool;
    }
}

int main()
{
    string sentence = "小明喜欢吃冰激凌";
    vector<string> vec;
    SplitTool * cppjieba = Cppjieba::getSplitTool();
    cppjieba->cut(sentence, vec);
    for(auto iter : vec)
    {
        cout << iter << endl;
    }
    return 1;
}