#pragma once

//-------------------
//		1Â÷½Ãµµ
//-------------------

struct SListEntry 
{
	SListEntry* next;
};

class Data
{
public:
	SListEntry _entry;

	int32 _hp;
	int32 _mp;
};

struct SListHeader {
	SListEntry* next = nullptr;
};

void InitializedHead(SListHeader* header) {
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry) {
	entry->next = header->next;
	header->next = entry;
}

SListEntry* PopEntrySList(SListHeader* header) {
	SListEntry* first = header->next;
	
	if (first != nullptr)
		header->next = first->next;

	return first;
}
