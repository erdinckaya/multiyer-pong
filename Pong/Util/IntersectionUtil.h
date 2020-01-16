#pragma once
#include <linalg.h>


using namespace linalg::aliases;

/// Static Util class for calculating intersections between colliders.
class IntersectionUtil
{
public:


	static bool CircleToRect(float2 circle, float radius, float4 rect)
	{
		const auto halfW = rect.w * 0.5f;
		const auto halfH = rect.z * 0.5f;
		if (circle.y - radius > rect.y + halfH) return false;
		if (circle.y + radius < rect.y - halfH) return false;
		if (circle.x + radius < rect.x - halfW) return false;
		if (circle.x - radius > rect.x + halfW) return false;
		const auto distSq = distance2(circle, float2(rect.x, rect.y));
		if (distSq > halfW * halfW + halfH * halfH + radius * radius) return false;
		return true;
	}

	static bool RectToRect(float4 first, float4 second, float4& intersect)
	{
		const auto x = std::max(first.x, second.x);
		const auto w = std::min(first.x + first.w, second.x + second.w);
		const auto y = std::max(first.y, second.y);
		const auto h = std::min(first.y + first.z, second.y + second.z);
		if (w >= x && h >= y)
		{
			intersect.x = x;
			intersect.y = y;
			intersect.w = w - x;
			intersect.z = h - y;
			return true;
		}
		return false;
	}

	static bool CircleToCircle(float2 c1, float r1, float2 c2, float r2)
	{
		const auto dist = distance2(c1, c2);
		const float minDistSq = (r1 + r2) * (r1 + r2);
		return dist <= minDistSq;
	}
};
