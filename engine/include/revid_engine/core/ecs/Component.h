#pragma once

#include "ComponentType.h"

namespace Revid
{
	template <ComponentType ct>
	struct Component
	{
		static ComponentType GetComponentType() { return ct; }
	};
}
