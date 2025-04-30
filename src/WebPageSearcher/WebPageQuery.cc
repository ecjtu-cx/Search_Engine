#include "../../include/WebPageSearcher/WebPageQuery.h"
#include "../../include/WebPageSearcher/WebPage.h"
#include "../../include/SearchEngineServer/Configuration.h"
#include "../../include/SearchEngineServer/SplitTool.h"

WebPageQuery::WebPageQuery()
: _configs(*Configuration::getInstance())
, _wordSegmentation(Cppjieba::getSplitTool())
{
    loadLibrary();
    WebPage::ripepageIfs.open(_configs["ripepage"]); 
}

WebPageQuery::~WebPageQuery()
{
    if(WebPage::ripepageIfs)
    {
        WebPage::ripepageIfs.close();
    }
}

void WebPageQuery::doQuery(const string &sought, vector<string> &retval)
{
    /* vector<string> queryWords = {"山河", "岁月"}; */
    vector<string> queryWords;
    _wordSegmentation->cut(sought, queryWords);
    
    if(queryWords.empty())
    {
        retval.push_back(returnNoAnswer());
        return;
    }
                                                                
    vector<WebPage> queryResult;
    //按照密集度进行排序
    bool isExist = executeQuery(queryWords, queryResult);
    if(!isExist)//查询结果不存在
    {
        retval.push_back(returnNoAnswer());
        cout << retval[0] << endl;
        return;
    }
    else
    {
        //对结果进行排序
        sort(queryResult.begin(), queryResult.end(), [](const WebPage &a, const WebPage &b) 
                                                     -> bool { return a.sortWeight > b.sortWeight; }); 
        if(queryResult.size() > 20)
        {
            queryResult.resize(20);
        }
        retval.reserve(queryResult.size());
        for(auto &webpage : queryResult)
        {
            string tmp = webpage.docTitle + "\n" + webpage.docUrl + "\n" + webpage.docSummary + "\n";
            retval.push_back(std::move(tmp));
        }
    }
}

void WebPageQuery::loadLibrary()
{
    //读取网页偏移库
    std::ifstream ifsOffsetlib(_configs["offset"]);
    if(!ifsOffsetlib)
    {
        cout << "Error:open newoffset.dat failed!" << endl; 
        return;
    }
    string line;
    while(getline(ifsOffsetlib, line))
    {
        std::istringstream iss(line);
        int docid, docbeg, docend;
        iss >> docid >> docbeg >> docend;
        _offsetlib[docid] = std::make_pair(docbeg, docend);
    }
    ifsOffsetlib.close();

    //读取倒排索引库
    std::ifstream ifsInvertIndexlib(_configs["invertIndex"]);
    if(!ifsInvertIndexlib)
    {
        cout << "Error:open invertIdx.dat failed!" << endl; 
        return;
    }
    while(getline(ifsInvertIndexlib, line))
    {
        std::istringstream iss(line);
        string word;
        iss >> word;
        map<int, double> index;
        int docid;
        double weight; 
        while(iss >> docid >> weight)
        {
            index.insert({docid, weight});
        }
        _invertIndexLib[word] = index;
    }
    ifsInvertIndexlib.close();
    
}

vector<double> WebPageQuery::getQueryWordsWeightVector(const vector<string> & queryWords)
{
    vector<double> queryWordsWeight;
    queryWordsWeight.reserve(queryWords.size());
    for(auto &word : queryWords)
    {
        //TF-IDF算法,计算搜索词的权重
        double TF = 1;//某个词在文章中出现的次数,将搜索词看成一篇文档的话那么搜索词至少出现一次
        double DF = 1;//包含该词语的文档数量
        auto iter = _invertIndexLib.find(word);
        if(iter != _invertIndexLib.end())
        {
            DF += iter->second.size();
        }
        double N = _offsetlib.size() + 1;//文档总数
        double IDF = log2(N / (DF + 1));//逆文档频率
        double w = TF * IDF;//词语权重
        queryWordsWeight.push_back(w);
    }
    double sqrtSum = 0;
    for(auto &elem : queryWordsWeight)
    {
        sqrtSum += elem * elem;
    }
    double sqrtRoot = sqrt(sqrtSum);
    for(auto &elem : queryWordsWeight)
    {
        elem = elem / sqrtRoot;
    }

    return queryWordsWeight; 
}

bool WebPageQuery::executeQuery(const vector<string> & queryWords, vector<WebPage> &queryResult)
{
    if(1 == queryWords.size())//查询单个词
    {
        auto &querySet = _invertIndexLib[queryWords[0]];
        int queryResultSize = querySet.size() > 100 ? 100 : querySet.size();
        queryResult.reserve(queryResultSize);
        auto iter = querySet.begin();
        for(int idx = 0; idx < queryResultSize; ++idx, ++iter)
        {
            queryResult.push_back(WebPage(iter->first, _offsetlib, queryWords, iter->second));
        }
    }
    else//查询多个词
    {
        //查找包含单个查询词的网页
        vector<vector<int>> webPages(queryWords.size());
        for(size_t idx = 0; idx < queryWords.size(); ++idx)
        {
            auto iter = _invertIndexLib.find(queryWords[idx]);
            if(iter == _invertIndexLib.end())
            {
                return false;//有一个词不存在就返回false
            }
            else
            {
                for(auto &web : iter->second)
                {
                    webPages[idx].push_back(web.first);
                }
            }
        }
        //求交集——包含所有单词的网页
        for(size_t idx = 1; idx < queryWords.size(); ++idx)
        {
            auto iter = std::set_intersection(webPages[0].begin(), webPages[0].end(), 
                                  webPages[idx].begin(), webPages[idx].end(), webPages[0].begin());
            if(iter == webPages[0].begin())
            {
                return false;//不存在交集
            }
            else
            {
                webPages[0].resize(iter - webPages[0].begin());
            }
        }
        /* cout << "webPages[0].size() = " << webPages[0].size() << endl; */
        int queryResultSize = webPages[0].size() > 100 ? 100 : webPages[0].size();
        queryResult.reserve(queryResultSize);
        for(size_t idx = 0; idx < queryResultSize; ++idx)
        {
            queryResult.push_back(WebPage(webPages[0][idx], _offsetlib, queryWords));
        }
    }

    return true;
}

string WebPageQuery::returnNoAnswer()
{
    string noAnswer = "404 not found";

    return noAnswer;
}

#if 0
bool WebPageQuery::executeQueryByCosinesim(const vector<string> & queryWords, vector<WebPage> &queryResult)
{
    //查找包含查询词的网页
    vector<map<int, double>> webPages;
    webPages.reserve(queryWords.size());
    for(auto &word : queryWords)
    {
        auto iter = _invertIndexLib.find(word);
        if(iter == _invertIndexLib.end())
        {
            return false;//有一个词不存在就返回false
        }
        else
        {
            webPages.push_back(iter->second);            
        }
    }
    //求交集——包含所有单词的网页
    vector<pair<int, double>> intersection;
    for(auto &elem : webPages[0])
    {
        intersection.push_back(elem);
    }
    for(int idx = 1; idx < webPages.size(); ++idx)
    {
        auto iter = std::set_intersection(intersection.begin(), intersection.end(), 
                              webPages[idx].begin(), webPages[idx].end(), intersection.begin(),
                              [](pair<int, double> a, pair<int, double> b) -> bool
                              { return a.first < b.first; });
        intersection.resize(iter - intersection.begin());
        if(intersection.empty())
        {
            return false;//不存在交集
        }
    }
#if 1
    //将包含每个单词的网页利用余弦相似度算法进行排序
    vector<pair<int, double>> queryWebPages;//保存符合查询结果的网页ID和余弦值
    vector<double> base = getQueryWordsWeightVector(queryWords);//获取查询词权重Base
    for(auto &elem1 : intersection)//elem1 = pair<docId, weight>
    {
        int docId = elem1.first;
        vector<double> weights;
        weights.reserve(queryWords.size());
        for(auto &elem2 : webPages)//elem2 = set<pair<docId, weights>>
        {
            auto iter = elem2.begin();
            while(iter != elem2.end() && iter->first != docId)
            {
                ++iter;
            }
            weights.push_back(iter->second);
        }
        //计算每个网页的余弦相似度
        double X = 0, Y = 0;
        double X_x_Y = 0;
        for(int idx = 0; idx < queryWords.size(); ++idx)
        {
            X += base[idx] * base[idx];
            Y += weights[idx] * weights[idx];
            X_x_Y += weights[idx] * base[idx];
        }
        X = sqrt(X);
        Y = sqrt(Y);
        double cosinSim = X_x_Y / (X * Y);
        queryWebPages.push_back(std::make_pair(docId, cosinSim));
    }
    //根据余弦相似度从大到小排序
    std::sort(queryWebPages.begin(), queryWebPages.end(), 
              [](const pair<int, double> &a, const pair<int, double> &b)
              ->bool {return a.second > b.second;});
#endif

    return true;
}
#endif
