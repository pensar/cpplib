// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef JSON_UTIL_HPP
#define JSON_UTIL_HPP

#include "concept.hpp"
#include "string_def.hpp"
#include "header_lib/json.hpp"
#include "version.hpp" 
#include "type_util.hpp"

#include <iostream>

namespace pensar_digital
{ 
    namespace cpplib
    {
        using Json = nlohmann::json;

        // Jsonable concept. Requires a member function json() returning something convertible to S.
        template <typename T>
        concept Jsonable = requires (T t)
        {
            {t.json()} noexcept -> std::convertible_to<S>;
        };

        template<class T>
        Id id(const S& sjson, Json* j)
        {
            if (j == nullptr)
            {
                auto k = Json::parse(sjson);
                S json_class = k.at("class");
                if (json_class != pd::class_name<T, char>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<T, char>());
                return k.at("id");
            }
            else
            {
                *j = Json::parse(sjson);
                S json_class = j->at("class");
                if (json_class != pd::class_name<T, char>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<T, char>());
                return j->at("id");
            }

        }
        template <class T, typename IdType = Id>
        T& read_json(const S& sjson, T& o, IdType* out_id, VersionPtr* out_v, Json* out_j = nullptr)
        {
            *out_id = (id<T>(sjson, out_j));
            *out_v = Version::get(*out_j);
            return o;
        }

        template <class T, typename IdType = Id>
        std::istream& read_json(std::istream& is, T& o, IdType* out_id, VersionPtr* out_v, Json* out_j = nullptr)
        {
            S sjson;
            read_json (read_all (is, sjson), o, out_id, out_v, out_j);
            return is;
        }

        template <Versionable T>
        S json (const T& o)
        {
            std::stringstream ss;
            ss << "{ \"class\" : \"" << o.class_name();
            ss << "\", \"id\" : " << o.id() << ", \"VERSION\": ";
            ss << *(o.VERSION);
            return ss.str();
        }
    } // namespace cpplib
} // namespace pensar_digital
    
#endif // IOBJECT_HPP
