#pragma once
#include <iostream>
#include <list>
#include <memory>

class Resource
{
public:
    Resource(std::weak_ptr<Resource> Parent = {}): Parent(Parent)
    {
        static size_t staticID = 0;
        
        ID = staticID;
        
        staticID++;
        
        std::cout << "Construct Resource: ID " << +ID << std::endl;
    }

    ~Resource()
    {
        std::cout << "Delete Resource: ID " << +ID << std::endl;
    }

    size_t ID;

    void SetParent(const std::weak_ptr<Resource>& NewParent={});

    std::weak_ptr<Resource> GetParent();

    void AddChildResource(const std::shared_ptr<Resource>& ChildActor);

    void RemoveChildResource(const std::shared_ptr<Resource>& ChildActor);

private:
    std::weak_ptr<Resource> Parent;

    std::list<std::shared_ptr<Resource>> ChildList;
};
