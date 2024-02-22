// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef XML_UTIL_HPP_INCLUDED
#define XML_UTIL_HPP_INCLUDED

#include "constant.hpp"
#include "s.hpp"
#include "type_util.hpp"
#include "header_lib/xmlParser.h"

namespace pensar_digital
{
    namespace cpplib
    {
        // Concept requiring a class_name() method returning something convertible to String and a id() method returning something convertible to Id.
        template <class T>
        concept Objectable = requires (T t)    
        {
            { t.class_name() } -> std::convertible_to<S>;
            { t.id() } -> std::convertible_to<Id>;
        };


        template<Objectable T>
        S ObjXMLPrefix (const T& o) noexcept
        { 
            return "<object class_name = \"" + o.class_name() + "\" id = \"" + std::to_string (o.id ()) + "\""; 
        }

        template <class T>
        XMLNode parse_object_tag (const S& sxml, Id* id_from_xml)
        {
            const char* xml = sxml.c_str ();
            XMLCSTR tag = W("object");
            XMLResults* pResults = 0;
            XMLNode node = XMLNode::parseString (xml, tag, pResults);
            S xml_class_name = node.getAttribute (W("class_name"));
            if (xml_class_name == class_name<T> ())
            {
                S sid = node.getAttribute (W("id"));
                *id_from_xml = std::stoi (sid);
            }
            else
                throw std::runtime_error (W("Invalid class name"));
            return node;
        }


    }
}

#endif // XML_UTIL_HPP_INCLUDED
