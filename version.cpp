// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "version.hpp"
#include "json_util.hpp"
#include "iobject.hpp"
#include "type_util.hpp"

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
		String Version::class_name() const
		{
			return pd::class_name<Version> ();
		}

		String Version::debug_string() const noexcept
		{
			return String();
		}
		bool Version::equals(const IObjectRO& o) const noexcept
		{
			// Check if o is a Version.
			if (typeid(o) != typeid(Version))
			{
				return false;
			}
			// Cast o to Version.
			const Version& v = static_cast<const Version&>(o);
			return (v.mid == mid) && (v.mprivate == mprivate) && (v.mprotected == mprotected) && (v.mpublic == mpublic);	
		}
		const Id Version::get_id() const noexcept
		{
			return mid;
		}
		const Hash Version::get_hash() const noexcept
		{
			return Hash(897896785686); // todo: implement hash.
		}

		String Version::json() const noexcept
		{
			return pd::json<Version>(*this);
		}
		std::ostream& Version::write(std::ostream& os) const
		{
			return write_json<Version>(os, *this);
		}
		bool Version::operator==(const IObjectRO& o) const
		{
			return equals (o);
		}
		bool Version::operator!=(const IObjectRO& o) const
		{
			return ! equals (o);
		}


		void to_json(Json& j, const Version& v)
		{
			j["class"                      ] = v.class_name    ();
			j["id"                         ] = v.get_id        ();
			j["private_interface_version"  ] = v.get_private   ();
			j["protected_interface_version"] = v.get_protected ();
			j["public_interface_version"   ] = v.get_public    ();
		}

		void from_json(const Json& j, Version& v)
		{
			String class_name = v.class_name();
			String json_class = j.at("class");
			if (class_name == json_class)
			{
				v.mid        = j.at ("id"                         );
				v.mprivate   = j.at ("private_interface_version"  );
				v.mprotected = j.at ("protected_interface_version");
				v.mpublic    = j.at ("public_interface_version"   );	
			}
			else throw new std::runtime_error("Version expected class = " + class_name + " but json has " + json_class);
		}
	} // namespace cpplib
} // namespace pensar_digital