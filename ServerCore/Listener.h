#pragma once
#include "IocpCore.h"
#include "NetAddress.h"
#include "IocpEvent.h"

class AcceptEvent;

/*-----------------
	Listener
------------------*/

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* �ܺο��� ��� */
	bool StartAcccept(NetAddress netAddress);
	void CloseCoskt();

public:
	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;
	Vector<AcceptEvent*> _acceptEvents;
};

