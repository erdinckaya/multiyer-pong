#pragma once

/// Simple network debug data.
struct NetworkDebug
{
	explicit NetworkDebug() : latency(0.0f), jitter(0.0f), duplicates(0.0f)
	{
	}

	float latency;
	float jitter;
	float duplicates;
};
