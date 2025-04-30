#ifndef __WEBPAGEQUERY_H__
#define __WEBPAGEQUERY_H__

#include "../myhead.h"
#include "WebPage.h"

class SplitTool;

class WebPageQuery 
{
public: 
    WebPageQuery();
    ~WebPageQuery();
    void doQuery(const string &sought, vector<string> &retval);

private:
    void loadLibrary();
    vector<double> getQueryWordsWeightVector(const vector<string> & queryWords);
    bool executeQuery(const vector<string> & queryWords, vector<WebPage> &queryResult);
    string returnNoAnswer();

private:
    unordered_map<int, pair<int, int>> _offsetlib;//偏移库
    unordered_map<string, map<int, double>> _invertIndexLib;//倒排索引库
    Configuration  & _configs;
    SplitTool *_wordSegmentation;
};

#endif //_WEBPAGEQUERY_H
