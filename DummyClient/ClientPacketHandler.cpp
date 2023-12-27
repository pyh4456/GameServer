#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer, len);
		break;
	}
}

#pragma pack(1)
// [PKT_S_TEST][BuffsListItem BuffsListItem ... BuffsListItem]
struct PKT_S_TEST
{
	struct BuffsListItem
	{
		uint64 buffId;
		float remainTime;
	};

	uint16 packetSize; // 공용 헤더
	uint16 packetId; // 공용 헤더
	uint64 id;
	uint32 hp;
	uint16 attack;
	uint16 buffsOffset;
	uint16 buffsCount;

	bool Validate()
	{
		uint32 size = 0;

		size += sizeof(PKT_S_TEST);
		if (packetSize < size)
			return false;

		size += buffsCount * sizeof(BuffsListItem);
		if (size != packetSize)
			return false;

		if (buffsOffset + buffsCount * sizeof(BuffsListItem) > packetSize)
			return false;

		return true;

	}

	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;

	BuffsList GetBuffsList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsOffset;
		return BuffsList(reinterpret_cast<PKT_S_TEST::BuffsListItem*>(data), buffsCount);
	}
	//vector<BuffData> buffs;
	//wstring name;
};
#pragma pack()

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PKT_S_TEST* pkt = reinterpret_cast<PKT_S_TEST*>(buffer);


	if (pkt->Validate() == false)
		return;

	//cout << "ID : " << id << ", HP : " << hp << ", ATT : " << attack << endl;

	PKT_S_TEST::BuffsList buffs = pkt->GetBuffsList();

	cout << "BuffCount : " << buffs.Count() << endl;
	for (int32 i = 0; i < buffs.Count(); i++)
	{
		cout << "BuffInfo : " << buffs[i].buffId << " " << buffs[i].remainTime << endl;
	}

	for (auto it = buffs.begin(); it != buffs.end(); ++it)
	{
		cout << "BuffInfo : " << it->buffId << " " << it->remainTime << endl;
	}

	for (auto& buff : buffs)
	{
		cout << "BuffInfo : " << buff.buffId << " " << buff.remainTime << endl;
	}
}
