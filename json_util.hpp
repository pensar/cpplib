// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef JSON_UTIL_HPP
#define JSON_UTIL_HPP

#include "concept.hpp"
#include "string_def.hpp"
#include "header_lib/json.hpp"
#include "iversion.hpp"
#include "version_factory.hpp" // versionf

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

        template<class T>
        Id get_id(const String& sjson, Json* j)
        {
            if (j == nullptr)
            {
                auto k = Json::parse(sjson);
                String json_class = k.at("class");
                if (json_class != pd::class_name<T>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<T>());
                return k.at("id");
            }
            else
            {
                *j = Json::parse(sjson);
                String json_class = j->at("class");
                if (json_class != pd::class_name<T>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<T>());
                return j->at("id");
            }

        }
        template <class T>
        T& read_json(const String& sjson, T& o, Id* out_id, IVersionPtr* out_v, Json* out_j = nullptr)
        {
            *out_id = (get_id<T>(sjson, out_j));
            *out_v = versionf.get(*out_j);
            return o;
        }

        template <class T>
        std::istream& read_json(std::istream& is, T& o, Id* out_id, IVersionPtr* out_v, Json* out_j = nullptr)
        {
            String sjson;
            is >> sjson;
            read_json(sjson, o, out_id, out_v, out_j);
            return is;
        }

        template <Versionable T>
        String json (const T& o)
        {
            std::stringstream ss;
            ss << "{ \"class\" : \"" << o.class_name();
            ss << "\", \"id\" : " << o.get_id() << ", \"VERSION\": ";
            ss << *(o.VERSION);
            return ss.str();
        }
    } // namespace cpplib
} // namespace pensar_digital
    
#endif // IOBJECT_HPP
