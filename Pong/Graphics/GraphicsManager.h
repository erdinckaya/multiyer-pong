#pragma once
#include "Graphics.h"


/// Graphics manager is responsible for storing graphics
/// and managing its life time.
class GraphicsManager
{
public:
	GraphicsManager();

	static GraphicsManager Instance;
	void Init();

	Graphics* GetGraphics() const;

	bool Ready() const;

private:
	std::unique_ptr<Graphics> graphics;
	bool ready;
};
