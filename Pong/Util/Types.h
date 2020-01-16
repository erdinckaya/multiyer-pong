#pragma once

#include <enum.h>

/// Game enums for different purposes.

BETTER_ENUM(Direction, int, Left, Right, Top, Bottom)

BETTER_ENUM(ColliderType, int, Rectangle, Circle)

BETTER_ENUM(MouseButtonState, int, MouseLeft, MouseRight, MouseMiddle)

BETTER_ENUM(FontType, int, Fontana, Count)

BETTER_ENUM(SpriteType, int, Background, Ball, Paddle, BallParticle, Count)

BETTER_ENUM(ShaderType, int, Text, Count)

BETTER_ENUM(NetworkObjectType, int, Server = 0, Client = 1)

BETTER_ENUM(TextId, int, PlayerPoint = 0, RivalPoint = 1, 
	Fps, Pause,
	NetworkRTT, NetworkPackageLoss, NetworkLatency, NetworkJitter, NetworkDuplicate,
	HelpTip, Help, HelpPause, HelpRestart, HelpMoveLeft, HelpMoveRight,
	HelpPackageLoss, HelpLatency, HelpJitter, HelpDuplicates,
	Colliders,
	Count = 1000)