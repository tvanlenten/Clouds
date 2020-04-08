#pragma once
#include <memory>

template <class T>
class EventHandeler
{
public:
	virtual void handle(std::shared_ptr<T> e) = 0;
};

