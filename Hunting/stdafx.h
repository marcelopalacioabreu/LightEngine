// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


// TODO: reference additional headers your program requires here
#include <ISubSystem.h>
#include <IContext.h>

#include <IResourceManager.h>
#include <Renderer.h>
#include <IGamePlugin.h>

#include <Mesh.h>
extern "C" __declspec(dllexport) IGamePlugin* CreateInterface();