#include "../../include/KeyRecommander/KeyRecommander.h"
#include "../../include/Cache/CacheManager.h"

extern __thread const char * name;

vector <string> read_utf8_onebyone(string input);
/*
KeyRecommander::KeyRecommander(const string & query, TcpConnectionPtr conn)
: _queryWord(query)
{}
*/

KeyRecommander::KeyRecommander(const string & word)
: _queryWord(word)
{
}

//void KeyRecommander::executeQuery()
//{}

void KeyRecommander::queryIndexTable()
{
    Dictionary* dicitionary = Dictionary::createInstance();//获取Dictionary
    set<int> line_no;//每个字符对应的行号
    vector<string> queryWord = read_utf8_onebyone(_queryWord);//将待查询的字符串每一个元素存入数组

    for(auto elem:queryWord)
    {
        if((dicitionary ->getIndexTable().find(elem)) != (dicitionary -> getIndexTable().end()))//索引中存在待查寻元素
        {
            set<int> temp_line_no = dicitionary -> getIndexTable().at(elem);//找出每一个元素对应的行号set集合
    
            line_no.insert(temp_line_no.begin(), temp_line_no.end());// 并将集合插入到集合line_no中
        }

    }
    
    int i = 0;//从0行开始遍历词典
    for(auto elem:dicitionary -> getDict())//遍历词典 把行号set集合中 存在的行号 所对应的行中的词组存入_similarWord
    {
        if(line_no.count(i))
        {
            _similarWord.push_back(make_pair(elem.first, elem.second));
        }
        i++;
    }
}

void KeyRecommander::statistic()
{
    
    struct CandidateResult myresult;
    for(auto elem: _similarWord)
    {
        bzero(&myresult, sizeof(myresult));//清空myresult结构体
        myresult._dist = editDistance(_queryWord, elem.first);//计算出最小编辑距离
        myresult._freq = elem.second;//获取词语对应的词频
        myresult._word = elem.first;//把词语存入
        _que.push(myresult); //把结构体放进优先队列
    }
}

void KeyRecommander::product_similar_finally()
{
    int i = 0;
    while(!_que.empty() && i++ < 3)
    {
        _similar_finally.push_back(_que.top()._word);
        //cout << "product_similar_finally" << endl;
        //cout << "i = "<< i << "word = "<< _que.top()._word << endl;
        _que.pop();
    }

}


json KeyRecommander::get_serialize()//将优先级最高的三个近似词以json类型返回
{  
    json j;
    if(CacheManager::getCachemanager()->getKeyWordCache(atoi(name)).get(_queryWord,j))
        return j;
    else{
        executeQuery();
        j = ProtocolParser::vector2json(_similar_finally);
        CacheManager::getCachemanager()->getKeyWordCache(atoi(name)).put(_queryWord,j);
        return j;
    }
    
}


void KeyRecommander::executeQuery()
{
    queryIndexTable();//先生成存放所有近似词的vector数组
    statistic();//再被每一个近似词对应的 MyResult 结构体存入优先队列
    product_similar_finally();

}

vector <string> read_utf8_onebyone(string input)
{
    vector <std::string> words;
    int len = input.length();
    int i = 0;

    while (i < len)
    {
        assert((input[i] & 0xF8) <= 0xF0);
        int next = 1;
        if((input[i] & 0x80) == 0x00)
        {}
        else if ((input[i] & 0xE0) == 0xC0)
        {
            next = 2;
        }
        else if ((input[i] & 0xF0) == 0xE0)
        {
            next = 3;
        }
        else if ((input[i] & 0xF8) == 0xF0)
        {
            next = 4;
        }
        words.push_back(input.substr(i, next));
        i += next;
    }
    return words;
}