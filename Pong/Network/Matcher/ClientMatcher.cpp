
#ifdef CLIENT

#include "../../Util/UtilMacro.h"
#include "ClientMatcher.h"
#include "../Config/GameConnectionConfig.h"

/// ClientMatcher is responsible from requesting token from match maker server.
ClientMatcher::ClientMatcher() : matcher(yojimbo::GetDefaultAllocator())
{
	clientId = 0;
	yojimbo::random_bytes(reinterpret_cast<uint8_t *>(&clientId), 8);
	DEBUG_CLIENT(info, "client id is %.16" PRIx64 "", clientId);
}

/// Request connection token from match maker.
bool ClientMatcher::RequestMatch()
{
	if (!matcher.Initialize())
	{
		DEBUG_CLIENT(critical, "Failed to initialize matcher");
		return false;
	}

	DEBUG_CLIENT(info, "requesting match from {0}:{1}", "https://localhost", "8080");

	matcher.RequestMatch(PROTOCOL_ID, clientId, false, "https://localhost", "8080");

	if (matcher.GetMatchStatus() == yojimbo::MATCH_FAILED)
	{
		DEBUG_CLIENT(critical,
			"Request match failed. Is the matcher running? Please run \"premake5 matcher\" before you connect a secure client");
		return false;
	}


	matcher.GetConnectToken(connectToken);
	DEBUG_CLIENT(critical, "received connect token from matcher");
	return true;
}

uint8_t* ClientMatcher::getConnectToken()
{
	return connectToken;
}

uint64_t ClientMatcher::getClientId() const
{
	return clientId;
}

#endif
