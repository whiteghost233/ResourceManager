#include "Resource.h"

void Resource::SetParent(const std::weak_ptr<Resource>& NewParent)
{
    Parent = NewParent;
}

std::weak_ptr<Resource> Resource::GetParent()
{
    return Parent;
}

void Resource::AddChildResource(const std::shared_ptr<Resource>& ChildActor)
{
    if (nullptr == ChildActor)
    {
        return;
    }

    ChildList.push_back(ChildActor);
}

void Resource::RemoveChildResource(const std::shared_ptr<Resource>& ChildActor)
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
