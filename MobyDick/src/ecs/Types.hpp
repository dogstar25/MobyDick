#pragma once

#include <bitset>
#include <cstdint>


constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
	return fnv1a_32(s, count);
}


// ECS
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
const std::uint32_t MAX_IDENTITY_TAGS =	32;
using Signature = std::bitset<MAX_COMPONENTS>;
using IdentityTags = std::bitset<MAX_IDENTITY_TAGS>;

