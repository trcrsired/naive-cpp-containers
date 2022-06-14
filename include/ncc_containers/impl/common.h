#pragma once
#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning( disable : 4514 )
#pragma warning( disable : 4668 )
#pragma warning( disable : 4710 )
#pragma warning( disable : 5045 )
#endif
// Only Allow include freestanding C++ headers
#include<cstddef>
#include<cstdlib>
#include<type_traits>
#include<limits>
#include<concepts>
#include<cstdint>
#include<new>

#include"utils.h"
#include"concepts.h"
#include"allocators/impl.h"
#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif
