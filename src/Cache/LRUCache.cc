#include "../../include/Cache/LRUCache.h"

//LRUCache::LRUCache(int capacity)
//: _capacity(capacity)
//, _isUpdating(false)
//{
//
//}
LRUCache::LRUCache(const LRUCache & cache)
: _capacity(cache._capacity)
{
    update(cache);
}

bool LRUCache::get(string key, json &value) //查找_resultList中有无key值，若有，传出value
{
    auto it = _umap.find(key);
    if(it == _umap.end())
    {
        return false;
    }
    _resultList.splice(_resultList.begin(), _resultList, it->second);
    _pendingUpdateList.remove(*(it->second));
    _pendingUpdateList.push_front(std::move(make_pair(it->second->first, it->second->second)));

    value = it->second->second;

    return true;

}


void LRUCache::put(string key, json &value) //将key-value放入缓存中
{
    auto it = _umap.find(key);
    if(it == _umap.end())
    {
        if(_umap.size() >= _capacity)
        {
            _umap.erase(_resultList.back().first);
            _resultList.pop_back();
        }
        _resultList.push_front(std::move(make_pair(key, value)));
        _umap[key] = _resultList.begin();

        _pendingUpdateList.push_front(std::move(make_pair(key, value)));

    }
    else 
    {
        _resultList.splice(_resultList.begin(), _resultList, it->second);
        //_pendingUpdateList.splice(_pendingUpdateList.begin(), _pendingUpdateList, it->second);
        //_pendingUpdateList.remove(*(it->second));
        //_pendingUpdateList.push_front(std::move(make_pair(it->second->first, it->second->second)));
    }
}

void LRUCache::writeToFile(string filename)
{
    ofstream ofs(filename);
    if(!ofs.good())
    {
        cout << "error : " << filename << endl;
    }
    for( auto &it : getResultList())
    {
        ofs << it.first << " " << it.second << endl;
    }
    ofs.close();


}
void LRUCache::writeToFile(string filename, string key, json value)
{
    ofstream ofs(filename, std::ios::app);
    if(!ofs.good())
    {
        cout << "error : " << filename << endl;
    }
    ofs << key << " " << value << endl;
    ofs.close();

}

void LRUCache::readFromFile(string filename)
{
    ifstream ifs(filename);
    if(!ifs.good())
    {
        cout << "error: " << filename << endl;
    }

    string line;
    while(getline(ifs, line))
    {
        istringstream iss(line);
        string key;
        json value;
        iss >> key;
        iss >> value;
        
        put(key, value);
    }
    ifs.close();
}

void LRUCache::update(const LRUCache &cache)
{
    _isUpdating = true;
    int count = 0;
    auto it = cache._pendingUpdateList.rbegin();
    for(int distance = _pendingUpdateList.size() - _resultList.size(); distance > 0; --distance)
    {
        ++it;
    }

    for(; (it != cache._pendingUpdateList.rend()); ++it)
    //for(auto &it : _pendingUpdateList)
    {
        json j = it->second;
        put(it->first, j);
        ++count;
        if(count >= _capacity)
        {
            _isUpdating = false;
            return;
        }
    }
    _isUpdating = false;

}

void LRUCache::show()
{
    for(auto it = _resultList.begin(); it != _resultList.end(); ++it)
    {
        cout << "key: " << it->first << " ";
        cout << "value: ";
        for (auto iv : it->second)
        {
            cout << iv << " ";
        }
        cout << endl;
    }

}

list<pair<string, json>> &LRUCache::getPendingUpdateList()
{
    return _pendingUpdateList;
}


list<pair<string, json>> &LRUCache::getResultList()
{
    return _resultList;
}



#if 0
int main()
{
    LRUCache lc(2);

    //lc.readFromFile("cache.dat");
    string s1("yes");
    json j1 = nlohmann::json("yes1 yes2 yes3"); 

    string s2("here");
    json j2 = nlohmann::json("here1 here2 here3"); 

    string s3("you");
    json j3 = nlohmann::json("you1 you2"); 

    lc.put(s1, j1);
    lc.put(s2, j2);
    lc.put(s3, j3);
    cout << "before search: " << endl;
    cout << "lc._resultList: " << endl;
    lc.show();
    cout << endl;

    string s("here");
    json j;
    if(lc.get(s, j))
    {
        cout << s << " found!" << endl; 
        cout << s << " " << j << endl;
    }

    cout << "lc._resultList: " << endl;
    lc.show();
    cout << endl;

    cout << "lc._pendingUpdateList: " << endl;
    for(auto it : lc.getPendingUpdateList())
    {
        cout << it << endl;
    }

    lc.update(lc);
    cout << "after update: " << endl;
    lc.show();
    cout << endl;
    
    lc.writeToFile("wordcache.dat");

}

#endif

