#ifndef ENCODE_HPP
#define ENCODE_HPP

#include "s.hpp"
#include "bool.hpp"
#include "array.hpp"
#include "endian.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		using BomBytes = CArray<4>;
		inline static BomBytes NO_BOM = { 0, 0, 0, 0 };
		inline static BomBytes BOM_UTF_8     = { 0xEF, 0xBB, 0xBF, 0x00 };
		inline static BomBytes BOM_UTF_16_BE = { 0xFE, 0xFF, 0x00, 0x00 };
		inline static BomBytes BOM_UTF_16_LE = { 0xFF, 0xFE, 0x00, 0x00 };
		inline static BomBytes BOM_UTF_32_BE = { 0x00, 0x00, 0xFE, 0xFF };
		inline static BomBytes BOM_UTF_32_LE = { 0xFF, 0xFE, 0x00, 0x00 };

		inline static void init_bom ()
		{
			NO_BOM._data_size = 0;
			BOM_UTF_8._data_size = 3;
			BOM_UTF_16_BE._data_size = 2;
			BOM_UTF_16_LE._data_size = 2;
			BOM_UTF_32_BE._data_size = 4;
			BOM_UTF_32_LE._data_size = 4;
		}

		struct Encoding {};

		template <size_t NameSize, Bool HasBOM = Bool::T>
		struct EncodingBase : public Encoding
		{
			CS<NameSize> name;
			Endian endian; // std::endian::big if big endian, std::endian::little if little endian.
			BomBytes bom; // Byte Order Mark.
		};

		const EncodingBase< 5, Bool::F> ASCII         = { Encoding (),     "ASCII"    , Endian::NOT_APPLICABLE, NO_BOM        };
		const EncodingBase< 5, Bool::F> UTF_8         = { Encoding (),     "UTF-8"    , Endian::NOT_APPLICABLE, NO_BOM        };
		const EncodingBase< 9, Bool::T> UTF_8_BOM     = { Encoding (), "UTF-8-BOM"    , Endian::NOT_APPLICABLE, BOM_UTF_8     };
		const EncodingBase< 9, Bool::F> UTF_16_BE     = { Encoding (), "UTF-16-BE"    , Endian::BIG           , NO_BOM        };
		const EncodingBase<13, Bool::T> UTF_16_BE_BOM = { Encoding (), "UTF-16-BE-BOM", Endian::BIG           , BOM_UTF_16_BE };
		const EncodingBase< 9, Bool::F> UTF_16_LE     = { Encoding (), "UTF-16-LE"    , Endian::LITTLE        , NO_BOM        };
		const EncodingBase<13, Bool::T> UTF_16_LE_BOM = { Encoding (), "UTF-16-LE-BOM", Endian::LITTLE        , BOM_UTF_16_LE };
		const EncodingBase< 9, Bool::F> UTF_32_BE     = { Encoding (), "UTF-32-BE"    , Endian::BIG           , NO_BOM        };
		const EncodingBase<13, Bool::T> UTF_32_BE_BOM = { Encoding (), "UTF-32-BE-BOM", Endian::BIG           , BOM_UTF_32_BE };
		const EncodingBase< 9, Bool::F> UTF_32_LE     = { Encoding (), "UTF-32-LE"    , Endian::LITTLE        , NO_BOM        };
		const EncodingBase<13, Bool::T> UTF_32_LE_BOM = { Encoding (), "UTF-32-LE-BOM", Endian::LITTLE        , BOM_UTF_32_LE };
			}; // namespace cpplib
} // namespace pensar_digital

#endif // ENCODE_HPP
