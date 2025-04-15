#pragma once
class ResourceManagerTool
{
public:
	ResourceManagerTool(size_t Cnt = 0);

	void SetCount(size_t Cnt);

	size_t GetCount();

private:
	size_t Count;
};

