#include "NetworkManager.h"


#ifdef SERVER
#include "GameServer.h"
#else
#include "GameClient.h"
#endif

/// Singleton instance for network manager.
NetworkManager NetworkManager::Instance;

/// Logger function that overloads yojimbo printf.
int log(const char * format, ...)
{
	char buffer[4 * 1024];
	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	va_end(args);
	const int length = strlen(buffer);
	if (buffer[length - 1] == '\n')
	{
		buffer[length - 1] = '\0';
	}
	spdlog::info("[Yojimbo] {}", buffer);
	return 0;
}


NetworkManager::NetworkManager()
{
	// Init yojimbo
	CHECK_ERROR_EXIT(!InitializeYojimbo(), "error: failed to initialize Yojimbo!\n");
	// Set log level.
	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);
	// Override yojimbo printf
	yojimbo_set_printf_function(log);

	// Init network.
#ifdef SERVER
	network = std::make_unique<GameServer>(yojimbo::Address(SERVER_ADDRESS, SERVER_PORT));
#else
	network = std::make_unique<GameClient>();
#endif

}


NetworkManager::~NetworkManager() = default;

// Disconnect from yojimbo.
void NetworkManager::CommitSuicide()
{
	// Release object and close yojimbo.
	network.reset();
	ShutdownYojimbo();
}
