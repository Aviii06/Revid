#pragma once
#include <revid_engine/core/ecs/systems/RenderSystem.h>
#include <revid_engine/core/ecs/System.h>

namespace Revid
{
	class RenderSystem : public System
	{
		void Update(float dt);
	};
}

