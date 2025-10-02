#pragma once

#include "revid_engine/core/ecs/Component.h"
#include "types/Containers.h"
#include "types/SmartPointers.h"
#include "revid_engine/core/renderer/Mesh.h"

namespace Revid
{
	struct ModelComponent
	{
		Vector<Ref<Mesh>> m_meshes;
	};
}
