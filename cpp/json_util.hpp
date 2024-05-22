// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef JSON_UTIL_HPP
#define JSON_UTIL_HPP

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>

using namespace rapidjson;

#include "concept.hpp"
#include "s.hpp"
#include "header_lib/json.hpp"
#include "type_util.hpp"

#include <iostream>

namespace pensar_digital
{ 
    namespace cpplib
    {
        #ifdef WIDE_CHAR
            typedef GenericDocument<UTF16<> > JsonDoc;
            typedef GenericValue<UTF16<> > JsonValue;
            typedef GenericStringBuffer<UTF16<> > JsonStrBuffer;
            typedef GenericWriter<UTF16<> > JsonWriter;
        #else
            typedef GenericDocument<UTF8<> > JsonDoc;   
			typedef GenericValue<UTF8<> > JsonValue;
			typedef GenericStringBuffer<UTF8<> > JsonStrBuffer;
			typedef Writer<UTF8<> > JsonWriter;
        #endif
        using JsonObject = rapidjson::Value::Object;
        using Json = nlohmann::json;

        // Jsonable concept. Requires a member function json() returning something convertible to S.
        template <typename T>
        concept Jsonable = requires (T t)
        {
            {t.json()} noexcept -> std::convertible_to<S>;
        };
        
        inline JsonDoc parse_json(const S& sjson)
		{
			JsonDoc d;
			d.Parse(sjson.c_str());
            if (d.HasParseError())
            {
                S err = W("Object::get (const S& sjson) : Invalid json string. Error code = ") + pd::to_string(d.GetParseError());
                //LOG(ss.str());
                throw std::runtime_error(err);
            }
			return d;
		}

        template <Identifiable T>
        const JsonDoc& parse(T& o, const JsonDoc&  d)
        {
            S class_name = pd::class_name<T>();
            if (!d.HasMember (W("class")))
                throw std::runtime_error(W("Object::get (const S& sjson) : Invalid json string. Missing \"class\""));
            S json_class = d[W("class")].GetString ();
            
            if (json_class != class_name)
                throw std::runtime_error(W("parse<T> (const S& sjson) : Invalid class name: expected ") + json_class + W(" but sjson had ") + class_name);

            Id id = d["id"].GetInt();

            o.set_id(id);

            return d;
        }
        
        /// <summary>
        /// Saves vector of objects to an output stream. and closes the stream.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="os"></param>
        /// <param name="ops"></param>
        /// <returns></returns>
        template <OutputStreamable T>
        OutStream& save (OutStream os, const std::vector<std::shared_ptr<T>>& v)
		{
            os << W("[");
            for (Id i = 0; i < v.size () - 1; i++)
            {
                os << *(v[i]) << W(", ") << std::endl;
            }
            os << *(v[v.size () - 1]) << W("]");
            os.flush ();
            return os;
		}

        template <Identifiable T>
        JsonDoc parse (T& o, const S& sjson)
		{
            JsonDoc d =  parse_json(sjson);

			return parse<T> (o, d);
		}

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
    } // namespace cpplib
} // namespace pensar_digital
    
#endif // IOBJECT_HPP
