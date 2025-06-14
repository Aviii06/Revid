#pragma once
#include <revid_engine/ServiceLocater.h>

#include "maths/Vec.h"
#include <revid_engine/core/ecs/components/TransformComponent.h>
#include "revid_engine/core/ecs/Registry.h"

namespace Revid
{
	class TransformSystem: public System
	{
	public:
		static Signature GetSignature()
        {
            Signature signature;
            signature.set((size_t)ServiceLocator::GetComponentManager()->GetComponentType<TransformComponent>());
            return signature;
        }
	};
}