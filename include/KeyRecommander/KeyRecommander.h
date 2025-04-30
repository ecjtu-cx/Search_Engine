#ifndef _KEYRECOMMANDER_H
#define _KEYRECOMMANDER_H
#include "../myhead.h"
#include "../TcpServer/TcpConnection.h"
#include "../Cache/CandidateResult.h"
#include "../ProtocolParser.h"
#include "Dictionary.h"
#include "EditDistance.h"
#include "../json.h"
#include <assert.h>
#include <string.h>
#include <queue>

using json = nlohmann::json;


class ResultCompare
{
public:
    bool operator()(const CandidateResult & lhs, const CandidateResult & rhs)
    {
        if(lhs._dist == rhs._dist)
        {
            if(lhs._freq == rhs._freq)
            {
                if(lhs._word < rhs._word)//a的优先级高于b
                {
                    return false;

                }
                else
                {
                    return true;
                }
            }
            else
            {
                if(lhs._freq < rhs._freq)//a的优先级高于b
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            if(lhs._dist < rhs._dist)//a的优先级高于b
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        //return lhs._dist < rhs._dist;
    }
};

class KeyRecommander {
public: 
    //KeyRecommander(const string & query, TcpConnectionPtr conn);
    KeyRecommander(const string& word);
    void executeQuery();//开始执行查询
    void queryIndexTable();//从索引文件找出查询词的近似词
    void statistic(/*set<int>& iset */);//进行计算(最小编辑距离，词频，候选词) 结果放入优先队列
    //int distance(const string & rhs);
    void product_similar_finally();//产生优先级最高的三个近似词
    json get_serialize();
    //int editDistance(const std::string & lhs, const std::string &rhs);
    //void reponse();
private:
    string _queryWord;//等查询的单词
    priority_queue<CandidateResult,
    vector<CandidateResult>, ResultCompare> _que;//保存比较的结果
    //vector<string> _similarWord;//存放近似词
    vector<pair<string,int>> _similarWord;
    vector<string> _similar_finally;//存放优先级最高的三个近似词
    TcpConnectionPtr _conn;
};

#endif //_KEYRECOMMANDER_H
