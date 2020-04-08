#pragma once
#include <vector>

namespace Utils
{
	template<typename T>
	bool Remove(std::vector<T>& vec, T item) //is this bad???
	{
		int index = -1;
		for (int i = 0; i < vec.size(); ++i)
		{
			if (vec[i] == item)
			{
				index = i;
				break;
			}
		}
		if (index != -1)
		{
			vec.erase(vec.begin() + index);
			return true;
		}
		return false;
	}

	template<typename T>
	void PushFront(std::vector<T>& vec, T item)
	{
		vec.insert(vec.begin(), item);
	}

	/*
		returns -1 if not found
	*/
	template<typename T>
	int FindIndex(std::vector<T>& vec, T item)
	{
		int index = -1;
		for (int i = 0; i < vec.size(); ++i)
		{
			if (vec[i] == item)
			{
				index = i;
				break;
			}
		}

		return index;
	}
}