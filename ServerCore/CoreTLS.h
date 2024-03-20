#pragma once
#include <stack>

extern thread_local uint32				LThreadId;
extern thread_local uint64				LEndTickCount;

extern thread_local class JobQueue* LCurrentJobQueue;