// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef JSON_UTIL_HPP
#define JSON_UTIL_HPP

/*#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>

using namespace rapidjson;
*/
#include "concept.hpp"
#include "s.hpp"
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
                S json_class = k.at(W("class"));
                if (json_class != pd::class_name<T>())
                    throw std::runtime_error(W("Invalid class name: ") + pd::class_name<T>());
                return k.at(W("id"));
            }
            else
            {
                *j = Json::parse(sjson);
                S json_class = j->at(W("class"));
                if (json_class != pd::class_name<T>())
                    throw std::runtime_error(W("Invalid class name: ") + pd::class_name<T>());
                return j->at(W("id"));
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
        InStream& read_json(InStream& is, T& o, IdType* out_id, VersionPtr* out_v, Json* out_j = nullptr)
        {
            S sjson;
            read_json (read_all (is, sjson), o, out_id, out_v, out_j);
            return is;
        }

        template <Versionable T>
        S json (const T& o)
        {
            SStream ss;
            ss << W("{ \"class\" : \"") << o.class_name();
            ss << W("\", \"id\" : ") << o.id() << W(", \"VERSION\": ");
            ss << *(o.VERSION);
            return ss.str();
        }
    } // namespace cpplib
} // namespace pensar_digital
    
#endif // IOBJECT_HPP
