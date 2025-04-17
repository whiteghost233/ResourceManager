#pragma once
#include <chrono>
#include <unordered_map>
#include <thread>

#include "Actor.h"

class ResourceManagerTool
{
public:
    ResourceManagerTool()
    {
        RootActor=new Actor(nullptr);
        
        StartGC();
    }

    ~ResourceManagerTool()
    {
        delete RootActor;
    }

    using TimePoint = std::chrono::steady_clock::time_point;

    size_t CreateResourceActor(Actor *Parent=nullptr);

    Actor* GetResourceActor(size_t ID);

    void ReleaseResource(size_t ID);

    void SetReleaseDelay(std::chrono::milliseconds Delay);

    void ExecuteGC();

    void GCWorker();

    void StartGC();

    size_t GetUseCount();

    void ADDToRoot(size_t ID);

    void RemoveFromRoot(size_t ID);

private:
    struct ResourceWrapperBase
    {
        ResourceWrapperBase()
        {
            ReleaseTime=new TimePoint;
        };
        
        virtual ~ResourceWrapperBase()
        {
            delete ResouceActor;
            delete ReleaseTime;
        };
        
        Actor *ResouceActor;
        
        TimePoint *ReleaseTime;
        
        size_t UseCount = 0;
        
        bool bCanReach = false;
    };

    std::unordered_map<size_t, ResourceWrapperBase*> ResourcesMap;
    
    std::vector<size_t> PendingReleaseVec;
    
    std::chrono::milliseconds ReleaseDelay = std::chrono::milliseconds(1000);
    
    //size_t NextID = 0;
    
    std::thread GcThread;

    Actor *RootActor;
};
