#include "../../include/PagePreprocessor/DirScanner.h"

void DirScanner::operator()(const std::string &dir)
{
    traverse(dir);
}

std::vector<std::string> &DirScanner::getFiles()
{
    return _files;
}

void DirScanner::traverse(const std::string &dir)
{
    DIR *pdir = opendir(dir.c_str());
    if(nullptr == pdir)
    {
        perror("opendir");
    }
    struct dirent *pd;
    while((pd = readdir(pdir)))
    {
        if(strcmp(pd->d_name, ".") == 0 || strcmp(pd->d_name, "..") == 0)
        {
            continue;
        }
        _files.push_back(std::string(dir) + std::string("/") + std::string(pd->d_name));
    }
    closedir(pdir);
}
