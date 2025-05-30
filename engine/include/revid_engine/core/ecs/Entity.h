#pragma once
#include "ComponentType.h"

#include <common/types/Types.h>

namespace Revid
{
	using EntityID = uint32_t;

	class Entity
	{
	private:
		EntityID m_id;
		Signature m_signature;

	public:
		Entity() = default;
		Entity(int id, int signature);

		constexpr uint32_t GetID() const { return m_id; }
		constexpr Signature GetSignature() const { return m_signature; }

		void SetSignature(Signature signature) { m_signature = signature; }
	};

	constexpr size_t MAX_ENTITIES = 5000;
}
