#pragma once
#include <yojimbo.h>
#include "NetworkDebug.h"

/// Abstract network debugger class to debug or monitor network.
class INetworkDebugger
{
public:

	NetworkDebug networkDebug;
#ifdef CLIENT
	virtual void GetNetworkInfo(yojimbo::NetworkInfo& networkInfo) const = 0;
#else
	virtual void GetNetworkInfo(int clientIndex, yojimbo::NetworkInfo& networkInfo) const = 0;
#endif


	virtual void SetLatency(float milliseconds) = 0;

	virtual void SetJitter(float milliseconds) = 0;

	virtual void SetPacketLoss(float percent) = 0;

	virtual void SetDuplicates(float percent) = 0;
};


