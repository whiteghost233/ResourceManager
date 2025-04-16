#pragma once
#include <chrono>
#include <unordered_map>
#include <memory>

class ResourceManagerTool
{
public:
    using ResourceID = size_t;
    using TimePoint = std::chrono::steady_clock::time_point;

    template <typename T, typename... Args>
    ResourceID AddResource()
    {
        ResourceID NewID = NextID++;

        ResourceWrapperBase *wrapper = new ResourceWrapper<T>();
        wrapper->ReleaseTime=std::chrono::steady_clock::now();
        ResourcesMap[NewID] = wrapper;
        return NewID;
    }

    template <typename T>
    std::shared_ptr<T> AcquireResource(ResourceID ID)
    {
        auto It=ResourcesMap.find(ID);
        if(It!=ResourcesMap.end())
        {
            auto CurResource=dynamic_cast <ResourceWrapper<T>*>(It->second);
            CurResource->UseCount++;
            return CurResource->Resource;
        }
        return nullptr;
    }

    void ReleaseResource(ResourceID ID);

    void SetReleaseDelay(std::chrono::milliseconds Delay);

    void ExecuGC();

    size_t GetUseCount();

private:
    struct ResourceWrapperBase
    {
        virtual ~ResourceWrapperBase() = default;
        TimePoint ReleaseTime;
        size_t UseCount=0;
    };

    template <typename T>
    struct ResourceWrapper : ResourceWrapperBase
    {
        std::shared_ptr<T> Resource;
    };

    std::unordered_map<ResourceID, ResourceWrapperBase*> ResourcesMap;
    std::vector<ResourceID> PendingReleaseVec;
    std::chrono::milliseconds ReleaseDelay = std::chrono::seconds(30);
    ResourceID NextID = 0;
};
