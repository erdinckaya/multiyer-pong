
#ifdef CLIENT
#include "../../Network/Data/PauseCommandMessage.h"
#include "../../Network/NetworkManager.h"
#include "../Events/RemoveTextEvent.h"

#include "../Events/ChangeTextEvent.h"
#include "../../Network/Data/RestartCommandMessage.h"


#include "../Events/PauseEvent.h"
#include "../Events/AddPlayerEvent.h"

#include "../../Input/KeyInput.h"



#include "ClientGameManagerSystem.h"
#include "../EntityFactory.h"
#include "../../Graphics/GraphicsManager.h"
#include "../../Network/Config/GameMessageType.h"
#include "../../Network/Config/GameChannel.h"

#ifdef SendMessage
#undef SendMessage
#endif

#ifdef EDITOR
#include "../../Editor/Editor.h"
#endif

ClientGameManagerSystem::ClientGameManagerSystem() : entities(nullptr), events(nullptr), isHelpMenuOpen(false), isNetworkMenuOpened(false)
{
}

void ClientGameManagerSystem::update(EntityManager& entities, EventManager& events, TimeDelta dt)
{
	// Handle pause.
	HandlePauseAndRestart();
	// Handle Open Menu.
	HandleHelpMenu();
	// Update network panel.
	UpdateNetworkPanel();
	// Handle debug inputs.
	DebugNetwork();
}

void ClientGameManagerSystem::configure(EntityManager& entities, EventManager& events)
{
	this->entities = &entities;
	this->events = &events;

	events.subscribe<AddPlayerEvent>(*this);
	events.subscribe<StartGameEvent>(*this);
	events.subscribe<PauseEvent>(*this);
	events.subscribe<RestartEvent>(*this);
}

void ClientGameManagerSystem::receive(const AddPlayerEvent& addPlayerEvent)
{
	if (!addPlayerEvent.owner)
	{
		/// Change titles both editor and game.
		char clientTitle[30];
		char editorTitle[30];
		sprintf_s(clientTitle, 30, "Player_%d", addPlayerEvent.clientIndex);
		GraphicsManager::Instance.GetGraphics()->SetWindowTitle(clientTitle);
#ifdef EDITOR
		sprintf_s(editorTitle, 30, "Player_%d Editor", addPlayerEvent.clientIndex);
		Editor::Instance.SetWindowTitle(editorTitle);
#endif
		GraphicsManager::Instance.GetGraphics()->SetWindowDirection(
			Direction::_from_integral(addPlayerEvent.clientIndex));
	}

	// Create components and activate them.
	EntityFactory::AddPaddle(addPlayerEvent.entity, addPlayerEvent.owner);
}

void ClientGameManagerSystem::receive(const StartGameEvent& startGameEvent)
{
	// Create network panel.
	CreateNetworkPanel();
	// Create ball.
	EntityFactory::CreateBall();
}

void ClientGameManagerSystem::receive(const PauseEvent& pauseEvent)
{
	const auto pauseMessage = dynamic_cast<PauseCommandMessage*>(pauseEvent.message);
	World::Instance.PauseWorld(pauseMessage->value);
	events->emit<ChangeTextEvent>(TextId::Pause, "Pause: " + BOOL_TO_STR(World::Instance.IsWorldPaused()));
}

void ClientGameManagerSystem::receive(const RestartEvent& restartEvent)
{
	Restart();
}

void ClientGameManagerSystem::HandlePauseAndRestart()
{
	// Check for pause.
	if (KeyInput::Instance.WasKeyReleased(SDL_SCANCODE_P))
	{
		const auto pauseMessage = dynamic_cast<PauseCommandMessage*>(NetworkManager::Instance
			.network->CreateMessage(GameMessageType::PAUSE));
		pauseMessage->clientId = NetworkManager::Instance.network->GetClientId();
		NetworkManager::Instance.network->SendMessage(GameChannel::RELIABLE, pauseMessage);
	}
	// Check for restart.
	else if (KeyInput::Instance.WasKeyReleased(SDL_SCANCODE_R))
	{
		const auto restartMessage = dynamic_cast<RestartCommandMessage*>(NetworkManager::Instance
			.network->CreateMessage(GameMessageType::RESTART)
			);
		restartMessage->clientId = NetworkManager::Instance.network->GetClientId();
		NetworkManager::Instance.network->SendMessage(GameChannel::RELIABLE, restartMessage);
	}
}

void ClientGameManagerSystem::Restart() const
{
	entities->each<Paddle, Translation, Move, Size, Scale, PlayerMoveHistory>(
		[](ex::Entity entity, Paddle& paddle, Translation& translation, Move& move, Size& size, Scale& scale,
		   PlayerMoveHistory& playerMoveHistory)
		{
			translation.value.x = PLAYER_START_X;
			size.value = float2(PADDLE_DEFAULT_SIZE_X, PADDLE_DEFAULT_SIZE_Y);
			move.state = 0;
			move.direction = float2(0, 0);
			move.speed = PLAYER_DEFAULT_SPEED;
			scale.value = float2(1, 1);
			playerMoveHistory.queue.clear();
		});

	entities->each<Ball, Translation, Move, Size, Scale>(
		[](ex::Entity entity, Ball& ball, Translation& translation, Move& move, Size& size, Scale& scale)
		{
			translation.value = float2(BALL_START_X, BALL_START_Y);
			size.value = float2(BALL_RADIUS * 2.f, BALL_RADIUS * 2.f);
			move.state = 0;
			move.direction = float2(0.0f, -1.0f);
			move.speed = BALL_DEFAULT_SPEED;
			scale.value = float2(1, 1);
		});
}

void ClientGameManagerSystem::HandleHelpMenu()
{
	const auto totalTextCount = 10;
	// Open help menu.
	if (KeyInput::Instance.WasKeyPressed(SDL_SCANCODE_H) && !isHelpMenuOpen)
	{
		DEBUG_LOG("Add Help");

		isHelpMenuOpen = true;
		// Prepare help menu texts.
		const auto fontSize = 25;
		const auto font = FontType::Fontana;
		const auto color = Color{150.0f / 255.0f, 143 / 255.0f, 1.0f, 1.0f};

		const char* helpMenuTexts[totalTextCount] = {
			"                  HELP",
			"PAUSE           : P",
			"RESTART        : R",
			"MOVING LEFT   : LEFT ARROW",
			"MOVING RIGHT : RIGHT ARROW",
			"PACKAGE LOSS : [G, SHIFT + G] +-0.1%",
			"LATENCY        : [L, SHIFT + L] +-10ms",
			"JITTER           : [J, SHIFT + J] +-10ms",
			"DUPLICATES    : [D, SHIFT + D] +-0.1%",
			"COLLIDERS      : C",
		};

		// Calculate offsets.
		float xOffset = 0;
		for (int i = 0; i < totalTextCount; ++i)
		{
			const auto textWidth = GraphicsManager::Instance.GetGraphics()->CalculateTextWidth(
				helpMenuTexts[i], font, fontSize);
			xOffset = std::max(xOffset, textWidth);
		}


		// Create text entities.
		const float yTopOffset = totalTextCount * fontSize * 0.5f;
		xOffset *= -0.5f;
		float2 pos = {xOffset, yTopOffset};
		for (int i = 0; i < totalTextCount; ++i)
		{
			EntityFactory::CreateText(TextId::_from_integral(TextId::Help + i), helpMenuTexts[i], font, fontSize, pos,
			                          i == 0 ? Color{0, 1, 0, 1} : color);
			pos.y -= fontSize;
		}
	}
	// If key released remove help panel.
	else if (KeyInput::Instance.WasKeyReleased(SDL_SCANCODE_H) && isHelpMenuOpen)
	{
		DEBUG_LOG("Remove Help");
		isHelpMenuOpen = false;
		for (int i = 0; i < totalTextCount; ++i)
		{
			events->emit<RemoveTextEvent>(TextId::Help + i);
		}
	}
}
void ClientGameManagerSystem::CreateNetworkPanel()
{
	// Prepare texts.
	const auto totalNetworkString = 5;
	const char* networkMenuTexts[totalNetworkString] = {
		"RTT: 0ms",
		"PackageLoss: 0%",
		"Latency: 0ms",
		"Jitter: 0ms",
		"Duplicates: 0%"
	};

	// Calculate offsets.
	const auto fontSize = 25;
	const float yTopOffset = CLIENT_WINDOW_HEIGHT * 0.5f - 125.0f;
	float2 pos = {-CLIENT_WINDOW_WIDTH * 0.5f, yTopOffset};

	const auto font = FontType::Fontana;
	const auto color = Color{0.0f, 1.0f, 1.0f, 1.0f};

	// Create text entities.
	for (int i = 0; i < totalNetworkString; ++i)
	{
		EntityFactory::CreateText(TextId::_from_integral(TextId::NetworkRTT + i), networkMenuTexts[i], font, fontSize,
		                          pos, color);
		pos.y -= fontSize;
	}
	isNetworkMenuOpened = true;
}

void ClientGameManagerSystem::UpdateNetworkPanel() const
{
	if (!isNetworkMenuOpened)
	{
		return;
	}
	// Get network info.
	yojimbo::NetworkInfo networkInfo;
	NetworkManager::Instance.network->GetNetworkInfo(networkInfo);
	std::ostringstream stream;

	// Update rtt text
	stream << "RTT: " << networkInfo.RTT << "ms";
	events->emit<ChangeTextEvent>(TextId::NetworkRTT, stream.str());
	stream.str("");
	stream.clear();

	// Update package loss text
	stream << "PackageLoss: %" << networkInfo.packetLoss;
	events->emit<ChangeTextEvent>(TextId::NetworkPackageLoss, stream.str());
	stream.str("");
	stream.clear();

	// Update latency text
	stream << "Latency: " << NetworkManager::Instance.network->networkDebug.latency <<"ms";
	events->emit<ChangeTextEvent>(TextId::NetworkLatency, stream.str());
	stream.str("");
	stream.clear();

	// Update jitter text
	stream << "Jitter: " << NetworkManager::Instance.network->networkDebug.jitter << "ms";
	events->emit<ChangeTextEvent>(TextId::NetworkJitter, stream.str());
	stream.str("");
	stream.clear();

	// Update dupicate packages text
	stream << "Duplicates: %" << NetworkManager::Instance.network->networkDebug.duplicates;
	events->emit<ChangeTextEvent>(TextId::NetworkDuplicate, stream.str());
	stream.str("");
	stream.clear();
}

void ClientGameManagerSystem::DebugNetwork() const
{
	// Check for package loss
	if (KeyInput::Instance.WasKeyReleased(SDL_SCANCODE_G))
	{
		auto value = 10.0f;
		// Check for decrease
		if (KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_LSHIFT) || KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_RSHIFT))
		{
			value *= -1.0f;
		}

		// Update it.
		yojimbo::NetworkInfo networkInfo;
		NetworkManager::Instance.network->GetNetworkInfo(networkInfo);
		networkInfo.packetLoss += value;
		CLAMP(networkInfo.packetLoss, 0.0f, 100.0f);
		NetworkManager::Instance.network->SetPacketLoss(networkInfo.packetLoss);
		return;
	}

	// Check for latency
	if (KeyInput::Instance.WasKeyReleased(SDL_SCANCODE_L))
	{
		auto value = 10.0f;
		// Check for decrease
		if (KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_LSHIFT) || KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_RSHIFT))
		{
			value *= -1.0f;
		}

		NetworkManager::Instance.network->networkDebug.latency += value;
		// Update it.
		CLAMP(NetworkManager::Instance.network->networkDebug.latency, 0.0f, 10000.0f);
		NetworkManager::Instance.network->SetLatency(NetworkManager::Instance.network->networkDebug.latency);
		return;
	}

	// Check for jitter
	if (KeyInput::Instance.WasKeyReleased(SDL_SCANCODE_J))
	{
		auto value = 10.0f;
		// Check for decrease
		if (KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_LSHIFT) || KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_RSHIFT))
		{
			value *= -1.0f;
		}

		NetworkManager::Instance.network->networkDebug.jitter += value;
		// Update it.
		CLAMP(NetworkManager::Instance.network->networkDebug.jitter, 0.0f, 10000.0f);
		NetworkManager::Instance.network->SetJitter(NetworkManager::Instance.network->networkDebug.jitter);
		return;
	}

	// Check for duplicates
	if (KeyInput::Instance.WasKeyReleased(SDL_SCANCODE_D))
	{
		auto value = 10.0f;
		// Check for decrease
		if (KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_LSHIFT) || KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_RSHIFT))
		{
			value *= -1.0f;
		}

		NetworkManager::Instance.network->networkDebug.duplicates += value;
		// Update it.
		CLAMP(NetworkManager::Instance.network->networkDebug.duplicates, 0.0f, 100.0f);
		NetworkManager::Instance.network->SetDuplicates(NetworkManager::Instance.network->networkDebug.duplicates);
		return;
	}
}

#endif
