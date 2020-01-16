#pragma once

/// Abstract base network class for server.
class INetworkServerBase
{
public:
	virtual bool IsClientConnected(const int& clientIndex) = 0;
};


