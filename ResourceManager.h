#pragma once
#include <chrono>
#include <memory>
#include <memory>
#include <unordered_map>
#include <thread>

#include "Resource.h"

constexpr std::chrono::milliseconds GCTime = std::chrono::milliseconds(1000);

class ResourceManager
{
public:
    ResourceManager()
    {
        RootResource = std::make_shared<Resource>();

        StartGC();
    }

    bool CheckIfResourceInGC(size_t ID);

    using TimePoint = std::chrono::steady_clock::time_point;

    size_t CreateResourceResource(const std::shared_ptr<Resource>& Parent = {});

    std::shared_ptr<Resource> GetResourceResourceWithLink(const size_t ID);

    void DislinkResourceResource(const size_t ID);

    void SetReleaseDelay(const std::chrono::milliseconds& Delay);

    void ExecuteGC();

    void GCWorker();

    void StartGC();

    size_t GetResouceCountWithGC() const;

    void ADDToRoot(const size_t ID);

    void RemoveFromRoot(const size_t ID);

private:
    struct ResourceWrapper
    {
        ResourceWrapper()
        {
            ReleaseTime = std::make_shared<TimePoint>();
        }
        
        std::shared_ptr<Resource> Resource;

        std::shared_ptr<TimePoint> ReleaseTime;
    };

    std::unordered_map<size_t, std::shared_ptr<ResourceWrapper>> ResourcesMap;

    std::vector<size_t> PendingReleaseVec;

    std::chrono::milliseconds ReleaseDelay = GCTime;

    std::thread GcThread;

    std::shared_ptr<Resource> RootResource;
};
