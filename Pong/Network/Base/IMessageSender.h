#pragma once

#ifdef SendMessage
#undef SendMessage
#endif

enum class GameChannel;
enum class GameMessageType;

class IMessageSender
{
public:
	virtual ~IMessageSender() = default;
#ifdef CLIENT
	virtual yojimbo::Message* CreateMessage(const GameMessageType& type) = 0;
	virtual void ReleaseMessage(yojimbo::Message* message) = 0;
	virtual bool SendMessage(const GameChannel& channel, yojimbo::Message* message) = 0;
#else
	virtual yojimbo::Message* CreateMessage(const int& clientIndex, const GameMessageType& type) = 0;
	virtual void ReleaseMessage(const int& clientIndex, yojimbo::Message* message) = 0;
	virtual bool SendMessage(const GameChannel& channel, const int& clientIndex, yojimbo::Message* message) = 0;
#endif
};
