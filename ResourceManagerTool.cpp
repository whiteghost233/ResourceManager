#include "ResourceManagerTool.h"

size_t ResourceManagerTool::CreateResourceActor(Actor* Parent)
{
    ResourceWrapperBase* NewWrapper = new ResourceWrapperBase();

    NewWrapper->ResouceActor = new Actor(Parent);

    *(NewWrapper->ReleaseTime) = std::chrono::steady_clock::now();

    ResourcesMap.insert({NewWrapper->ResouceActor->ID, NewWrapper});

    if (Parent == nullptr)
    {
        return NewWrapper->ResouceActor->ID;
    }

    NewWrapper->ResouceActor->SetParent(Parent);

    auto It = ResourcesMap.find(Parent->ID);

    if (ResourcesMap.end() == It)
    {
        return NewWrapper->ResouceActor->ID;
    }

    if (nullptr == It->second || false == It->second->bCanReach)
    {
        return NewWrapper->ResouceActor->ID;
    }

    return NewWrapper->ResouceActor->ID;
}

Actor* ResourceManagerTool::GetResourceActor(size_t ID)
{
    auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return nullptr;
    }

    if (nullptr == It->second)
    {
        return nullptr;
    }

    It->second->bCanReach = true;

    return It->second->ResouceActor;
}

void ResourceManagerTool::ADDToRoot(size_t ID)
{
    auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return;
    }

    RootActor->AddChildActor(It->second->ResouceActor);

    It->second->ResouceActor->SetParent(RootActor);

    It->second->bCanReach = true;
}

void ResourceManagerTool::RemoveFromRoot(size_t ID)
{
    auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return;
    }

    RootActor->RemoveChildActor(It->second->ResouceActor);
}

void ResourceManagerTool::ReleaseResource(size_t ID)
{
    auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return;
    }

    *(It->second->ReleaseTime) = std::chrono::steady_clock::now();

    It->second->bCanReach = false;
}

void ResourceManagerTool::SetReleaseDelay(std::chrono::milliseconds Delay)
{
    ReleaseDelay = Delay;
}

void ResourceManagerTool::ExecuteGC()
{
    static int GCtime = 0;
    std::cout << GCtime++ << std::endl;
    auto Now = std::chrono::steady_clock::now();
    for (auto It = ResourcesMap.begin(); It != ResourcesMap.end();)
    {
        if (false == It->second->bCanReach
            && 0 == It->second->UseCount
            && Now - *(It->second->ReleaseTime) >= ReleaseDelay)
        {
            delete It->second;
            It = ResourcesMap.erase(It);
        }
        else
        {
            ++It;
        }
    }
}

void ResourceManagerTool::StartGC()
{
    GcThread = std::thread([this]()
    {
        GCWorker();
    });
    GcThread.detach();
}

void ResourceManagerTool::GCWorker()
{
    while (true)
    {
        std::this_thread::sleep_for(ReleaseDelay);
        ExecuteGC();
    }
}

size_t ResourceManagerTool::GetUseCount()
{
    return ResourcesMap.size();
}
