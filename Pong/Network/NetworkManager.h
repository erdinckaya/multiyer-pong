#pragma once
#include <memory>


#include "Base/INetwork.h"

/// NetworkManager is responsible from all network staff.
/// If config is client it keeps yojimbo client otherwise it keeps yojimbo server.
class NetworkManager
{
public:
	static NetworkManager Instance;

	
	NetworkManager();
	~NetworkManager();
	void CommitSuicide();

	std::unique_ptr<INetwork> network;
};


