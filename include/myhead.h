#ifndef _MYHEAD_H_
#define _MYHEAD_H_
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <memory>
#include <queue>
#include <fstream>
#include <sstream>
#include <sys/eventfd.h>
#include <pthread.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <regex>
#include <dirent.h>
#include <ctype.h>
#include <numeric>
#include <math.h>
#define DEBUG
#define RELEASE
using std::vector;
using std::string;
using std::list;
using std::unordered_map;
using std::pair;
using std::map;
using std::set;
using std::unique_ptr;
using std::shared_ptr;
using std::priority_queue;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::istringstream;
using std::ostringstream;
using std::stringstream;
using std::unordered_set;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

#define ARGS_CHECK(argc, num)            \
{if(argc != num)                         \
    {                                    \
        fprintf(stderr, "args error!\n"); \
        return -1;                       \
    }                                    \
}                                        

#define ERROR_CHECK(ret, num, msg)      \
{                                       \
    if(ret == num) {                    \
        perror(msg);                    \
        return -1;                      \
    }                                   \
}

#define THREAD_ERROR_CHECK(ret, msg)                    \
{                                                       \
    if(ret != 0) {                                      \
        fprintf(stderr, "%s:%s\n", msg,strerror(ret));  \
        return -1;                                      \
    }                                                   \
}

#endif
