#include "ResourceManagerTool.h"

bool ResourceManagerTool::CheckIfActorInGC(size_t ID)
{
    auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return false;
    }

    if (nullptr == It->second)
    {
        return false;
    }

    return true;
}

size_t ResourceManagerTool::CreateResourceActor(std::shared_ptr<Actor> Parent)
{
    auto NewWrapper = std::make_shared<ResourceWrapper>();

    NewWrapper->ResouceActor = std::make_shared<Actor>();

    *(NewWrapper->ReleaseTime) = std::chrono::steady_clock::now();

    ResourcesMap.insert({NewWrapper->ResouceActor->ID, NewWrapper});

    if (Parent == nullptr)
    {
        return NewWrapper->ResouceActor->ID;
    }

    NewWrapper->bCanReach = true;

    NewWrapper->ResouceActor->SetParent(std::weak_ptr<Actor>(Parent));

    Parent->AddChildActor(NewWrapper->ResouceActor);

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

std::shared_ptr<Actor> ResourceManagerTool::GetResourceActorWithLink(size_t ID)
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

    It->second->UseCount++;

    return It->second->ResouceActor;
}

void ResourceManagerTool::RestoreResourceActorWithLink(size_t ID)
{
    auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return;
    }

    if (nullptr == It->second)
    {
        return;
    }

    It->second->UseCount--;
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

size_t ResourceManagerTool::GetResouceCountWithGC()
{
    return ResourcesMap.size();
}
