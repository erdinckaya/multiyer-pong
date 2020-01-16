#pragma once
#include <linalg.h>
#include "../../Editor/IReflect.h"

/// Keeps final render values of shape.
struct RenderTarget ADD_REFLECTION
{
	explicit RenderTarget(const linalg::aliases::float4& value = {0, 0, 0, 0})
	{
	}

	linalg::aliases::float4 value;

	START_REFLECTION(RenderTarget)
		ADD_REFLECTION_FIELD(value, "float4")
		END_REFLECTION
};
