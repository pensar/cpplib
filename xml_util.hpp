// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef XML_UTIL_HPP_INCLUDED
#define XML_UTIL_HPP_INCLUDED

#include "constant.hpp"
#include "string_util.hpp"
#include "iobject.hpp"
#include "type_util.hpp"
#include "header_lib/xmlParser.h"

namespace pensar_digital
{
    namespace cpplib
    {
        inline String ObjXMLPrefix (const IObjectRO& o) noexcept 
        { 
            return "<object class_name = \"" + o.class_name() + "\" id = \"" + std::to_string (o.get_id ()) + "\""; 
        }

        template <class T>
        XMLNode parse_object_tag (const String& sxml, Id* id_from_xml)
        {
            const char* xml = sxml.c_str ();
            XMLCSTR tag = "object";
            XMLResults* pResults = 0;
            XMLNode node = XMLNode::parseString (xml, tag, pResults);
            String xml_class_name = node.getAttribute ("class_name");
            if (xml_class_name == class_name<T> ())
            {
                String sid = node.getAttribute ("id");
                *id_from_xml = std::stoi (sid);
            }
            else
                throw std::runtime_error ("Invalid class name");
            return node;
        }


    }
}

#endif // XML_UTIL_HPP_INCLUDED
