#include "../../include/PagePreprocessor/PageLib.h"
#include "../../include/PagePreprocessor/DirScanner.h"
#include "../../include/PagePreprocessor/tinyxml2.h"

using namespace tinyxml2;

PageLib::PageLib()
    : _config(*Configuration::getInstance())
    , _simhasher(_config["dict_path"], _config["hmm_path"], _config["idf_path"], _config["stop_words_path"])
{
    _dirScanner(_config["xml_path"]);
}

void PageLib::create()
{
    std::vector<std::string> &files = _dirScanner.getFiles();
    XMLDocument doc;
    size_t topN = 20;
    for(int idx = 0; idx < files.size(); ++idx)
    {
        doc.LoadFile(files[idx].c_str());
        if(doc.ErrorID())
        {
            std::cerr << "LoadFile fail" << std::endl;
            return;
        }
        XMLElement *itemNode = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
        while(itemNode)
        {
            std::string title = itemNode->FirstChildElement("title")->GetText();
            std::string url = itemNode->FirstChildElement("link")->GetText();
            std::string content = itemNode->FirstChildElement("content") ? itemNode->FirstChildElement("content")->GetText()
                                : itemNode->FirstChildElement("description") ? itemNode->FirstChildElement("description")->GetText()
                                : "";
            std::regex reg("( |　|&nbsp;|[\r]|[\n]|<[^>]+>|showPlayer[(]+[^)]+[)];)");
            title = std::regex_replace(title, reg, "");
            content = std::regex_replace(content, reg, "");
            uint64_t contentHash;
            content.empty()
            ? _simhasher.make(title, topN, contentHash)
            : _simhasher.make(content, topN, contentHash);
            if(_simhashset.insert(contentHash).second)
            {
                _pages.emplace_back(std::string("<doc>\n\t<docid>") + std::to_string(_pages.size() + 1) + "</docid>\n\t<url>" + url + "</url>\n\t<title>" + title + "</title>\n\t<content>" + content + "</content>\n</doc>\n");
            }
            itemNode = itemNode->NextSiblingElement("item");
        }
    }
}

void PageLib::store()
{
    std::ofstream ofsp(_config["ripepage"]);
    std::ofstream ofsq(_config["offset"]);
    std::fstream::pos_type p1, p2;
    for(int idx = 0; idx < _pages.size(); ++idx)
    {
        p1 = ofsp.tellp();
        ofsp << _pages[idx];
        p2 = ofsp.tellp();
        ofsq << idx + 1 << " " << std::to_string(p1) << " " << std::to_string(p2 - p1) << "\n"; 
    }
    ofsp.close();
}
