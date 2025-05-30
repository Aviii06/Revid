#pragma once
#include <core/ecs/System.h>
#include <editor/camera/Camera.h>

namespace Revid
{
	class RenderSystem : public System
	{
		void Update(Camera* camera);
	};
}

