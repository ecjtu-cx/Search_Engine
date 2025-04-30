#include "../../include/PagePreprocessor/PageLibPreprocessor.h"
#include "../../include/PagePreprocessor/PageLib.h"
#include <chrono>

using namespace std;

int main()
{
    PageLib pl;
    pl.create();
    pl.store();
    auto t1 = std::chrono::high_resolution_clock::now();
    PageLibPreprocessor plp;
    auto t2 = std::chrono::high_resolution_clock::now();
    plp.bulidInvertIndexMap();
    auto t3 = std::chrono::high_resolution_clock::now();
    plp.storeOnDisk();
    auto t4 = std::chrono::high_resolution_clock::now();
    printf("读入网页库:\t%fms\n建立倒排索引:\t%fms\n写入磁盘:\t%fms\n",
           std::chrono::duration<double, std::milli>(t2 - t1).count(),
           std::chrono::duration<double, std::milli>(t3 - t2).count(),
           std::chrono::duration<double, std::milli>(t4 - t3).count());
    return 0;
}
