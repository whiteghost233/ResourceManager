#pragma once
#include <iostream>
#include <list>

class Actor
{
public:
    Actor(Actor* Parent = nullptr): Parent(Parent)
    {
        static size_t staticID = 0;
        ID = staticID;
        staticID++;
        std::cout << "Construct Actor: ID " << +ID << std::endl;
    }

    ~Actor()
    {
        for (auto It = ChildList.begin(); It != ChildList.end();)
        {
            delete *It;
            
            It = ChildList.erase(It);
        }   
        
        std::cout << "Delete Actor: ID " << +ID << std::endl;
    }

    size_t ID;

    void SetParent(Actor* NewParent);

    Actor* GetParent();

    void ChangeParent(Actor* ParentActor = nullptr);

    void AddChildActor(Actor* ChildActor);

    void RemoveChildActor(Actor* ChildActor);

private:
    Actor* Parent;

    std::list<Actor*> ChildList;
};
