#pragma once

/*------------------------
	MemoryHeader
-------------------------*/

struct MemoryHeader {
	//[MemoryHeader][Data]
	MemoryHeader(int32 size) : allocSize(size) { }

	static void* AttachHeader(MemoryHeader* header, int32 size) {
		new(header)MemoryHeader(size);
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr) {
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
};

/*------------------------
	MemoryPool
-------------------------*/

class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader*	Pop();
private:
	int32 _allocSize = 0;
	atomic<int32> _allocCount = 0;

	USE_LOCK;
	queue<MemoryHeader*> _queue;
};

