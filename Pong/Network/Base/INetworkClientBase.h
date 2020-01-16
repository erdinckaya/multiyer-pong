#pragma once

/// Abstract base network class for client.
class INetworkClientBase
{
public:
	virtual uint64_t GetClientId() const = 0;
};


