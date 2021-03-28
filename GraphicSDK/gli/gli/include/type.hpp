/// @brief Include to use basic GLI types.
/// @file gli/type.hpp

#pragma once

// STD
#include <cstddef>

// GLM
#define GLM_FORCE_EXPLICIT_CTOR
#include <glm/glm.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/gtx/std_based_type.hpp>

#if GLM_COMPILER & GLM_COMPILER_VC
#	define GLI_FORCE_INLINE __forceinline
#elif GLM_COMPILER & (GLM_COMPILER_GCC | GLM_COMPILER_APPLE_CLANG | GLM_COMPILER_LLVM)
#	define GLI_FORCE_INLINE inline __attribute__((__always_inline__))
#else
#	define GLI_FORCE_INLINE inline
#endif//GLM_COMPILER

#define GLI_DISABLE_ASSERT 0

#if defined(NDEBUG) || GLI_DISABLE_ASSERT
#	define GLI_ASSERT(test)
#else
#	define GLI_ASSERT(test) assert((test))
#endif

namespace gli
{
	using namespace glm;

	using std::size_t;

	typedef tvec1<int> extent1d;
	typedef tvec2<int> extent2d;
	typedef tvec3<int> extent3d;
	typedef tvec4<int> extent4d;

	template <typename T, precision P>
	inline tvec4<T, P> make_vec4(tvec1<T, P> const & v)
	{
		return tvec4<T, P>(v.x, static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
	}

	template <typename T, precision P>
	inline tvec4<T, P> make_vec4(tvec2<T, P> const & v)
	{
		return tvec4<T, P>(v.x, v.y, static_cast<T>(0), static_cast<T>(1));
	}

	template <typename T, precision P>
	inline tvec4<T, P> make_vec4(tvec3<T, P> const & v)
	{
		return tvec4<T, P>(v.x, v.y, v.z, static_cast<T>(1));
	}

	template <typename T, precision P>
	inline tvec4<T, P> make_vec4(tvec4<T, P> const & v)
	{
		return v;
	}
}//namespace gli
