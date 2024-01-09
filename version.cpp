// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "version.hpp"
#include "type_util.hpp"
#include "io_util.hpp"
#include "xml_util.hpp"
#include "concept.hpp"

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
			S class_name = v.class_name();
			S json_class = j.at("class");
			if (class_name == json_class)
			{
				v.set_id(j.at("id"));
				v.mdata.mpublic = j.at("mpublic");
				v.mdata.mprotected = j.at("mprotected");
				v.mdata.mprivate = j.at("mprivate");
			}
			else throw new std::runtime_error("Version expected class = " + class_name + " but json has " + json_class);
		}
		
		std::istream& operator >> (std::istream& is, Version& v)
		{
			// Check if os is in binary mode.
			if (is.flags() & std::ios::binary)
			{
				return is >> v.mdata.mpublic >> v.mdata.mprotected >> v.mdata.mprivate;
			}
			else // json format
			{
				Json j;
				is >> j;
				v.mdata.mpublic    = j.at("mpublic"   ).get<VersionInt>();
				v.mdata.mprotected = j.at("mprotected").get<VersionInt>();
				v.mdata.mprivate   = j.at("mprivate"  ).get<VersionInt>();
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

		S Version::debug_string() const noexcept
		{
			return S();
		}
		
		bool Version::equals(const Version& v) const noexcept
		{
			static_assert (Hashable<Version>);
			return (v.id () == id ()) && (v.mdata.mprivate == mdata.mprivate) && (v.mdata.mprotected == mdata.mprotected) && (v.mdata.mpublic == mdata.mpublic);
		}

		void Version::from_xml (const S& sxml)
		{
			XMLNode node = pd::parse_object_tag<Version> (sxml, &mdata.mid);
			XMLNode n = node.getChildNode("mprivate");
			if (!n.isEmpty())
				mdata.mprivate = atoi(n.getText());
			n = node.getChildNode("mprotected");
			if (!n.isEmpty())
				mdata.mprotected = atoi(n.getText());
			n = node.getChildNode("mpublic");
			if (!n.isEmpty())
				mdata.mpublic = atoi(n.getText());
		}
		 
		S Version::xml() const noexcept
		{
			S xml = pd::ObjXMLPrefix<Version>(*this) + ">";
			xml += "<mpublic>" + pd::to_string(mdata.mpublic) + "</mpublic>";
			xml += "<mprotected>" + pd::to_string(mdata.mprotected) + "</mprotected>";
			xml += "<mprivate>" + pd::to_string(mdata.mprivate) + "</mprivate>";

			xml += "</object>";
			return xml;
		}

		const Hash Version::get_hash() const noexcept
		{
			return Hash (897896785686); // todo: implement hash.
		}

		S Version::json() const noexcept
		{
			std::stringstream ss;
			ss << "{ \"class\" : \"" << pd::class_name<Version, char>() << "\" , \"id\" : " << id() << ", \"mpublic\" : " << mdata.mpublic << ", \"mprotected\" : " << mdata.mprotected << ", \"mprivate\" : " << mdata.mprivate << " }";
			return ss.str();
		}
	
		std::istream& Version::read (std::istream& is, const IO_Mode amode, const std::endian& byte_order)
		{
			if (amode == BINARY)
			{
				/* S sclass_name;
				binary_read(is, sclass_name, byte_order);
				if (sclass_name != class_name())
					throw new std::runtime_error("Version::read: class name mismatch.");
				binary_read<Id>        (is, mdata.mid, byte_order);
				binary_read<VersionInt>(is, mdata.mpublic, byte_order);
				binary_read<VersionInt>(is, mdata.mprotected, byte_order);
				binary_read<VersionInt>(is, mdata.mprivate, byte_order);
				*/
				is.read((char*)data(), data_size());
			}
			else // json format
			{
				S sjson;
				is >> sjson;
				auto j = Json::parse(sjson);
				S json_class = j.at("class");
				if (json_class != pd::class_name<Version, char>())
					throw std::runtime_error("Invalid class name: " + pd::class_name<Version, char>());

				mdata.mid        = j.at("id"        ).get<Id>        ();
				mdata.mpublic    = j.at("mpublic"   ).get<VersionInt>();
				mdata.mprotected = j.at("mprotected").get<VersionInt>();
				mdata.mprivate   = j.at("mprivate"  ).get<VersionInt>();
			}
			return is;
		}

		std::ostream& Version::write(std::ostream& os, const IO_Mode amode, const std::endian& byte_order) const
		{
			if (amode == BINARY)
			{
				/*
				binary_write(os, class_name(), byte_order);
				binary_write<Id> (os, mid, byte_order);
				binary_write<VersionInt>(os, mpublic);
				binary_write<VersionInt>(os, mprotected);
				binary_write<VersionInt>(os, mprivate);
				return os;
				*/
				os.write((const char*)data(), data_size());
			}
			else // json format
			{
				os << json ();
			}
			return os;
		}
	} // namespace cpplib
} // namespace pensar_digital