#include "../../include/PagePreprocessor/PageLibPreprocessor.h"
#include "../../include/SearchEngineServer/Configuration.h"
#include "../../include/DictProducer/SplitTool.h"

PageLibPreprocessor::PageLibPreprocessor()
    : _wordCutter(Cppjieba::getSplitTool())
{
    auto &config = *Configuration::getInstance();
    std::ifstream ifs(config["offset"]);
    int id, start, size;
    while(ifs >> id >> start >> size)
    {
        _offsetLib.emplace(id, std::pair<int, int>(start, size));
    }
    ifs.close();
    ifs.open(config["ripepage"]);
    /* std::regex reg("\\s*<doc>\\s*<docid>(\\d*)</docid>\\s*<url>([\\s\\S]*)</url>\\s*<title>([\\s\\S]*)</title>\\s*<content>([\\s\\S]*)</content>\\s*</doc>\\s*"); */
    for(auto &offset : _offsetLib)
    {
        ifs.seekg(offset.second.first);
        int size = offset.second.second;
        char origDoc[size + 1]; 
        ::bzero(origDoc, sizeof(origDoc));
        ifs.read(origDoc, size);
        std::string doc(origDoc);

        size_t pos1 = doc.find("<docid>") + 7,
               pos2 = doc.find("</docid>", pos1),
               pos3 = doc.find("<url>", pos2 + 8) + 5,
               pos4 = doc.find("</url>", pos3),
               pos5 = doc.find("<title>", pos4 + 6) + 7,
               pos6 = doc.find("</title>", pos5),
               pos7 = doc.find("<content>", pos6 + 8) + 9;
        _pageList.emplace_back(std::stoi(doc.substr(pos1, pos2 - pos1)), doc.substr(pos5, pos6 - pos5), doc.substr(pos3, pos4 - pos3), doc.substr(pos7, doc.find("</content>") - pos7));
        /* std::smatch pure; */
        /* std::regex_match(doc, pure, reg); */
        /* _pageList.emplace_back(atoi(pure[1].str().c_str()), pure[3].str(), pure[2].str(), pure[4].str()); */
    }
    ifs.close();
}

void PageLibPreprocessor::bulidInvertIndexMap()
{
    std::map<std::string, int> DFmap;
    for(int idx = 0; idx < _pageList.size(); ++idx)
    {
        std::vector<std::string> splictWords;
        _wordCutter->cut(_pageList[idx].docTitle + _pageList[idx].docContent, splictWords);
        for(auto &word : splictWords)
        {
            if(!_pageList[idx].wordsMap[word]++)
                ++DFmap[word];
        }
    }
    std::unordered_map<int, double> dw;
    for(int idx = 0; idx < _pageList.size(); ++idx)
    {
        int id = _pageList[idx].docId;
        for(auto &aPair : _pageList[idx].wordsMap)
        {
            double w = aPair.second * log2(static_cast<double>(_pageList.size()) / (DFmap[aPair.first] + 1));
            _invertIndexLib[aPair.first].emplace(id, w);
            dw[id] += w * w;
        }
        dw[id] = sqrt(dw[id]);
    }
    for(auto &wordMap : _invertIndexLib)
    {
        for(auto &p : wordMap.second)
        {
            p.second /= dw[p.first];
        }
    }
}

void PageLibPreprocessor::storeOnDisk()
{
    auto &config = *Configuration::getInstance();
    std::ofstream ofs(config["invertIndex"]);
    for(auto &line : _invertIndexLib)
    {
        ofs << line.first;
        for(auto &aPair : line.second)
        {
            ofs << " " << aPair.first << " " << aPair.second;
        }
        ofs << "\n";
    }
    ofs.close();
}
