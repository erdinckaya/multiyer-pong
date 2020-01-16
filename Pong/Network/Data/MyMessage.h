#pragma once

#include <yojimbo.h>

// Test message.
class MyMessage : public yojimbo::Message
{
public:
	int m_data;
	char text[100];

	MyMessage() : m_data(0)
	{
	}

	template <typename Stream>
	bool Serialize(Stream& stream)
	{
		yojimbo_serialize_int(stream, m_data, 0, 512);
		yojimbo_serialize_string(stream, text, 512);
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
