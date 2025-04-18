#pragma once
#include <iostream>
#include <list>
#include <memory>

class Actor
{
public:
    Actor(std::weak_ptr<Actor> Parent = {}): Parent(Parent)
    {
        static size_t staticID = 0;
        ID = staticID;
        staticID++;
        std::cout << "Construct Actor: ID " << +ID << std::endl;
    }

    ~Actor()
    {
        std::cout << "Delete Actor: ID " << +ID << std::endl;
    }

    size_t ID;

    void SetParent(std::weak_ptr<Actor> NewParent={});

    std::weak_ptr<Actor> GetParent();

    void ChangeParent(std::weak_ptr<Actor> ParentActor);

    void AddChildActor(const std::shared_ptr<Actor>& ChildActor);

    void RemoveChildActor(const std::shared_ptr<Actor>& ChildActor);

private:
    std::weak_ptr<Actor> Parent;

    std::list<std::shared_ptr<Actor>> ChildList;
};
