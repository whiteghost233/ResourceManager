#include "ResourceManagerTool.h"

ResourceManagerTool::ResourceManagerTool(size_t Cnt):Count(Cnt)
{
}

void ResourceManagerTool::SetCount(size_t Cnt)
{
	Count = Cnt;
}

size_t ResourceManagerTool::GetCount()
{
	return Count;
}
