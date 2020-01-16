#pragma once


/// Base abstact network class for client and server.
class INetworkBase
{
public:
	virtual void Run() = 0;
	virtual void Stop() = 0;
	virtual float GetTime() const = 0;
};


