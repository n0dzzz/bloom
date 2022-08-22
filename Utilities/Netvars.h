#pragma once

#include "Netvars.h"
#include <windows.h>
#include <SDKDDKVer.h>
#include <string.h>
#include <vadefs.h>
#include <stdio.h>
#include <xstring>
#include <Psapi.h>
#include <thread>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>
#include <time.h>
#include <winuser.h>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <array>
#include <assert.h>
#include <deque>
#include <intrin.h>
#include <atomic>
#include <mutex>
#include <stdint.h>
#include <stdlib.h>
#include <process.h>
#include <Wincon.h>
#include <cstdint>
#include <chrono>
#include <Shlobj.h>
#include <future>
#include <Lmcons.h>
#include <tchar.h>
#include "../SDK/Misc/Fnv.h"

namespace NetvarManager
{
    uintptr_t GetNetVar(uint32_t table, uint32_t prop);
}

#define NETVAR( table, prop, func_name, type ) type& func_name() { \
    static uintptr_t offset = 0; \
    if( !offset ) { \
        offset = NetvarManager::GetNetVar( Fnv::hash( table ), Fnv::hash( prop ) ); \
    } \
    return *reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
}

#define NETVAR_PTR( table, prop, func_name, type ) type* func_name() { \
    static uintptr_t offset = 0; \
    if( !offset ) { \
        offset = NetvarManager::GetNetVar( Fnv::hash( table ), Fnv::hash( prop ) ); \
    } \
    return reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
}

#define OFFSET( type, var, offset ) type& var() { \
    return *( type* )( uintptr_t( this ) + offset ); \
}