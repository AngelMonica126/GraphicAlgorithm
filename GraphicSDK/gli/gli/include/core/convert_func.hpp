#pragma once

#include "../type.hpp"
#include "../texture1d.hpp"
#include "../texture1d_array.hpp"
#include "../texture2d.hpp"
#include "../texture2d_array.hpp"
#include "../texture3d.hpp"
#include "../texture_cube.hpp"
#include "../texture_cube_array.hpp"
#include <glm/gtc/packing.hpp>
#include <glm/gtc/color_space.hpp>
#include <limits>

namespace gli{
namespace detail
{
	enum convertMode
	{
		CONVERT_MODE_DEFAULT,
		CONVERT_MODE_CAST,
		CONVERT_MODE_NORM,
		CONVERT_MODE_SRGB,
		CONVERT_MODE_HALF,
		CONVERT_MODE_RGB9E5,
		CONVERT_MODE_RG11B10F,
		CONVERT_MODE_RGB10A2UNORM,
		CONVERT_MODE_RGB10A2SNORM,
		CONVERT_MODE_RGB10A2USCALE,
		CONVERT_MODE_RGB10A2SSCALE,
		CONVERT_MODE_RGB10A2UINT,
		CONVERT_MODE_RGB10A2SINT,
		CONVERT_MODE_44UNORM,
		CONVERT_MODE_44SCALED,
		CONVERT_MODE_4444UNORM,
		CONVERT_MODE_4444SCALED,
		CONVERT_MODE_565UNORM,
		CONVERT_MODE_565SCALED,
		CONVERT_MODE_5551UNORM,
		CONVERT_MODE_5551SCALED,
		CONVERT_MODE_332UNORM
	};

	template <typename textureType, typename genType>
	struct accessFunc
	{};

	template <typename genType>
	struct accessFunc<texture1d, genType>
	{
		static genType load(texture1d const & Texture, texture1d::extent_type const & TexelCoord, texture1d::size_type Layer, texture1d::size_type Face, texture1d::size_type Level)
		{
			GLI_ASSERT(Layer == 0 && Face == 0);
			return Texture.load<genType>(TexelCoord, Level);
		}

		static void store(texture1d & Texture, texture1d::extent_type const & TexelCoord, texture1d::size_type Layer, texture1d::size_type Face, texture1d::size_type Level, genType const & Texel)
		{
			GLI_ASSERT(Layer == 0 && Face == 0);
			Texture.store<genType>(TexelCoord, Level, Texel);
		}
	};

	template <typename genType>
	struct accessFunc<texture1d_array, genType>
	{
		static genType load(texture1d_array const& Texture, texture1d_array::extent_type const& TexelCoord, texture1d_array::size_type Layer, texture1d_array::size_type Face, texture1d_array::size_type Level)
		{
			GLI_ASSERT(Face == 0);
			return Texture.load<genType>(TexelCoord, Layer, Level);
		}

		static void store(texture1d_array& Texture, texture1d_array::extent_type const& TexelCoord, texture1d_array::size_type Layer, texture1d_array::size_type Face, texture1d_array::size_type Level, genType const& Texel)
		{
			GLI_ASSERT(Face == 0);
			Texture.store<genType>(TexelCoord, Layer, Level, Texel);
		}
	};

	template <typename genType>
	struct accessFunc<texture2d, genType>
	{
		static genType load(texture2d const & Texture, texture2d::extent_type const & TexelCoord, texture2d::size_type Layer, texture2d::size_type Face, texture2d::size_type Level)
		{
			GLI_ASSERT(Layer == 0 && Face == 0);
			return Texture.load<genType>(TexelCoord, Level);
		}

		static void store(texture2d & Texture, texture2d::extent_type const & TexelCoord, texture2d::size_type Layer, texture2d::size_type Face, texture2d::size_type Level, genType const & Texel)
		{
			GLI_ASSERT(Layer == 0 && Face == 0);
			Texture.store<genType>(TexelCoord, Level, Texel);
		}
	};

	template <typename genType>
	struct accessFunc<texture2d_array, genType>
	{
		static genType load(texture2d_array const & Texture, texture2d_array::extent_type const & TexelCoord, texture2d_array::size_type Layer, texture2d_array::size_type Face, texture2d_array::size_type Level)
		{
			GLI_ASSERT(Face == 0);
			return Texture.load<genType>(TexelCoord, Layer, Level);
		}

		static void store(texture2d_array & Texture, texture2d_array::extent_type const & TexelCoord, texture2d_array::size_type Layer, texture2d_array::size_type Face, texture2d_array::size_type Level, genType const & Texel)
		{
			GLI_ASSERT(Face == 0);
			Texture.store<genType>(TexelCoord, Layer, Level, Texel);
		}
	};

	template <typename genType>
	struct accessFunc<texture3d, genType>
	{
		static genType load(texture3d const & Texture, texture3d::extent_type const & TexelCoord, texture3d::size_type Layer, texture3d::size_type Face, texture3d::size_type Level)
		{
			GLI_ASSERT(Layer == 0 && Face == 0);
			return Texture.load<genType>(TexelCoord, Level);
		}

		static void store(texture3d & Texture, texture3d::extent_type const & TexelCoord, texture3d::size_type Layer, texture3d::size_type Face, texture3d::size_type Level, genType const & Texel)
		{
			GLI_ASSERT(Layer == 0 && Face == 0);
			Texture.store<genType>(TexelCoord, Level, Texel);
		}
	};

	template <typename genType>
	struct accessFunc<texture_cube, genType>
	{
		static genType load(texture_cube const& Texture, texture_cube::extent_type const& TexelCoord, texture_cube::size_type Layer, texture_cube::size_type Face, texture_cube::size_type Level)
		{
			GLI_ASSERT(Layer == 0);
			return Texture.load<genType>(TexelCoord, Face, Level);
		}

		static void store(texture_cube& Texture, texture_cube::extent_type const& TexelCoord, texture_cube::size_type Layer, texture_cube::size_type Face, texture_cube::size_type Level, genType const& Texel)
		{
			GLI_ASSERT(Layer == 0);
			Texture.store<genType>(TexelCoord, Face, Level, Texel);
		}
	};

	template <typename genType>
	struct accessFunc<texture_cube_array, genType>
	{
		static genType load(texture_cube_array const & Texture, texture_cube_array::extent_type const & TexelCoord, texture_cube_array::size_type Layer, texture_cube_array::size_type Face, texture_cube_array::size_type Level)
		{
			return Texture.load<genType>(TexelCoord, Layer, Face, Level);
		}

		static void store(texture_cube_array & Texture, texture_cube_array::extent_type const & TexelCoord, texture_cube_array::size_type Layer, texture_cube_array::size_type Face, texture_cube_array::size_type Level, genType const & Texel)
		{
			Texture.store<genType>(TexelCoord, Layer, Face, Level, Texel);
		}
	};

	// convertFunc class

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType, convertMode mode = CONVERT_MODE_CAST, bool isSamplerFloat = false>
	struct convertFunc
	{
		typedef accessFunc<textureType, vecType<T, P> > access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			return make_vec4<retType, P>(vecType<retType, P>(access::load(Texture, TexelCoord, Layer, Face, Level)));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			access::store(Texture, TexelCoord, Layer, Face, Level, vecType<T, P>(Texel));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType, bool isSamplerFloat>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_DEFAULT, isSamplerFloat>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			return tvec4<retType, P>(0, 0, 0, 1);
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_NORM, true>
	{
		typedef accessFunc<textureType, vecType<T, P> > access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_NORM requires a float sampler");
			return make_vec4<retType, P>(compNormalize<retType>(access::load(Texture, TexelCoord, Layer, Face, Level)));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_NORM requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, compScale<T>(vecType<retType, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_SRGB, true>
	{
		typedef accessFunc<textureType, vecType<T, P> > access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_SRGB requires a float sampler");
			return make_vec4<retType, P>(convertSRGBToLinear(compNormalize<retType>(access::load(Texture, TexelCoord, Layer, Face, Level))));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_SRGB requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, gli::compScale<T>(convertLinearToSRGB(vecType<retType, P>(Texel))));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_RGB9E5, true>
	{
		typedef accessFunc<textureType, uint32> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB9E5 requires a float sampler");
			return tvec4<retType, P>(unpackF3x9_E1x5(access::load(Texture, TexelCoord, Layer, Face, Level)), static_cast<retType>(1));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB9E5 requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packF3x9_E1x5(tvec3<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_RG11B10F, true>
	{
		typedef accessFunc<textureType, uint32> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RG11B10F requires a float sampler");
			return tvec4<retType, P>(unpackF2x11_1x10(access::load(Texture, TexelCoord, Layer, Face, Level)), static_cast<retType>(1));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RG11B10F requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packF2x11_1x10(tvec3<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_HALF, true>
	{
		typedef accessFunc<textureType, vecType<uint16, P> > access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_HALF requires a float sampler");
			return make_vec4<retType, P>(vecType<retType, P>(unpackHalf(access::load(Texture, TexelCoord, Layer, Face, Level))));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_HALF requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packHalf(vecType<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_44UNORM, true>
	{
		typedef accessFunc<textureType, uint8> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_44UNORM requires a float sampler");
			return tvec4<retType, P>(tvec2<retType, P>(unpackUnorm2x4(access::load(Texture, TexelCoord, Layer, Face, Level))), static_cast<retType>(0), static_cast<retType>(1));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_44UNORM requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packUnorm2x4(tvec2<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_4444UNORM, true>
	{
		typedef accessFunc<textureType, uint16> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_4444UNORM requires a float sampler");
			return tvec4<retType, P>(unpackUnorm4x4(access::load(Texture, TexelCoord, Layer, Face, Level)));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_4444UNORM requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packUnorm4x4(tvec4<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_565UNORM, true>
	{
		typedef accessFunc<textureType, uint16> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_565UNORM requires a float sampler");
			return tvec4<retType, P>(unpackUnorm1x5_1x6_1x5(access::load(Texture, TexelCoord, Layer, Face, Level)), static_cast<retType>(1));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_565UNORM requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packUnorm1x5_1x6_1x5(tvec3<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_5551UNORM, true>
	{
		typedef accessFunc<textureType, uint16> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_5551UNORM requires a float sampler");
			return tvec4<retType, P>(unpackUnorm3x5_1x1(access::load(Texture, TexelCoord, Layer, Face, Level)));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_5551UNORM requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packUnorm3x5_1x1(tvec4<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_332UNORM, true>
	{
		typedef accessFunc<textureType, uint8> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_332UNORM requires a float sampler");
			return tvec4<retType, P>(unpackUnorm2x3_1x2(access::load(Texture, TexelCoord, Layer, Face, Level)), static_cast<retType>(1));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_332UNORM requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packUnorm2x3_1x2(tvec3<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_RGB10A2UNORM, true>
	{
		typedef accessFunc<textureType, uint32> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2UNORM requires a float sampler");
			return tvec4<retType, P>(unpackUnorm3x10_1x2(access::load(Texture, TexelCoord, Layer, Face, Level)));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2UNORM requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packUnorm3x10_1x2(tvec4<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_RGB10A2SNORM, true>
	{
		typedef accessFunc<textureType, uint32> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2SNORM requires a float sampler");
			return tvec4<retType, P>(unpackSnorm3x10_1x2(access::load(Texture, TexelCoord, Layer, Face, Level)));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2SNORM requires a float sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packSnorm3x10_1x2(Texel));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_RGB10A2USCALE, true>
	{
		typedef accessFunc<textureType, uint32> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2USCALE requires a float sampler");
			glm::detail::u10u10u10u2 Unpack;
			Unpack.pack = access::load(Texture, TexelCoord, Layer, Face, Level);
			return tvec4<retType, P>(Unpack.data.x, Unpack.data.y, Unpack.data.z, Unpack.data.w);
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2USCALE requires a float sampler");
			glm::detail::u10u10u10u2 Unpack;
			Unpack.data.x = static_cast<uint>(Texel.x);
			Unpack.data.y = static_cast<uint>(Texel.y);
			Unpack.data.z = static_cast<uint>(Texel.z);
			Unpack.data.w = static_cast<uint>(Texel.w);
			access::store(Texture, TexelCoord, Layer, Face, Level, Unpack.pack);
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_RGB10A2SSCALE, true>
	{
		typedef accessFunc<textureType, uint32> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2SSCALE requires a float sampler");
			glm::detail::i10i10i10i2 Unpack;
			Unpack.pack = access::load(Texture, TexelCoord, Layer, Face, Level);
			return tvec4<retType, P>(Unpack.data.x, Unpack.data.y, Unpack.data.z, Unpack.data.w);
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2SSCALE requires a float sampler");
			glm::detail::i10i10i10i2 Unpack;
			Unpack.data.x = static_cast<int>(Texel.x);
			Unpack.data.y = static_cast<int>(Texel.y);
			Unpack.data.z = static_cast<int>(Texel.z);
			Unpack.data.w = static_cast<int>(Texel.w);
			access::store(Texture, TexelCoord, Layer, Face, Level, Unpack.pack);
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_RGB10A2UINT, false>
	{
		typedef accessFunc<textureType, uint32> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_integer, "CONVERT_MODE_RGB10A2UINT requires an integer sampler");
			return tvec4<retType, P>(unpackU3x10_1x2(access::load(Texture, TexelCoord, Layer, Face, Level)));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_integer, "CONVERT_MODE_RGB10A2UINT requires an integer sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packU3x10_1x2(Texel));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_RGB10A2SINT, false>
	{
		typedef accessFunc<textureType, uint32> access;

		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_integer, "CONVERT_MODE_RGB10A2SINT requires an integer sampler");
			return tvec4<retType, P>(unpackI3x10_1x2(access::load(Texture, TexelCoord, Layer, Face, Level)));
		}

		static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_integer, "CONVERT_MODE_RGB10A2SINT requires an integer sampler");
			access::store(Texture, TexelCoord, Layer, Face, Level, packI3x10_1x2(Texel));
		}
	};

	template <typename textureType, typename samplerValType, precision P>
	struct convert
	{
		typedef glm::tvec4<samplerValType, P>(*fetchFunc)(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level);
		typedef void(*writeFunc)(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, glm::tvec4<samplerValType, P> const & Texel);

		template <typename T, template <typename, precision> class vecType, convertMode mode>
		struct conv
		{
			static tvec4<samplerValType, P> fetch(textureType const & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level)
			{
				return convertFunc<textureType, samplerValType, T, P, vecType, mode, std::numeric_limits<samplerValType>::is_iec559>::fetch(Texture, TexelCoord, Layer, Face, Level);
			}

			static void write(textureType & Texture, typename textureType::extent_type const & TexelCoord, typename textureType::size_type Layer, typename textureType::size_type Face, typename textureType::size_type Level, tvec4<samplerValType, P> const & Texel)
			{
				convertFunc<textureType, samplerValType, T, P, vecType, mode, std::numeric_limits<samplerValType>::is_iec559>::write(Texture, TexelCoord, Layer, Face, Level, Texel);
			}
		};

		struct func
		{
			fetchFunc Fetch;
			writeFunc Write;
		};

		static func call(format Format)
		{
			static func Table[] =
			{
				{conv<u8, tvec2, CONVERT_MODE_44UNORM>::fetch, conv<u8, tvec2, CONVERT_MODE_44UNORM>::write},				// FORMAT_RG4_UNORM
				{conv<u8, tvec4, CONVERT_MODE_4444UNORM>::fetch, conv<u8, tvec4, CONVERT_MODE_4444UNORM>::write},			// FORMAT_RGBA4_UNORM
				{conv<u8, tvec4, CONVERT_MODE_4444UNORM>::fetch, conv<u8, tvec4, CONVERT_MODE_4444UNORM>::write},			// FORMAT_BGRA4_UNORM
				{conv<u8, tvec3, CONVERT_MODE_565UNORM>::fetch, conv<u8, tvec3, CONVERT_MODE_565UNORM>::write},				// FORMAT_R5G6B5_UNORM
				{conv<u8, tvec3, CONVERT_MODE_565UNORM>::fetch, conv<u8, tvec3, CONVERT_MODE_565UNORM>::write},				// FORMAT_B5G6R5_UNORM
				{conv<u8, tvec4, CONVERT_MODE_5551UNORM>::fetch, conv<u8, tvec4, CONVERT_MODE_5551UNORM>::write},			// FORMAT_RGB5A1_UNORM
				{conv<u8, tvec4, CONVERT_MODE_5551UNORM>::fetch, conv<u8, tvec4, CONVERT_MODE_5551UNORM>::write},			// FORMAT_BGR5A1_UNORM
				{conv<u8, tvec4, CONVERT_MODE_5551UNORM>::fetch, conv<u8, tvec4, CONVERT_MODE_5551UNORM>::write},			// FORMAT_A1RGB5_UNORM

				{conv<u8, tvec1, CONVERT_MODE_NORM>::fetch, conv<u8, tvec1, CONVERT_MODE_NORM>::write},						// FORMAT_R8_UNORM
				{conv<i8, tvec1, CONVERT_MODE_NORM>::fetch, conv<i8, tvec1, CONVERT_MODE_NORM>::write},						// FORMAT_R8_SNORM
				{conv<u8, tvec1, CONVERT_MODE_CAST>::fetch, conv<u8, tvec1, CONVERT_MODE_CAST>::write},						// FORMAT_R8_USCALED
				{conv<i8, tvec1, CONVERT_MODE_CAST>::fetch, conv<i8, tvec1, CONVERT_MODE_CAST>::write},						// FORMAT_R8_SSCALED
				{conv<u8, tvec1, CONVERT_MODE_CAST>::fetch, conv<u8, tvec1, CONVERT_MODE_CAST>::write},						// FORMAT_R8_UINT
				{conv<i8, tvec1, CONVERT_MODE_CAST>::fetch, conv<i8, tvec1, CONVERT_MODE_CAST>::write},						// FORMAT_R8_SINT
				{conv<u8, tvec1, CONVERT_MODE_SRGB>::fetch, conv<u8, tvec1, CONVERT_MODE_SRGB>::write},						// FORMAT_R8_SRGB

				{conv<u8, tvec2, CONVERT_MODE_NORM>::fetch, conv<u8, tvec2, CONVERT_MODE_NORM>::write},						// FORMAT_RG8_UNORM
				{conv<i8, tvec2, CONVERT_MODE_NORM>::fetch, conv<i8, tvec2, CONVERT_MODE_NORM>::write},						// FORMAT_RG8_SNORM
				{conv<u8, tvec2, CONVERT_MODE_CAST>::fetch, conv<u8, tvec2, CONVERT_MODE_CAST>::write},						// FORMAT_RG8_USCALED
				{conv<i8, tvec2, CONVERT_MODE_CAST>::fetch, conv<i8, tvec2, CONVERT_MODE_CAST>::write},						// FORMAT_RG8_SSCALED
				{conv<u8, tvec2, CONVERT_MODE_CAST>::fetch, conv<u8, tvec2, CONVERT_MODE_CAST>::write},						// FORMAT_RG8_UINT
				{conv<i8, tvec2, CONVERT_MODE_CAST>::fetch, conv<i8, tvec2, CONVERT_MODE_CAST>::write},						// FORMAT_RG8_SINT
				{conv<u8, tvec2, CONVERT_MODE_SRGB>::fetch, conv<u8, tvec2, CONVERT_MODE_SRGB>::write},						// FORMAT_RG8_SRGB

				{conv<u8, tvec3, CONVERT_MODE_NORM>::fetch, conv<u8, tvec3, CONVERT_MODE_NORM>::write},						// FORMAT_RGB8_UNORM
				{conv<i8, tvec3, CONVERT_MODE_NORM>::fetch, conv<i8, tvec3, CONVERT_MODE_NORM>::write},						// FORMAT_RGB8_SNORM
				{conv<u8, tvec3, CONVERT_MODE_CAST>::fetch, conv<u8, tvec3, CONVERT_MODE_CAST>::write},						// FORMAT_RGB8_USCALED
				{conv<i8, tvec3, CONVERT_MODE_CAST>::fetch, conv<i8, tvec3, CONVERT_MODE_CAST>::write},						// FORMAT_RGB8_SSCALED
				{conv<u8, tvec3, CONVERT_MODE_CAST>::fetch, conv<u8, tvec3, CONVERT_MODE_CAST>::write},						// FORMAT_RGB8_UINT
				{conv<i8, tvec3, CONVERT_MODE_CAST>::fetch, conv<i8, tvec3, CONVERT_MODE_CAST>::write},						// FORMAT_RGB8_SINT
				{conv<u8, tvec3, CONVERT_MODE_SRGB>::fetch, conv<u8, tvec3, CONVERT_MODE_SRGB>::write},						// FORMAT_RGB8_SRGB

				{conv<u8, tvec3, CONVERT_MODE_NORM>::fetch, conv<u8, tvec3, CONVERT_MODE_NORM>::write},						// FORMAT_BGR8_UNORM
				{conv<i8, tvec3, CONVERT_MODE_NORM>::fetch, conv<i8, tvec3, CONVERT_MODE_NORM>::write},						// FORMAT_BGR8_SNORM
				{conv<u8, tvec3, CONVERT_MODE_CAST>::fetch, conv<u8, tvec3, CONVERT_MODE_CAST>::write},						// FORMAT_BGR8_USCALED
				{conv<i8, tvec3, CONVERT_MODE_CAST>::fetch, conv<i8, tvec3, CONVERT_MODE_CAST>::write},						// FORMAT_BGR8_SSCALED
				{conv<u32, tvec3, CONVERT_MODE_CAST>::fetch, conv<u32, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_BGR8_UINT
				{conv<i32, tvec3, CONVERT_MODE_CAST>::fetch, conv<i32, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_BGR8_SINT
				{conv<u8, tvec3, CONVERT_MODE_SRGB>::fetch, conv<u8, tvec3, CONVERT_MODE_SRGB>::write},						// FORMAT_BGR8_SRGB

				{conv<u8, tvec4, CONVERT_MODE_NORM>::fetch, conv<u8, tvec4, CONVERT_MODE_NORM>::write},						// FORMAT_RGBA8_UNORM
				{conv<i8, tvec4, CONVERT_MODE_NORM>::fetch, conv<i8, tvec4, CONVERT_MODE_NORM>::write},						// FORMAT_RGBA8_SNORM
				{conv<u8, tvec4, CONVERT_MODE_CAST>::fetch, conv<u8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_RGBA8_USCALED
				{conv<i8, tvec4, CONVERT_MODE_CAST>::fetch, conv<i8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_RGBA8_SSCALED
				{conv<u8, tvec4, CONVERT_MODE_CAST>::fetch, conv<u8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_RGBA8_UINT
				{conv<i8, tvec4, CONVERT_MODE_CAST>::fetch, conv<i8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_RGBA8_SINT
				{conv<u8, tvec4, CONVERT_MODE_SRGB>::fetch, conv<u8, tvec4, CONVERT_MODE_SRGB>::write},						// FORMAT_RGBA8_SRGB

				{conv<u8, tvec4, CONVERT_MODE_NORM>::fetch, conv<u8, tvec4, CONVERT_MODE_NORM>::write},						// FORMAT_BGRA8_UNORM
				{conv<i8, tvec4, CONVERT_MODE_NORM>::fetch, conv<i8, tvec4, CONVERT_MODE_NORM>::write},						// FORMAT_BGRA8_SNORM
				{conv<u8, tvec4, CONVERT_MODE_CAST>::fetch, conv<u8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_BGRA8_USCALED
				{conv<i8, tvec4, CONVERT_MODE_CAST>::fetch, conv<i8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_BGRA8_SSCALED
				{conv<u8, tvec4, CONVERT_MODE_CAST>::fetch, conv<u8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_BGRA8_UINT
				{conv<i8, tvec4, CONVERT_MODE_CAST>::fetch, conv<i8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_BGRA8_SINT
				{conv<u8, tvec4, CONVERT_MODE_SRGB>::fetch, conv<u8, tvec4, CONVERT_MODE_SRGB>::write},						// FORMAT_BGRA8_SRGB

				{conv<u8, tvec4, CONVERT_MODE_NORM>::fetch, conv<u8, tvec4, CONVERT_MODE_NORM>::write},						// FORMAT_ABGR8_UNORM
				{conv<i8, tvec4, CONVERT_MODE_NORM>::fetch, conv<i8, tvec4, CONVERT_MODE_NORM>::write},						// FORMAT_ABGR8_SNORM
				{conv<u8, tvec4, CONVERT_MODE_CAST>::fetch, conv<u8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_ABGR8_USCALED
				{conv<i8, tvec4, CONVERT_MODE_CAST>::fetch, conv<i8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_ABGR8_SSCALED
				{conv<u8, tvec4, CONVERT_MODE_CAST>::fetch, conv<u8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_ABGR8_UINT
				{conv<i8, tvec4, CONVERT_MODE_CAST>::fetch, conv<i8, tvec4, CONVERT_MODE_CAST>::write},						// FORMAT_ABGR8_SINT
				{conv<u8, tvec4, CONVERT_MODE_SRGB>::fetch, conv<u8, tvec4, CONVERT_MODE_SRGB>::write},						// FORMAT_ABGR8_SRGB

				{conv<u8, tvec4, CONVERT_MODE_RGB10A2UNORM>::fetch, conv<u8, tvec4, CONVERT_MODE_RGB10A2UNORM>::write},		// FORMAT_RGB10A2_UNORM
				{conv<i8, tvec4, CONVERT_MODE_RGB10A2SNORM>::fetch, conv<i8, tvec4, CONVERT_MODE_RGB10A2SNORM>::write},		// FORMAT_RGB10A2_SNORM
				{conv<u8, tvec4, CONVERT_MODE_RGB10A2USCALE>::fetch, conv<u8, tvec4, CONVERT_MODE_RGB10A2USCALE>::write},	// FORMAT_RGB10A2_USCALED
				{conv<i8, tvec4, CONVERT_MODE_RGB10A2SSCALE>::fetch, conv<i8, tvec4, CONVERT_MODE_RGB10A2SSCALE>::write},	// FORMAT_RGB10A2_SSCALED
				{conv<u8, tvec4, CONVERT_MODE_RGB10A2UINT>::fetch, conv<u8, tvec4, CONVERT_MODE_RGB10A2UINT>::write},		// FORMAT_RGB10A2_UINT
				{conv<i8, tvec4, CONVERT_MODE_RGB10A2SINT>::fetch, conv<i8, tvec4, CONVERT_MODE_RGB10A2SINT>::write},		// FORMAT_RGB10A2_SINT

				{conv<u8, tvec4, CONVERT_MODE_RGB10A2UNORM>::fetch, conv<u8, tvec4, CONVERT_MODE_RGB10A2UNORM>::write},		// FORMAT_BGR10A2_UNORM
				{conv<i8, tvec4, CONVERT_MODE_RGB10A2SNORM>::fetch, conv<i8, tvec4, CONVERT_MODE_RGB10A2SNORM>::write},		// FORMAT_BGR10A2_SNORM
				{conv<u8, tvec4, CONVERT_MODE_RGB10A2USCALE>::fetch, conv<u8, tvec4, CONVERT_MODE_RGB10A2USCALE>::write},	// FORMAT_BGR10A2_USCALED
				{conv<i8, tvec4, CONVERT_MODE_RGB10A2SSCALE>::fetch, conv<i8, tvec4, CONVERT_MODE_RGB10A2SSCALE>::write},	// FORMAT_BGR10A2_SSCALED
				{conv<u8, tvec4, CONVERT_MODE_RGB10A2UINT>::fetch, conv<u8, tvec4, CONVERT_MODE_RGB10A2UINT>::write},		// FORMAT_BGR10A2_UINT
				{conv<i8, tvec4, CONVERT_MODE_RGB10A2SINT>::fetch, conv<i8, tvec4, CONVERT_MODE_RGB10A2SINT>::write},		// FORMAT_BGR10A2_SINT

				{conv<u16, tvec1, CONVERT_MODE_NORM>::fetch, conv<u16, tvec1, CONVERT_MODE_NORM>::write},					// FORMAT_R16_UNORM_PACK16
				{conv<i16, tvec1, CONVERT_MODE_NORM>::fetch, conv<i16, tvec1, CONVERT_MODE_NORM>::write},					// FORMAT_R16_SNORM_PACK16
				{conv<u16, tvec1, CONVERT_MODE_CAST>::fetch, conv<u16, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R16_USCALED_PACK16
				{conv<i16, tvec1, CONVERT_MODE_CAST>::fetch, conv<i16, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R16_SSCALED_PACK16
				{conv<u16, tvec1, CONVERT_MODE_CAST>::fetch, conv<u16, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R16_UINT_PACK16
				{conv<i16, tvec1, CONVERT_MODE_CAST>::fetch, conv<i16, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R16_SINT_PACK16
				{conv<u16, tvec1, CONVERT_MODE_HALF>::fetch, conv<u16, tvec1, CONVERT_MODE_HALF>::write},					// FORMAT_R16_SFLOAT_PACK16

				{conv<u16, tvec2, CONVERT_MODE_NORM>::fetch, conv<u16, tvec2, CONVERT_MODE_NORM>::write},					// FORMAT_RG16_UNORM_PACK16
				{conv<i16, tvec2, CONVERT_MODE_NORM>::fetch, conv<i16, tvec2, CONVERT_MODE_NORM>::write},					// FORMAT_RG16_SNORM_PACK16
				{conv<u16, tvec2, CONVERT_MODE_CAST>::fetch, conv<u16, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG16_USCALED_PACK16
				{conv<i16, tvec2, CONVERT_MODE_CAST>::fetch, conv<i16, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG16_SSCALED_PACK16
				{conv<u16, tvec2, CONVERT_MODE_CAST>::fetch, conv<u16, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG16_UINT_PACK16
				{conv<i16, tvec2, CONVERT_MODE_CAST>::fetch, conv<i16, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG16_SINT_PACK16
				{conv<u16, tvec2, CONVERT_MODE_HALF>::fetch, conv<u16, tvec2, CONVERT_MODE_HALF>::write},					// FORMAT_RG16_SFLOAT_PACK16

				{conv<u16, tvec3, CONVERT_MODE_NORM>::fetch, conv<u16, tvec3, CONVERT_MODE_NORM>::write},					// FORMAT_RGB16_UNORM_PACK16
				{conv<i16, tvec3, CONVERT_MODE_NORM>::fetch, conv<i16, tvec3, CONVERT_MODE_NORM>::write},					// FORMAT_RGB16_SNORM_PACK16
				{conv<u16, tvec3, CONVERT_MODE_CAST>::fetch, conv<u16, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB16_USCALED_PACK16
				{conv<i16, tvec3, CONVERT_MODE_CAST>::fetch, conv<i16, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB16_SSCALED_PACK16
				{conv<u16, tvec3, CONVERT_MODE_CAST>::fetch, conv<u16, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB16_UINT_PACK16
				{conv<i16, tvec3, CONVERT_MODE_CAST>::fetch, conv<i16, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB16_SINT_PACK16
				{conv<u16, tvec3, CONVERT_MODE_HALF>::fetch, conv<u16, tvec3, CONVERT_MODE_HALF>::write},					// FORMAT_RGB16_SFLOAT_PACK16

				{conv<u16, tvec4, CONVERT_MODE_NORM>::fetch, conv<u16, tvec4, CONVERT_MODE_NORM>::write},					// FORMAT_RGBA16_UNORM_PACK16
				{conv<i16, tvec4, CONVERT_MODE_NORM>::fetch, conv<i16, tvec4, CONVERT_MODE_NORM>::write},					// FORMAT_RGBA16_SNORM_PACK16
				{conv<u16, tvec4, CONVERT_MODE_CAST>::fetch, conv<u16, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA16_USCALED_PACK16
				{conv<i16, tvec4, CONVERT_MODE_CAST>::fetch, conv<i16, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA16_SSCALED_PACK16
				{conv<u16, tvec4, CONVERT_MODE_CAST>::fetch, conv<u16, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA16_UINT_PACK16
				{conv<i16, tvec4, CONVERT_MODE_CAST>::fetch, conv<i16, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA16_SINT_PACK16
				{conv<u16, tvec4, CONVERT_MODE_HALF>::fetch, conv<u16, tvec4, CONVERT_MODE_HALF>::write},					// FORMAT_RGBA16_SFLOAT_PACK16

				{conv<u32, tvec1, CONVERT_MODE_CAST>::fetch, conv<u32, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R32_UINT_PACK32
				{conv<i32, tvec1, CONVERT_MODE_CAST>::fetch, conv<i32, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R32_SINT_PACK32
				{conv<f32, tvec1, CONVERT_MODE_CAST>::fetch, conv<f32, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R32_SFLOAT_PACK32

				{conv<u32, tvec2, CONVERT_MODE_CAST>::fetch, conv<u32, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG32_UINT_PACK32
				{conv<i32, tvec2, CONVERT_MODE_CAST>::fetch, conv<i32, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG32_SINT_PACK32
				{conv<f32, tvec2, CONVERT_MODE_CAST>::fetch, conv<f32, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG32_SFLOAT_PACK32

				{conv<u32, tvec3, CONVERT_MODE_CAST>::fetch, conv<u32, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB32_UINT_PACK32
				{conv<i32, tvec3, CONVERT_MODE_CAST>::fetch, conv<i32, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB32_SINT_PACK32
				{conv<f32, tvec3, CONVERT_MODE_CAST>::fetch, conv<f32, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB32_SFLOAT_PACK32

				{conv<u32, tvec4, CONVERT_MODE_CAST>::fetch, conv<u32, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA32_UINT_PACK32
				{conv<i32, tvec4, CONVERT_MODE_CAST>::fetch, conv<i32, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA32_SINT_PACK32
				{conv<f32, tvec4, CONVERT_MODE_CAST>::fetch, conv<f32, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA32_SFLOAT_PACK32

				{conv<u64, tvec1, CONVERT_MODE_CAST>::fetch, conv<u64, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R64_UINT_PACK64
				{conv<i64, tvec1, CONVERT_MODE_CAST>::fetch, conv<i64, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R64_SINT_PACK64
				{conv<f64, tvec1, CONVERT_MODE_CAST>::fetch, conv<f64, tvec1, CONVERT_MODE_CAST>::write},					// FORMAT_R64_SFLOAT_PACK64

				{conv<u64, tvec2, CONVERT_MODE_CAST>::fetch, conv<u64, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG64_UINT_PACK64
				{conv<i64, tvec2, CONVERT_MODE_CAST>::fetch, conv<i64, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG64_SINT_PACK64
				{conv<f64, tvec2, CONVERT_MODE_CAST>::fetch, conv<f64, tvec2, CONVERT_MODE_CAST>::write},					// FORMAT_RG64_SFLOAT_PACK64

				{conv<u64, tvec3, CONVERT_MODE_CAST>::fetch, conv<u64, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB64_UINT_PACK64
				{conv<i64, tvec3, CONVERT_MODE_CAST>::fetch, conv<i64, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB64_SINT_PACK64
				{conv<f64, tvec3, CONVERT_MODE_CAST>::fetch, conv<f64, tvec3, CONVERT_MODE_CAST>::write},					// FORMAT_RGB64_SFLOAT_PACK64

				{conv<u64, tvec4, CONVERT_MODE_CAST>::fetch, conv<u64, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA64_UINT_PACK64
				{conv<i64, tvec4, CONVERT_MODE_CAST>::fetch, conv<i64, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA64_SINT_PACK64
				{conv<f64, tvec4, CONVERT_MODE_CAST>::fetch, conv<f64, tvec4, CONVERT_MODE_CAST>::write},					// FORMAT_RGBA64_SFLOAT_PACK64

				{conv<u32, tvec1, CONVERT_MODE_RG11B10F>::fetch, conv<u32, tvec1, CONVERT_MODE_RG11B10F>::write},			// FORMAT_RG11B10_UFLOAT
				{conv<u32, tvec1, CONVERT_MODE_RGB9E5>::fetch, conv<u32, tvec1, CONVERT_MODE_RGB9E5>::write},				// FORMAT_RGB9E5_UFLOAT

				{conv<u16, tvec1, CONVERT_MODE_DEFAULT>::fetch, conv<u16, tvec1, CONVERT_MODE_DEFAULT>::write},				// FORMAT_D16_UNORM_PACK16
				{conv<u32, tvec1, CONVERT_MODE_DEFAULT>::fetch, conv<u32, tvec1, CONVERT_MODE_DEFAULT>::write},				// FORMAT_D24_UNORM
				{conv<float, tvec1, CONVERT_MODE_DEFAULT>::fetch, conv<float, tvec1, CONVERT_MODE_DEFAULT>::write},			// FORMAT_D32_SFLOAT_PACK32
				{conv<u8, tvec1, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec1, CONVERT_MODE_DEFAULT>::write},				// FORMAT_S8_UINT_PACK8
				{conv<u16, tvec2, CONVERT_MODE_DEFAULT>::fetch, conv<u16, tvec2, CONVERT_MODE_DEFAULT>::write},				// FORMAT_D16_UNORM_S8_UINT_PACK32
				{conv<u32, tvec2, CONVERT_MODE_DEFAULT>::fetch, conv<u32, tvec2, CONVERT_MODE_DEFAULT>::write},				// FORMAT_D24_UNORM_S8_UINT_PACK32
				{conv<u32, tvec2, CONVERT_MODE_DEFAULT>::fetch, conv<u32, tvec2, CONVERT_MODE_DEFAULT>::write},				// FORMAT_D32_SFLOAT_S8_UINT_PACK64

				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_DXT1_UNORM_BLOCK8
				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_DXT1_SRGB_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_DXT1_UNORM_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_DXT1_SRGB_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_DXT3_UNORM_BLOCK16
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_DXT3_SRGB_BLOCK16
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_DXT5_UNORM_BLOCK16
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_DXT5_SRGB_BLOCK16
				{conv<u8, tvec1, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec1, CONVERT_MODE_DEFAULT>::write},				// FORMAT_R_ATI1N_UNORM_BLOCK8
				{conv<u8, tvec1, CONVERT_MODE_DEFAULT>::fetch, conv<i8, tvec1, CONVERT_MODE_DEFAULT>::write},				// FORMAT_R_ATI1N_SNORM_BLOCK8
				{conv<u8, tvec2, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec2, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RG_ATI2N_UNORM_BLOCK16
				{conv<u8, tvec2, CONVERT_MODE_DEFAULT>::fetch, conv<i8, tvec2, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RG_ATI2N_SNORM_BLOCK16
				{conv<float, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<float, tvec3, CONVERT_MODE_DEFAULT>::write},			// FORMAT_RGB_BP_UFLOAT_BLOCK16
				{conv<float, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<float, tvec3, CONVERT_MODE_DEFAULT>::write},			// FORMAT_RGB_BP_SFLOAT_BLOCK16
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_BP_UNORM_BLOCK16
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_BP_SRGB_BLOCK16

				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_ETC2_UNORM_BLOCK8
				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_ETC2_SRGB_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_ETC2_A1_UNORM_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_ETC2_A1_SRGB_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_ETC2_UNORM_BLOCK16
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_ETC2_SRGB_BLOCK16
				{conv<u8, tvec1, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec1, CONVERT_MODE_DEFAULT>::write},				// FORMAT_R_EAC_UNORM_BLOCK8
				{conv<u8, tvec1, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec1, CONVERT_MODE_DEFAULT>::write},				// FORMAT_R_EAC_SNORM_BLOCK8
				{conv<u8, tvec2, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec2, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RG_EAC_UNORM_BLOCK16
				{conv<u8, tvec2, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec2, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RG_EAC_SNORM_BLOCK16

				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_4x4_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_4x4_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_5x4_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_5x4_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_5x5_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_5x5_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_6x5_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_6x5_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_6x6_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_6x6_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_8x5_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_8x5_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_8x6_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_8x6_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_8x8_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_8x8_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_10x5_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_10x5_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_10x6_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_10x6_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_10x8_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_10x8_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_10x10_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_10x10_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_12x10_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_12x10_SRGB
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_12x12_UNORM
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_ASTC_12x12_SRGB

				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_PVRTC1_8X8_UNORM_BLOCK32
				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_PVRTC1_8X8_SRGB_BLOCK32
				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_PVRTC1_16X8_UNORM_BLOCK32
				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_PVRTC1_16X8_SRGB_BLOCK32
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_PVRTC1_8X8_UNORM_BLOCK32
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_PVRTC1_8X8_SRGB_BLOCK32
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_PVRTC1_16X8_UNORM_BLOCK32
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_PVRTC1_16X8_SRGB_BLOCK32
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_PVRTC2_4X4_UNORM_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_PVRTC2_4X4_SRGB_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_PVRTC2_8X4_UNORM_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_PVRTC2_8X4_SRGB_BLOCK8

				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_ETC_UNORM_BLOCK8
				{conv<u8, tvec3, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec3, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGB_ATC_UNORM_BLOCK8
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_ATCA_UNORM_BLOCK16
				{conv<u8, tvec4, CONVERT_MODE_DEFAULT>::fetch, conv<u8, tvec4, CONVERT_MODE_DEFAULT>::write},				// FORMAT_RGBA_ATCI_UNORM_BLOCK16

				{conv<u8, tvec1, CONVERT_MODE_NORM>::fetch, conv<u8, tvec1, CONVERT_MODE_NORM>::write},						// FORMAT_L8_UNORM_PACK8
				{conv<u8, tvec1, CONVERT_MODE_NORM>::fetch, conv<u8, tvec1, CONVERT_MODE_NORM>::write},						// FORMAT_A8_UNORM_PACK8
				{conv<u8, tvec2, CONVERT_MODE_NORM>::fetch, conv<u8, tvec2, CONVERT_MODE_NORM>::write},						// FORMAT_LA8_UNORM_PACK8
				{conv<u16, tvec1, CONVERT_MODE_NORM>::fetch, conv<u16, tvec1, CONVERT_MODE_NORM>::write},					// FORMAT_L16_UNORM_PACK16
				{conv<u16, tvec1, CONVERT_MODE_NORM>::fetch, conv<u16, tvec1, CONVERT_MODE_NORM>::write},					// FORMAT_A16_UNORM_PACK16
				{conv<u16, tvec2, CONVERT_MODE_NORM>::fetch, conv<u16, tvec2, CONVERT_MODE_NORM>::write},					// FORMAT_LA16_UNORM_PACK16

				{conv<u8, tvec4, CONVERT_MODE_NORM>::fetch, conv<u8, tvec4, CONVERT_MODE_NORM>::write},						// FORMAT_BGRX8_UNORM
				{conv<u8, tvec4, CONVERT_MODE_SRGB>::fetch, conv<u8, tvec4, CONVERT_MODE_SRGB>::write},						// FORMAT_BGRX8_SRGB

				{conv<u8, tvec3, CONVERT_MODE_332UNORM>::fetch, conv<u8, tvec3, CONVERT_MODE_332UNORM>::write}				// FORMAT_RG3B2_UNORM
			};
			static_assert(sizeof(Table) / sizeof(Table[0]) == FORMAT_COUNT, "Texel functions need to be updated");

			return Table[Format - FORMAT_FIRST];
		}
	};
}//namespace detail
}//namespace gli
