#pragma once
#include <any>
#include <filesystem>
#include <deque>
#include <vector>
#include "../../SDK/Misc/FNV.h"
#include <windows.h>

namespace fnv_1a
{
	template< typename S >
	struct fnv_internal;
	template< typename S >
	struct fnv1a;

	template< >
	struct fnv_internal< uint32_t >
	{
		constexpr static uint32_t default_offset_basis = 0x811C9DC5;
		constexpr static uint32_t prime = 0x01000193;
	};

	template< >
	struct fnv1a< uint32_t > : public fnv_internal< uint32_t >
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

using fnv = fnv_1a::fnv1a< uint32_t >;

using FNV1A_t = std::uint32_t;
namespace FNV1A
{
	constexpr std::uint32_t ullBasis = 0x811C9DC5;
	constexpr std::uint32_t ullPrime = 0x1000193;

	constexpr FNV1A_t HashConst(const char* szString, const FNV1A_t uValue = ullBasis) noexcept
	{
		return !*szString ? uValue : HashConst(&szString[1], (uValue ^ FNV1A_t(szString[0])) * ullPrime);
	}

	inline const FNV1A_t Hash(const char* szString)
	{
		FNV1A_t uHashed = ullBasis;

		for (std::size_t i = 0U; i < strlen(szString); ++i)
		{
			uHashed ^= szString[i];
			uHashed *= ullPrime;
		}

		return uHashed;
	}
}

template <typename T, std::size_t S>
inline std::deque<T> GetFilledVector(const T& fill)
{
	std::deque<T> vecTemp(S);
	std::fill(vecTemp.begin(), vecTemp.begin() + S, fill);
	return vecTemp;
}

#define C_ADDVARIABLE( Type, szName, pDefault ) const std::uint32_t szName = C::AddVariable<Type>(FNV1A::HashConst(#szName), FNV1A::HashConst(#Type), pDefault);
#define C_ADDVARIABLE_VECTOR( Type, uSize, szName, pDefault ) const std::uint32_t szName = C::AddVariable<std::deque<Type>>(FNV1A::HashConst(#szName), FNV1A::HashConst("std::deque<" #Type ">"), GetFilledVector<Type, uSize>(pDefault));
#define C_INVALID_VARIABLE (std::size_t)-1

struct VariableObject_t
{
	VariableObject_t() = default;

	explicit VariableObject_t(const FNV1A_t	 uNameHash, const FNV1A_t	 uTypeHash, const std::any pDefault)
		: uNameHash(uNameHash), uTypeHash(uTypeHash), pValue(pDefault)
	{
	}

	~VariableObject_t() = default;

	template<typename T>
	T& Get()
	{
		return *(T*)(std::any_cast<T>(&pValue));
	}

	template<typename T>
	void Set(T value)
	{
		pValue.emplace<T>(value);
	}

	FNV1A_t			uNameHash;
	FNV1A_t			uTypeHash;
	std::any	pValue;
};

namespace C
{
	bool Setup(std::string_view szDefaultFileName);
	bool Save(std::string_view szFileName);
	bool Load(std::string_view szFileName);

	void Remove(std::string_view szFileName);
	void Refresh();

	std::size_t GetVariableIndex(const FNV1A_t	 uNameHash);
	std::filesystem::path GetWorkingPath();

	inline std::vector<VariableObject_t> vecVariables = { };

	template <typename T>
	T& Get(const std::uint32_t nIndex)
	{
		return vecVariables.at(nIndex).Get<T>();
	}

	template <typename T>
	std::uint32_t AddVariable(const FNV1A_t	 uNameHash, const FNV1A_t	 uTypeHash, const T pDefault)
	{
		vecVariables.push_back(VariableObject_t(uNameHash, uTypeHash, std::make_any<T>(pDefault)));
		return vecVariables.size() - 1U;
	}

	const std::filesystem::path fsPath = GetWorkingPath();
	inline std::deque<std::string> vecFileNames = { };
	inline std::string szConfigPath = "storm";
}