// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "version.hpp"
#include "json_util.hpp"

#include <iostream>

namespace pensar_digital
{
	namespace cpplib
	{
		std::istream& operator >> (std::istream& is, Version& v)
		{
			// Check if os is in binary mode.
			if (is.flags() & std::ios::binary)
			{
				return is >> v.mpublic >> v.mprotected >> v.mprivate;
			}
			else // json format
			{
				Json j;
				is >> j;
				v.mpublic    = j.at("public_interface_version"   ).get<VersionInt>();
				v.mprotected = j.at("protected_interface_version").get<VersionInt>();
				v.mprivate   = j.at("private_interface_version"  ).get<VersionInt>();
				return is;
			}
		}

		std::ostream& operator << (std::ostream& os, const Version& v)
		{
			// Check if os is in binary mode.
			if (os.flags() & std::ios::binary)
			{
				os << v.get_public() << "." << v.get_protected() << "." << v.get_private();
			}
			else // json format
			{
				Json j;
				j["public_interface_version"   ] = v.get_public    ();
				j["protected_interface_version"] = v.get_protected ();
				j["private_interface_version"  ] = v.get_private   ();
				os << j;
			}
			return os;
		}
	}
}