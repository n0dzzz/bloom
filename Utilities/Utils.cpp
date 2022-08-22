#include "Utils.h"

#include <vector>

namespace Utils
{
	void AttachConsole(const char* name)
	{
		FILE* file;
		AllocConsole();

		freopen_s(&file, "CONIN$", "r", stdin);
		freopen_s(&file, "CONOUT$", "w", stderr);
		freopen_s(&file, "CONOUT$", "w", stdout);

		SetConsoleTitleA(name);
	}

	void DetachConsole()
	{
		SetStdHandle(STD_OUTPUT_HANDLE, NULL);
		SetStdHandle(STD_ERROR_HANDLE, NULL);
		SetStdHandle(STD_INPUT_HANDLE, NULL);

		fclose(static_cast<_iobuf*>(__acrt_iob_func(0)));
		fclose(static_cast<_iobuf*>(__acrt_iob_func(1)));
		fclose(static_cast<_iobuf*>(__acrt_iob_func(2)));

		FreeConsole();
	}

	void ConsolePrint(const char* msg, int msgType)
	{
		switch (msgType)
		{
		case 0:
			std::cout << msg << std::endl;
			break;
		case 1:
			std::cout << yellow << msg << std::endl;
			break;
		case 2:
			std::cout << red << msg << std::endl;
			break;
		case 3:
			std::cout << green << msg << std::endl;
			break;
		default:
			std::cout << msg << std::endl;
			break;
		}
	}

	std::uint8_t* PatternScan(const char* module, const char* signature)
	{
		const auto module_handle = GetModuleHandleA(module);

		if (!module_handle)
			return nullptr;

		static auto pattern_to_byte = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + std::strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;

					if (*current == '?')
						++current;

					bytes.push_back(-1);
				}
				else
				{
					bytes.push_back(std::strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
		auto nt_headers =
			reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

		auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
		auto pattern_bytes = pattern_to_byte(signature);
		auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

		auto s = pattern_bytes.size();
		auto d = pattern_bytes.data();

		for (auto i = 0ul; i < size_of_image - s; ++i)
		{
			bool found = true;

			for (auto j = 0ul; j < s; ++j)
			{
				if (scan_bytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}
			if (found)
				return &scan_bytes[i];
		}

		throw std::runtime_error(std::string("Incorrect Sig: ") + signature);
	}
}