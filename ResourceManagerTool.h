#pragma once
#include <chrono>
#include <memory>
#include <memory>
#include <unordered_map>
#include <thread>

#include "Actor.h"

class ResourceManagerTool
{
public:
    ResourceManagerTool()
    {
        RootActor = std::make_shared<Actor>();

        StartGC();
    }

    ~ResourceManagerTool()
    {
    }

    bool CheckIfActorInGC(size_t ID);

    using TimePoint = std::chrono::steady_clock::time_point;

    size_t CreateResourceActor(std::shared_ptr<Actor> Parent = {});

    std::shared_ptr<Actor> GetResourceActorWithLink(size_t ID);

    void DislinkResourceActor(size_t ID);
    
    void SetReleaseDelay(std::chrono::milliseconds Delay);

    void ExecuteGC();

    void GCWorker();

    void StartGC();

    size_t GetResouceCountWithGC();

    void ADDToRoot(size_t ID);

    void RemoveFromRoot(size_t ID);

private:
    struct ResourceWrapper
    {
        ResourceWrapper()
        {
            ReleaseTime = std::make_shared<TimePoint>();
        };

        virtual ~ResourceWrapper()
        {
        };

        std::shared_ptr<Actor> ResouceActor;

        std::shared_ptr<TimePoint> ReleaseTime;
    };

    std::unordered_map<size_t, std::shared_ptr<ResourceWrapper>> ResourcesMap;

    std::vector<size_t> PendingReleaseVec;

    std::chrono::milliseconds ReleaseDelay = std::chrono::milliseconds(1000);

    std::thread GcThread;

    std::shared_ptr<Actor> RootActor;
};
