#pragma once
#include "ReflectObject.h"

// Simple reflection class.
// In order to monitor entities component via using IMGUI.

#ifdef EDITOR
#define ADD_REFLECTION : public IReflect

#define START_REFLECTION(obj_name) \
	ReflectObject Reflect() override \
	{\
		ReflectObject obj;\
		obj.name = #obj_name ;\

#define ADD_REFLECTION_FIELD(var, type) obj.fields.emplace_back(#var, (type), &var);\

#define ADD_REFLECTION_OBJECT(var) obj.fields.emplace_back(#var, "struct", &var);\

#define END_REFLECTION \
	return obj;\
}\
	
#else
#define ADD_REFLECTION
#define START_REFLECTION(obj_name)
#define ADD_REFLECTION_FIELD(var, type)
#define ADD_REFLECTION_OBJECT(var)
#define END_REFLECTION
#endif

struct IReflect
{
	virtual ~IReflect() = default;
	virtual ReflectObject Reflect() = 0;
};
