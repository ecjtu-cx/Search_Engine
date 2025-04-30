#include "../../include/DictProducer/SplitTool.h"
#include "../../include/DictProducer/DictProducer.h"
#include "../../include/SearchEngineServer/Configuration.h"

int main()
{
    SplitTool *stl = Cppjieba::getSplitTool();
    auto &configs = *Configuration::getInstance();
    DictProducer dp(stl);
    dp.storeDict(configs["xmldict_path"].c_str());
    dp.storeIndex(configs["index_path"].c_str());
    //dp.storeIndex("../data/dict/dictindex(1).dat");
    DictProducer dp1;
    dp1.storeDict(configs["engdict_path"].c_str());
    dp1.storeIndex(configs["en_index_path"].c_str());

    return 0;
}