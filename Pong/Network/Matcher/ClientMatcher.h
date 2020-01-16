#pragma once


#ifdef CLIENT

#include <yojimbo.h>

class ClientMatcher {
public:

	ClientMatcher();

	bool RequestMatch();

	uint8_t *getConnectToken();

	uint64_t getClientId() const;

private:
	yojimbo::Matcher matcher;
	uint8_t connectToken[yojimbo::ConnectTokenBytes];
	uint64_t clientId;
};

#endif