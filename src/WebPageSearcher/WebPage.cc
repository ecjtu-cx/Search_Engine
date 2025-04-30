#include "../../include/WebPageSearcher/WebPage.h"

std::ifstream WebPage::ripepageIfs;

WebPage::WebPage(int docid, const std::string &doctitle, const std::string &docurl, const std::string &doccontent)
    : docId(docid)
    , docTitle(doctitle)
    , docUrl(docurl)
      , docContent(doccontent)
{}

WebPage::WebPage(int docid, const std::unordered_map<int, std::pair<int, int>> &offsetLib, const std::vector<std::string> &queryWords, double weight)
    : docId(docid)
{
    ripepageIfs.seekg(offsetLib.at(docid).first);
    int size = offsetLib.at(docid).second;
    char origDoc[size + 1];
    ::bzero(origDoc, sizeof(origDoc));
    ripepageIfs.read(origDoc, size);
    std::string doc(origDoc);
    
    size_t pos1 = doc.find("<url>") + 5,
    pos2 = doc.find("</url>", pos1),
    pos3 = doc.find("<title>", pos2 + 6) + 7,
    pos4 = doc.find("</title>", pos3),
    pos5 = doc.find("<content>", pos4 + 8) + 9;
    docUrl = doc.substr(pos1, pos2 - pos1);
    docTitle = doc.substr(pos3, pos4 - pos3);
    docContent = doc.substr(pos5, doc.find("</content>", pos5) - pos5);
    if(docContent.empty()) docContent = "---视频资料---";
    /* std::regex reg("\\s*<doc>\\s*<docid>(\\d*)</docid>\\s*<url>([\\s\\S]*?)</url>\\s*<title>([\\s\\S]*?)</title>\\s*<content>([\\s\\S]*)</content>\\s*</doc>\\s*"); */
    /* std::smatch pure; */
    /* std::regex_match(doc, pure, reg); */
    /* docUrl = pure.str(2); */
    /* docTitle = pure.str(3); */
    /* docContent = pure[4].length() ? pure.str(4) : "---视频资料---"; */
    sortWeight = weight;
   
    createSummary(queryWords);
}

void WebPage::createSummary(const std::vector<std::string> &queryWords)
{
    using pos_t = std::string::size_type;
    pos_t begPos = 0;
    if(1 == queryWords.size())//只有一个词
    {
        pos_t pos = docContent.find(queryWords[0]);
        if(pos > 0)//查找成功
        {
            begPos = pos;
        }
    }
    else//有多个词
    {
        //记录每个词出现的坐标
        map<pos_t, string> queryWordsPos; 
        for(auto &word : queryWords)
        {
            pos_t pos = 0;
            pos_t offset = 0;
            while(offset < docContent.length() && (-1 != (pos = docContent.find(word, offset))))
            {
                queryWordsPos.insert({pos, word});
                offset = pos + 1;
            }
        }
        //求包含所有词的最小区间起始地址
        map<string, int> wordsMap;//记录区间内每个词的出现次数
        for(auto &word : queryWords)
        {
            wordsMap[word] = 0;
        }
        auto beg = queryWordsPos.begin();
        auto end = beg;
        if(end != queryWordsPos.end())
        {
            ++wordsMap[end->second];
        }
        begPos = beg->first;
        pos_t minLength = docContent.length();
        while(beg != queryWordsPos.end())
        {
            /* if(docId == 184) */
            /* { */
            /*     cout << "beg = " << beg->first << " " << beg->second << endl; */
            /*     cout << "end = " << end->first << " " << end->second << endl; */
            /*     cout << "begPos = " << begPos << endl; */
            /*     cout << "minLength = " << minLength << endl << endl; */
            /* } */
            bool isAllAppear = true;
            //查看是否所有单词都被包含在区间内
            for(auto &word : queryWords)
            {
                if(wordsMap[word] <= 0)
                {
                    isAllAppear = false;
                    break;
                }
            }
            if(isAllAppear)
            {
                pos_t curLength = end->first - beg->first;
                if(curLength < minLength)
                {
                    begPos = beg->first;
                    minLength = curLength;
                }
                --wordsMap[beg->second];
                ++beg;
            }
            else
            {
                ++end;
                if(end != queryWordsPos.end())
                {
                    ++wordsMap[end->second];
                }
                else
                {
                    break;
                }
            }
        }
        /* cout << "docId = " << docId << ", begPos = " << begPos << ", minLength = " << minLength << endl; */
        if(0 == minLength)
        {
            sortWeight = 1.0;
        }
        else
        {
            sortWeight = 1.0 / minLength; 
        }
    }
    //截取简介内容
    if(begPos >= 0)
    {
        pos_t utf8Size = strlen("中");
        int newBeg = begPos - 5 * utf8Size;//向前取5个字符 
        if(newBeg <= 0)
        {
            newBeg = 0;
        }
        //判断字符是否完整
        char ch = docContent[newBeg];
        while(ch & 0x80 && !(ch & 0x40))//utf8编码1110xxxx 10xxxxxx 10xxxxxx
        {
            ch = docContent[++newBeg];
        }

        int newEnd = begPos + 80 * utf8Size;
        if(newEnd >= docContent.size())
        {
            newEnd = docContent.size();
        }
        else
        {
            //判断字符是否完整
            ch = docContent[newEnd];
            while(ch & 0x80 && !(ch & 0x40))//utf8编码1110xxxx 10xxxxxx 10xxxxxx
            {
                ch = docContent[--newEnd];
            }
        }
        /* cout << docContent.size() << " " << newBeg << " " << newEnd << endl; */
        docSummary = string(docContent, newBeg, newEnd - newBeg);
        docSummary += "...";
    }
    //如果关键词在标题出现，权重加上0.15
    set<string> titleAppearQueryWords; 
    for(auto &word : queryWords)
    {
        pos_t pos = 0;
        pos_t offset = 0;
        if(offset < docTitle.length() && (-1 != (pos = docTitle.find(word, offset))))
        {
            titleAppearQueryWords.insert(word);
            offset = pos + 1;
        }
    }
    double titleAppearWeight = 0;
    if(queryWords.size() == titleAppearQueryWords.size())
    {
        titleAppearWeight = 0.16;
    }
    else
    {
        titleAppearWeight = 0.1 * titleAppearQueryWords.size() / queryWords.size();
    }
    sortWeight += titleAppearWeight;
}
