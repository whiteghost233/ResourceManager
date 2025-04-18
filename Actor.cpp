#include "Actor.h"

void Actor::SetParent(std::weak_ptr<Actor> NewParent)
{
    Parent = NewParent;
}

std::weak_ptr<Actor> Actor::GetParent()
{
    return Parent;
}

void Actor::ChangeParent(std::weak_ptr<Actor> ParentActor)
{
    Parent = ParentActor;
}

void Actor::AddChildActor(const std::shared_ptr<Actor>& ChildActor)
{
    if (nullptr == ChildActor)
    {
        return;
    }

    ChildList.push_back(ChildActor);
}

void Actor::RemoveChildActor(const std::shared_ptr<Actor>& ChildActor)
{
    for (auto It = ChildList.begin(); It != ChildList.end();)
    {
        if (*It == ChildActor)
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
