#include "ResourceManager.h"

bool ResourceManager::CheckIfResourceInGC(size_t ID)
{
    const auto It = ResourcesMap.find(ID);

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

size_t ResourceManager::CreateResourceResource(const std::shared_ptr<Resource>& Parent)
{
    auto NewWrapper = std::make_shared<ResourceWrapper>();

    NewWrapper->Resource = std::make_shared<Resource>();

    *(NewWrapper->ReleaseTime) = std::chrono::steady_clock::now();

    ResourcesMap.insert({NewWrapper->Resource->ID, NewWrapper});

    if (nullptr == Parent)
    {
        return NewWrapper->Resource->ID;
    }

    NewWrapper->Resource->SetParent(std::weak_ptr<Resource>(Parent));

    Parent->AddChildResource(NewWrapper->Resource);

    const auto It = ResourcesMap.find(Parent->ID);

    if (ResourcesMap.end() == It || nullptr == It->second)
    {
        return NewWrapper->Resource->ID;
    }

    return NewWrapper->Resource->ID;
}

std::shared_ptr<Resource> ResourceManager::GetResourceResourceWithLink(const size_t ID)
{
    const auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return nullptr;
    }

    if (nullptr == It->second)
    {
        return nullptr;
    }

    *(It->second->ReleaseTime) = std::chrono::steady_clock::now();

    return It->second->Resource;
}

void ResourceManager::DislinkResourceResource(const size_t ID)
{
    const auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return;
    }

    if (nullptr == It->second)
    {
        return;
    }

    *(It->second->ReleaseTime) = std::chrono::steady_clock::now();
}

void ResourceManager::ADDToRoot(const size_t ID)
{
    const auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return;
    }

    RootResource->AddChildResource(It->second->Resource);

    It->second->Resource->SetParent(RootResource);
}

void ResourceManager::RemoveFromRoot(const size_t ID)
{
    const auto It = ResourcesMap.find(ID);

    if (ResourcesMap.end() == It)
    {
        return;
    }

    It->second->Resource->SetParent();

    RootResource->RemoveChildResource(It->second->Resource);
}

void ResourceManager::SetReleaseDelay(const std::chrono::milliseconds& Delay)
{
    ReleaseDelay = Delay;
}

void ResourceManager::ExecuteGC()
{
    static int GCtime = 0;
    std::cout << "GC Times : " << GCtime++ << std::endl;
    const auto Now = std::chrono::steady_clock::now();
    for (auto It = ResourcesMap.begin(); It != ResourcesMap.end();)
    {
        if (1 == It->second->Resource.use_count() &&
            Now - *(It->second->ReleaseTime) >= ReleaseDelay)
        {
            It = ResourcesMap.erase(It);
        }
        else
        {
            ++It;
        }
    }
}

void ResourceManager::StartGC()
{
    GcThread = std::thread([this]()
    {
        GCWorker();
    });
    GcThread.detach();
}

void ResourceManager::GCWorker()
{
    while (true)
    {
        std::this_thread::sleep_for(ReleaseDelay);
        ExecuteGC();
    }
}

size_t ResourceManager::GetResouceCountWithGC() const 
{
    return ResourcesMap.size();
}
