#include "../../include/DictProducer/DictProducer.h"
#include "../../include/SearchEngineServer/Configuration.h"

DictProducer::DictProducer()
    : _config(Configuration::getInstance()->getConfigMap()), _cuttor(nullptr), _dict(), _dict_index(), _index()
{
    buildEnDict();
    createIndex();
}

DictProducer::DictProducer(SplitTool *cuttor)
    : _config(Configuration::getInstance()->getConfigMap()), _cuttor(cuttor), _dict(), _dict_index(), _index()
{
    buildCnDict();
    createIndex();
}

void DictProducer::buildEnDict()
{
    unordered_set<string> stop_word;
    Configuration *config = Configuration::getInstance();
    stop_word = config->getStopWordList();

    string englishPage(_config["englishpage"]);
    ifstream str(englishPage.c_str());
    if (!str.is_open())
    {
        cerr << "englishPath Open Error !" << endl;
    }
    str.seekg(std::ios_base::beg);
    string sentence;
    while (getline(str, sentence))
    {
        istringstream ss(sentence);
        string words;
        while (ss >> words)
        {
            for (int c = 0; c < words.size(); ++c)
            {
                if (words[c] >= 'A' && words[c] <= 'Z')
                {
                    words[c] = tolower(words[c]);
                }

                if (!isalpha(words[c]))
                {
                    words.erase(c);
                }
            }
            auto stop = stop_word.find(words);
            if (stop != stop_word.end())
            {
                continue;
            }
            else
            {
                auto exist = _dict_index.find(words);
                if (exist != _dict_index.end())
                {
                    ++(_dict[exist->second].second);
                }
                else
                {
                    _dict.push_back(pair<string, int>(words, 1));
                    _dict_index.insert(pair<string, int>(words, _dict.size()));
                }
            }
        }
    }
}

void DictProducer::buildCnDict()
{
    // map<string, string> &config = Configuration::getInstance()->getConfigMap();
    string pagePath(_config["ripepage"]);
    ifstream ifs(pagePath);
    if (!ifs.is_open())
    {
        cerr << "pagePath Open error !" << endl;
    }
    ifs.seekg(std::ios_base::beg);
    string sentence;
    vector<string> words;
    while (getline(ifs, sentence))
    {
        _cuttor->cut(sentence, words);
        for (auto &it : words)
        {
            auto exist = _dict_index.find(it);
            if (exist != _dict_index.end())
            {
                ++(_dict[exist->second].second);
            }
            else
            {
                _dict.push_back(pair<string, int>(it, 1));
                _dict_index.insert(pair<string, int>(it, _dict.size()));
            }
        }
    }
}

void DictProducer::createIndex()
{
    string letter;
    int index = 0;

    if (_cuttor == nullptr) //此标志说明是英文
    {
        for (auto &it : _dict)
        {
            for (char c : it.first)
            {
                letter = c;
                auto map_it = _index.find(letter);
                if (map_it != _index.end())
                {
                    map_it->second.insert(index);
                }
                else
                {
                    set<int> nset;
                    nset.insert(index);
                    _index.insert(pair<string, set<int>>(letter, nset));
                }
            }
            ++index;
        }
    }
    else //中文
    {
        for (auto &it : _dict)
        {
            int i = 0;
            while (i < it.first.size())
            {
                unsigned char c = it.first[i];
                if (c < 128)
                {
                    ++i;
                    continue;
                }
                int cnt = 1;
                for (cnt; cnt < 6; ++cnt)
                {
                    unsigned char b = c << cnt; //移位的是utf8编码，如果第一个字节是1110 xxxx,则左移3位是0xxx xxxx,小于128，为中文
                    if (b < 128)
                    {
                        break;
                    }
                }

                letter = it.first.substr(i, cnt);
                i += cnt;
                auto map_it = _index.find(letter);
                if (map_it != _index.end())
                {
                    map_it->second.insert(index);
                }
                else
                {
                    set<int> nset;
                    nset.insert(index);
                    _index.insert(pair<string, set<int>>(letter, nset));
                }
            }
            ++index;
        }
    }
}

void DictProducer::storeDict(const char *path)
{
    ofstream wfile(path);
    if (!wfile.good())
    {
        cout << "storeDict Error!" << endl;
        return;
    }
    for (auto &it : _dict)
    {
        wfile << it.first << " " << it.second << endl;
    }
    wfile.close();
}

void DictProducer::storeIndex(const char *path)
{
    ofstream wfile(path);
    if (!wfile.good())
    {
        cout << "storeIndex Error!" << endl;
        return;
    }
    for (auto &it : _index)
    {
        wfile << it.first << " ";
        for (auto &set_it : it.second)
        {
            wfile << set_it << " ";
        }
        wfile << endl;
    }
    wfile.close();
}
