#pragma once

template<typename T>
class LockQueue
{
public:
	void Push(T item)
	{
		WRITE_LOCK;
		_items.push(item);
	}

	T Pop()
	{
		WRITE_LOCK;
		return PopNoLock();
	}

	T PopNoLock()
	{
		if (_items.empty())
			return T();

		T ret = _items.front();
		_items.pop();
		return ret;
	}

	void PopAll(OUT vector<T>& items)
	{
		WRITE_LOCK;
		while (T item = PopNoLock())
			items.push_back(item);
	}

	void Clear()
	{
		WRITE_LOCK;
		_items = queue<T>();
	}

private:
	USE_LOCK;
	queue<T> _items;
};