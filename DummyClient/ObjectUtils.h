#pragma once

class ObjectUtils
{
public:
	static std::shared_ptr<Dummy> CreateDummy(std::shared_ptr<DummySession> session);
};

