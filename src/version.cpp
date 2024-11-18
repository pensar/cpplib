// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "version.hpp"
#include "type_util.hpp"
#include "io_util.hpp"
#include "concept.hpp"
#include "s.hpp"

#include <iostream>

namespace pensar_digital
{
	namespace cpplib
	{
		InStream& operator >> (InStream& is, Version& v)
		{
			VersionInt pub, pro, pri;
			return is >> pub >> pro >> pri;
			v.set_public (pub);
			v.set_protected (pro);
			v.set_private (pri);
			
		}
				
		OutStream& operator << (OutStream& os, const Version& v)
		{
			return os << v.get_public() << W(".") << v.get_protected() << W(".") << v.get_private();
		}

		S Version::debug_string() const noexcept
		{
			return S();
		}
		
		bool Version::equals(const Version& v) const noexcept
		{
			static_assert (Hashable<Version>);
			return (v.id () == id ()) && (v.mdata.mprivate == mdata.mprivate) && (v.mdata.mprotected == mdata.mprotected) && (v.mdata.mpublic == mdata.mpublic);
		}

		const Hash Version::get_hash() const noexcept
		{
			return Hash (897896785686); // todo: implement hash.
		}
	
		std::istream& Version::binary_read (std::istream& is, const std::endian& byte_order)
		{
			return is.read((char*)(&mdata), DATA_SIZE);
		}

		std::ostream& Version::binary_write (std::ostream& os, const std::endian& byte_order) const
		{
			return os.write((const char*)(&mdata), DATA_SIZE);
		}
	} // namespace cpplib
} // namespace pensar_digital