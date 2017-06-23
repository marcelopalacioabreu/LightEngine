#pragma once

// Precompile header file
// Thai Nhat Minh 
// Game 3D Project
// Day begin 31/52017

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <Windows.h>
#include <wchar.h>
#include <cassert>
#include <iomanip>
#include <set>
#include <iterator>
#include <algorithm>
#include <list>
#include <strstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
#pragma warning(disable: 4244) 
// OpenGL Include
// GLEW
#define GLEW_STATIC
#include <gl\glew.h>
// GLFW
#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\glfw3native.h>
// TinyXML 2
#include <tinyxml2.h>

// Direct Input
#include <dinput.h>

// Bullet Physic 
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>


#include <FastDelegate\FastDelegate.h>
using fastdelegate::MakeDelegate;

// Lua Plus 5.1
#include <LuaPlus.h>

// libary
#pragma comment(lib, "lib/glew32s.lib")
#pragma comment(lib, "lib/glfw3.lib")
#pragma comment(lib, "lib/Devil.lib")
#pragma comment(lib, "lib/tinyxml2.lib")
#pragma comment(lib, "lib/dinput8.lib")
#pragma comment(lib, "lib/dxguid.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "BulletCollision_Debug.lib")
#pragma comment(lib, "BulletDynamics_Debug.lib")
#pragma comment(lib, "LinearMath_Debug.lib")
#pragma comment(lib, "lib/luaplus51-1201.lib")
/*
// Math
#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"
#include "Math\EulerAngles.h"
#include "Math\CMath.h"

#include "Math\AABB.h"
#include "Math\Plane.h"
#include "Math\Frustum.h"
#include "Math\Sphere.h"
*/


// Math
#include "glm\mat4x4.hpp"
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"
#include "glm\vec2.hpp"

#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\constants.hpp"

#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\compatibility.hpp"
#include <glm\gtx\euler_angles.hpp>
#include "glm\geometric.hpp"
#include "glm\detail\func_matrix.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::quat;

#include "Math\AABB.h"
#include "Math\Plane.h"
#include "Math\Frustum.h"

// Interface
#include "interface.h"

#include "Utilities\Module.h"
#include "Utilities\Log.h"
#include "Utilities\Input.h"
#include "Utilities\DirectInput.h"
#include "Utilities\Shader.h"
#include "Utilities\Timer.h"
#include "Utilities\String.h"
#include "Utilities\Template.h"
#include "Utilities\Debug.h"

#include "ResourceCache\ModelCache.h"
#include "ResourceCache\Texture.h"
#include "ResourceCache\ResourceLoader.h"



#include "Graphics3D\Material.h"
#include "Graphics3D\Mesh.h"
#include "Graphics3D\SkeMesh.h"
#include "Graphics3D\AnimationDef.h"
//#include "Graphics3D\SkeAnimation.h"
#include "Graphics3D\Light.h"
#include "Graphics3D\TexShader.h"
#include "Graphics3D\PrimitiveShader.h"
#include "Graphics3D\SkeShader.h"

#include "Utilities\PrimitiveShape\Cube.h"
#include "Utilities\PrimitiveShape\Line.h"

// Lua Scripting
#include "LuaScripting\LuaStateManager.h"
#include "LuaScripting\ScriptEvents.h"
#include "LuaScripting\ScriptExports.h"
#include "EventManager\TestScriptEvent.h"

#include "EventManager\EventManager.h"
#include "EventManager\AnimationEvent.h"
#include "EventManager\PhysicsEvents.h"
#include "EventManager\Events.h"

#include "GameObject\ActorComponent.h"
#include "GameObject\TransformComponent.h"
#include "GameObject\MeshRenderComponent.h"
#include "GameObject\PhysicComponent.h"
#include "GameObject\AnimationComponent.h"
#include "GameObject\CharacterControllerComponent.h"
#include "GameObject\ScriptComponent.h"
#include "GameObject\ActorFactory.h"
#include "GameObject\Actor.h"

#include "Physic\PhysicDebugDrawer.h"
#include "Physic\Physic.h"

#include "Graphics3D\Scene.h"

#include "Core\Camera.h"
#include "Core\Windows.h"
#include "Core\CoreApplication.h"