// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef JSON_UTIL_HPP
#define JSON_UTIL_HPP

#include "concept.hpp"
#include "string_def.hpp"
#include "header_lib/json.hpp"

#include <iostream>

namespace pensar_digital
{ 
    namespace cpplib
    {
        using Json = nlohmann::json;

        // Jsonable concept. Requires a member function json() returning something convertible to String.
        template <typename T>
        concept Jsonable = requires (T t)
        {
            {t.json()} noexcept -> std::convertible_to<String>;
        };

        template <class T>
        std::istream& read_json(std::istream& is, T& o)
        {
            String sjson;
            is >> sjson;
            Json j = Json::parse(sjson);
            j.get_to(o);
            return is;
        }

        template <class T> 
        std::ostream& write_json(std::ostream& os, const T& o) 
        {
            return os << o.json();
        }

        template <class T>
        String json (const T& o) { Json j = o; return j.dump(); }

    } // namespace cpplib
} // namespace pensar_digital
    
#endif // IOBJECT_HPP
