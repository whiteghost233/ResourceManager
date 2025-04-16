#include "ResourceManagerTool.h"

void ResourceManagerTool::ReleaseResource(ResourceID ID)
{
    auto It=ResourcesMap.find(ID);
    if(It!=ResourcesMap.end())
    {
        It->second->ReleaseTime= std::chrono::steady_clock::now();
        It->second->UseCount--;
    }
}

void ResourceManagerTool::SetReleaseDelay(std::chrono::milliseconds Delay)
{
    ReleaseDelay=Delay;
}

void ResourceManagerTool::ExecuGC()
{
    auto Now=std::chrono::steady_clock::now();
    for(auto It=ResourcesMap.begin();It!=ResourcesMap.end();)
    {
        if(Now-It->second->ReleaseTime>=ReleaseDelay&&It->second->UseCount==0)
        {
            It=ResourcesMap.erase(It);
        }else
        {
            ++It;
        }
    }
}

size_t ResourceManagerTool::GetUseCount()
{
    return ResourcesMap.size();
}


