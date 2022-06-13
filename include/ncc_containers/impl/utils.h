#pragma once

#if defined(_MSC_VER)&&!defined(__clang__)
#include<intrin.h>
#endif

namespace ncc::details
{

[[noreturn]] inline void fast_terminate() noexcept
{
//https://llvm.org/doxygen/Compiler_8h_source.html
#if defined(__has_builtin)
#if __has_builtin(__builtin_trap)
	__builtin_trap();
#elif __has_builtin(__builtin_abort)
	__builtin_abort();
#else
	std::abort();
#endif
#elif defined(_MSC_VER)&&!defined(__clang__)
	__fastfail(1);
#else
	std::abort();
#endif
}

template<typename T>
#if __has_cpp_attribute(__gnu__::__always_inline__)
[[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]] inline constexpr T&& forward(std::remove_reference_t<T>& t) noexcept
{
	return static_cast<T&&>(t);
}

template<typename T>
#if __has_cpp_attribute(__gnu__::__always_inline__)
[[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]] inline constexpr typename std::remove_reference<T>::type&& move(T&& t) noexcept
{
	return static_cast<typename std::remove_reference<T>::type&&>(t);
}


template<typename R, typename ...Args>
struct make_noexcept
{};

template<typename R, typename ...Args>
struct make_noexcept<R(Args...)> { using type = R(Args...) noexcept; };

template<typename R, typename ...Args>
struct make_noexcept<R(Args...) noexcept> { using type = R(Args...) noexcept; };

template<typename R, typename ...Args>
using make_noexcept_t = typename make_noexcept<R,Args...>::type;

template<typename F,typename... Args>
requires std::is_function_v<F>
#if __has_cpp_attribute(__gnu__::__always_inline__)
[[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
inline
#if __cpp_if_consteval >= 202106L || __cpp_lib_is_constant_evaluated >= 201811
constexpr
#endif
decltype(auto) noexcept_call(F* f,Args&& ...args) noexcept
{
#if __cpp_if_consteval >= 202106L || __cpp_lib_is_constant_evaluated >= 201811
#if __cpp_if_consteval >= 202106L
	if consteval
#else
	if (__builtin_is_constant_evaluated())
#endif
	{
		return f(::ncc::details::forward<Args>(args)...);		//EH unwinding does not matter here
	}
	else
#endif
	{
		return reinterpret_cast<make_noexcept_t<F>*>(f)(::ncc::details::forward<Args>(args)...);
	}
}

}
