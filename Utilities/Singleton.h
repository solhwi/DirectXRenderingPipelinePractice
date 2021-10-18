#pragma once

template <typename T>
class Singleton
{
public:
	static void Create()
	{
		if (instance == nullptr)
			instance = new T();
	}

	static void Delete()
	{
		delete(instance);
		instance = nullptr;
	}

	static T* Get()
	{
		return instance;
	}

protected:
	static T* instance;
};

template <typename T>
T* Singleton<T>::instance = nullptr;