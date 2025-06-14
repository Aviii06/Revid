#pragma once
#include <bitset>


namespace Revid
{
	using ComponentId = uint32_t;
	constexpr size_t MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;


}