// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "version.hpp"
#include "type_util.hpp"
#include "io_util.hpp"
#include "xml_util.hpp"
#include <iostream>

namespace pensar_digital
{
	namespace cpplib
	{

		void to_json(Json& j, const Version& v)
		{
			j["class"] = v.class_name();
			j["id"] = v.id();
			j["mpublic"] = v.get_public();
			j["mprotected"] = v.get_protected();
			j["mprivate"] = v.get_private();
		}

		void from_json(const Json& j, Version& v)
		{
			String class_name = v.class_name();
			String json_class = j.at("class");
			if (class_name == json_class)
			{
				v.set_id(j.at("id"));
				v.mpublic = j.at("mpublic");
				v.mprotected = j.at("mprotected");
				v.mprivate = j.at("mprivate");
			}
			else throw new std::runtime_error("Version expected class = " + class_name + " but json has " + json_class);
		}
		
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
				v.mpublic    = j.at("mpublic"   ).get<VersionInt>();
				v.mprotected = j.at("mprotected").get<VersionInt>();
				v.mprivate   = j.at("mprivate"  ).get<VersionInt>();
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
				os << v.json ();
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
		
		bool Version::equals(const Version& v) const noexcept
		{
			return (v.id () == id ()) && (v.mprivate == mprivate) && (v.mprotected == mprotected) && (v.mpublic == mpublic);
		}

		void Version::from_xml (const String& sxml)
		{
			XMLNode node = pd::parse_object_tag<Version> (sxml, &mid);
			XMLNode n = node.getChildNode("mprivate");
			if (!n.isEmpty())
				mprivate = atoi(n.getText());
			n = node.getChildNode("mprotected");
			if (!n.isEmpty())
				mprotected = atoi(n.getText());
			n = node.getChildNode("mpublic");
			if (!n.isEmpty())
				mpublic = atoi(n.getText());
		}
		 
		String Version::xml() const noexcept
		{
			String xml = pd::ObjXMLPrefix<Version>(*this) + ">";
			xml += "<mpublic>" + pd::to_string(mpublic) + "</mpublic>";
			xml += "<mprotected>" + pd::to_string(mprotected) + "</mprotected>";
			xml += "<mprivate>" + pd::to_string(mprivate) + "</mprivate>";

			xml += "</object>";
			return xml;
		}

		const Hash Version::get_hash() const noexcept
		{
			return Hash (897896785686); // todo: implement hash.
		}

		String Version::json() const noexcept
		{
			std::stringstream ss;
			ss << "{ \"class\" : \"" << pd::class_name<Version>() << "\" , \"id\" : " << id() << ", \"mpublic\" : " << mpublic << ", \"mprotected\" : " << mprotected << ", \"mprivate\" : " << mprivate << " }";
			return ss.str();
		}
	
		std::istream& Version::read (std::istream& is, const IO_Mode amode, const std::endian& byte_order)
		{
			if (amode == BINARY)
			{
				String sclass_name;
				binary_read(is, sclass_name, byte_order);
				if (sclass_name != class_name())
					throw new std::runtime_error("Version::read: class name mismatch.");
				binary_read<Id>        (is, mid, byte_order);
				binary_read<VersionInt>(is, mpublic, byte_order);
				binary_read<VersionInt>(is, mprotected, byte_order);
				binary_read<VersionInt>(is, mprivate, byte_order);
			}
			else // json format
			{
				String sjson;
				is >> sjson;
				auto j = Json::parse(sjson);
				String json_class = j.at("class");
				if (json_class != pd::class_name<Version>())
					throw std::runtime_error("Invalid class name: " + pd::class_name<Version>());
				mid = j.at("id").get<Id>();
				mpublic = j.at("mpublic").get<VersionInt>();
				mprotected = j.at("mprotected").get<VersionInt>();
				mprivate = j.at("mprivate").get<VersionInt>();
			}
			return is;
		}

		std::ostream& Version::write(std::ostream& os, const IO_Mode amode, const std::endian& byte_order) const
		{
			if (amode == BINARY)
			{
				binary_write (os, class_name (), byte_order);
				binary_write<Id> (os, mid, byte_order);
				binary_write<VersionInt>(os, mpublic);	
				binary_write<VersionInt>(os, mprotected);
				binary_write<VersionInt>(os, mprivate);	
				return os;
			}
			else // json format
			{
				return os << json ();
			}
		}
	} // namespace cpplib
} // namespace pensar_digital