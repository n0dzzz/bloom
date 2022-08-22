#pragma once
#include <string>

namespace fnv_1a
{
	template< typename S >
	struct Fnv_internal;
	template< typename S >
	struct Fnv1a;

	template< >
	struct Fnv_internal< uint32_t >
	{
		constexpr static uint32_t default_offset_basis = 0x811C9DC5;
		constexpr static uint32_t prime = 0x01000193;
	};

	template< >
	struct Fnv1a< uint32_t > : public Fnv_internal< uint32_t >
	{
		constexpr static uint32_t hash(char const* string, const uint32_t val = default_offset_basis)
		{
			return (string[0] == '\0')
				? val
				: hash(&string[1], (val ^ uint32_t(string[0])) * prime);
		}

		constexpr static uint32_t hash(wchar_t const* string, const uint32_t val = default_offset_basis)
		{
			return (string[0] == L'\0')
				? val
				: hash(&string[1], (val ^ uint32_t(string[0])) * prime);
		}
	};
}

using Fnv = fnv_1a::Fnv1a< uint32_t >;