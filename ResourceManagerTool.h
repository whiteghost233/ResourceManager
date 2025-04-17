#pragma once
#include <chrono>
#include <unordered_map>
#include <memory>
#include <thread>

class ResourceManagerTool
{
public:
    ResourceManagerTool()
    {
        StartGC();
    }

    using TimePoint = std::chrono::steady_clock::time_point;

    template <typename T, typename... Args>
    size_t AddResource(Args&&... args)
    {
        size_t NewID = NextID++;

        T* Resource = new T(std::forward<Args>(args)...);
        ResourceWrapper<T>* wrapper = new ResourceWrapper<T>();
        wrapper->Resource = Resource;
        wrapper->ReleaseTime = std::chrono::steady_clock::now();

        ResourcesMap[NewID] = wrapper;
        return NewID;
    }

    template <typename T>
    T* AcquireResource(size_t ID)
    {
        auto It = ResourcesMap.find(ID);
        if (It != ResourcesMap.end())
        {
            auto CurResource = dynamic_cast<ResourceWrapper<T>*>(It->second);
            CurResource->UseCount++;
            return CurResource->Resource;
        }
        return nullptr;
    }

    void ReleaseResource(size_t ID);

    void SetReleaseDelay(std::chrono::milliseconds Delay);

    void ExecuGC();

    void GCWorker();

    void StartGC();

    size_t GetUseCount();

private:
    struct ResourceWrapperBase
    {
        virtual ~ResourceWrapperBase() = default;
        TimePoint ReleaseTime;
        size_t UseCount = 0;
    };

    template <typename T>
    struct ResourceWrapper : ResourceWrapperBase
    {
        T* Resource;

        ~ResourceWrapper() override { delete Resource; }
    };

    std::unordered_map<size_t, ResourceWrapperBase*> ResourcesMap;
    std::vector<size_t> PendingReleaseVec;
    std::chrono::milliseconds ReleaseDelay = std::chrono::milliseconds(300);
    size_t NextID = 0;
    std::thread GcThread;
};
