#include "Actor.h"

void Actor::ChangeParent(Actor* ParentActor)
{
    if(nullptr==ParentActor)
    {
        return;
    }
    
    Parent=ParentActor;
}

void Actor::AddChildActor(Actor* ChildActor)
{
    if(nullptr==ChildActor)
    {
        return;
    }
    
    ChildList.push_back(ChildActor);
}

void Actor::RemoveChildActor(Actor* ChildActor)
{
    for (auto It = ChildList.begin(); It != ChildList.end();)
    {
        if (*It==ChildActor)
        {
            It = ChildList.erase(It);
            break;
        }
        else
        {
            ++It;
        }
    }
}
