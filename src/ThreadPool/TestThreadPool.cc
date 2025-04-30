#include "../../include/ThreadPool/ThreadPool.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

// class MyTask
// {
// public:
//     void process() 
//     {
//         ::srand(clock());;
//         int number = ::rand()%100;
//         cout << "bo_threadpool.number = " << number << endl;
//     }

// };

// int func(int *number)
// {}

// void test()
// {
//     int a = 10;
//     func(&a);
// }

// int main(int argc, char **argv)
// {

//     MyTask task;

//     ThreadPool threadPool(5, 10);
//     threadPool.start();//五个子线程创建并运行

//     int cnt = 20;
//     while(cnt--> 0)
//     {
//         //如果在此处task的生命周期达到了，使用地址传递的话，
//         //会有相应的空指针的问题
//         threadPool.addTask(std::bind(&MyTask::process, &task));//地址传递
//         /* threadPool.addTask(std::bind(&MyTask::process, task));//值传递 */
//         cout << "cnt = " << cnt << endl;
//     }

//     threadPool.stop();//五个子线程进行退出

//     return 0;
// }