#pragma once


#include <yojimbo.h>
#include "IMessageSender.h"
#include "INetworkDebugger.h"
#include "INetworkClientBase.h"
#include "INetworkBase.h"
#include "INetworkServerBase.h"

/// Abstract network class for server and client
class INetwork : public virtual IMessageSender, public virtual INetworkDebugger, public virtual INetworkBase,
#ifdef SERVER
				 public virtual INetworkServerBase
#else
                 public virtual INetworkClientBase
#endif
{
};
