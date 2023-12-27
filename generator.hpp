// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "object.hpp"
#include "constant.hpp"
#include "json_util.hpp"
#include "io_util.hpp"

#include <memory>   // shared_ptr

namespace pensar_digital
{
    namespace cpplib
    {
      /// Generator is meant to be used as unique identifier generator for other classes.
      ///
      /// Usually this is used as a static member for an entity class in conjunction with the Id mixin class like in the Person class:
      /// \code {.cpp}
      ///  template <class String = std::wstring, class T = int> class Person : public Name<String>, public Id<T>
      ///  {
      ///    public:
      ///      Person(const String& aname = L"", const T aid = -1): Name<String>(aname), Id<T>((aid == NULL_ID) ? generator.get_id () : aid){};
      ///      virtual ~Person(){};
      ///
      ///    static Generator<T> generator;
      ///
      ///  };
      /// \endcode
      template <typename Type = void, typename T = Id>
      class Generator : public Object     
      {
        public:
            typedef Type UsedByType;
            typedef    T     IdType;

            typedef std::shared_ptr<Generator<Type, T>> GeneratorPtr;
            typedef pd::Factory<Generator<Type, T>, T, T, T> Factory;

            inline static Factory mfactory = { 3, 10, NULL_ID, 0, 1 }; //!< Member variable "factory"
            inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);

            /// \brief Constructs a Generator.
            /// \param [in] initial_value Initial value for the generator, defaults to 0.
            /// \param [in] astep Step to be used when incrementing the generator, defaults to 1.
            Generator (T aid = NULL_ID, T initial_value = 0, T step = 1) noexcept : Object(aid), minitial_value(initial_value), mvalue(initial_value), mstep(step) {};

            virtual ~Generator () = default;

            /// \brief Increments value and return the new value.
            /// \return The new value.
            inline virtual const T get_id () { mvalue += mstep; return mvalue; }

            /// \brief Gets the next value without incrementing the current one.
            /// \return The next value.
            inline virtual const T next() { return (mvalue + mstep); }

            /// \brief Gets the current value.
            /// \return The current value.
            inline virtual const T current () const { return mvalue; }
            
            /// \brief Initialize a Generator.
            /// \param [in] initial_value Initial value for the generator, defaults to 0.
            /// \param [in] astep Step to be used when incrementing the generator, defaults to 1.
            virtual bool initialize(T aid = NULL_ID, T initial_value = 0, T step = 1) noexcept
            {
                bool ok = Object::initialize(aid);
                minitial_value = initial_value;
                mvalue = initial_value;
                mstep = step;
                return ok;
            }
            
            /// \brief Create a new Generator using the factory method.

            /// \brief Set value. Next call to get will get value + 1.
            /// \param val New value to set
            inline virtual void set_value(T val) { mvalue = val; }

            // Conversion to json string.
            inline virtual String json() const noexcept
            {
                std::stringstream ss;
                ss << pd::json<Generator<Type, T>>(*this);
                ss << ", \"minitial_value\" : " << minitial_value << ", \"mvalue\" : " << mvalue << ", \"mstep\" : " << mstep << " }";
                return ss.str();
            }

            virtual std::istream& read(std::istream& is, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native)
            {
                if (amode == BINARY)
                {
                    read_bin_obj (is, byte_order);
                    read_bin_version (is, byte_order);
                    binary_read<decltype (minitial_value)> (is, minitial_value, byte_order);
                    binary_read<decltype (mvalue        )> (is, mvalue        , byte_order);   
                    binary_read<decltype (mstep         )> (is, mstep         , byte_order);   
                }
                else // json format
                {
                    Json j;
                    T id = NULL_ID;
                    VersionPtr v;
                    read_json<Generator<Type, T>>(is, *this, &id, &v, &j);
                    minitial_value = j["minitial_value"];
                    mvalue         = j["mvalue"        ];
                    mstep          = j["mstep"         ];
                }
                return is;
            };

            virtual std::wistream& read(std::wistream& is, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native)
            {
                return is;
            }

            virtual std::ostream& write(std::ostream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native) const
            {
                if (amode == BINARY)
                {
                    Object::write(os, amode, byte_order);
                    binary_write<decltype (minitial_value)> (os, minitial_value, byte_order);
                    binary_write<decltype (mvalue)>         (os, mvalue        , byte_order);
                    binary_write<decltype (mstep )>         (os, mstep         , byte_order);
                    VERSION->write(os, amode, byte_order);
                }
                else // json format
                {
                    os << json();
                }
                return os;
            };

            virtual std::wostream& write(std::wostream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native) const
            {
                return os;
            }

            void set_id (const T& aid) { Object::set_id (aid); }

            // Convertion to xml string.
            virtual String xml() const noexcept
            {
                String xml = ObjXMLPrefix() + ">";
                //xml += VERSION->xml(); //todo.
                xml += "<initial_value>" + pd::to_string<Id>(minitial_value, '.') + "</initial_value>";
                xml += "<value>"         + pd::to_string<Id>(mvalue, '.') + "</value>";
                xml += "<step>"          + pd::to_string<Id>(mstep, '.') + "</step>";
                xml += "</object>";
                return xml;
            }   
            
            // Convertion from xml string.
            virtual void from_xml(const String& sxml)
            {
                XMLNode node = parse_object_tag(sxml);
                // todo: check version.

                XMLNode n = node.getChildNode("initial_value");
                if (!n.isEmpty())
                    minitial_value = atoi(n.getText());

                n = node.getChildNode("value");
                if (!n.isEmpty())
                    mvalue = atoi (n.getText());

                n = node.getChildNode("step");
				if (!n.isEmpty()) 
                    mstep = atoi (n.getText());
            }

            Generator<Type, T>& parse_json(const String& s)
			{
				Json j = Json::parse(s);
				from_json<Type, T>(j, *this);
				return *this;
			}   
            
            static inline Factory::P  get (T aid = NULL_ID, T initial_value = 0, T step = 1) noexcept
            {
                return mfactory.get (aid, initial_value, step);
            };

            Factory::P clone()
            {
                return get (get_id (), minitial_value, mstep);
            };

            inline static Factory::P get(const Json& j)
            {
                String json_class = j.at("class");
                if (json_class != pd::class_name<Generator<Type, T>>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<Generator<Type, T>>());

                typename Factory::P ptr = get (j.at("mid"), j.at("minitial_value"), j.at("mstep"));

                VersionPtr v = Version::get(j["VERSION"]);

                if (*(ptr->VERSION) != *v)
                    throw std::runtime_error("Generator::Factory::parse_json: version mismatch.");

                return ptr;
            }

            inline static Factory::P get(const String& sjson)
            {
                Json j;
                T id = pd::id<Generator<Type, Id>>(sjson, &j);
                T initial_value = j.at("minitial_value");
                T step          = j.at("mstep"         );
                typename Factory::P ptr = get (id, initial_value, step);

                VersionPtr v = Version::get(j);

                // todo: check version compatibility.
                if (*(ptr->VERSION) != *v)
                    throw std::runtime_error("Factory::parse_json: version mismatch.");
                return ptr;
            } // parse_json


            friend void from_json(const Json& j, Generator<Type, T>& g);

        protected:
            bool _equals(const Object& other) const noexcept override
			{
				const Generator<Type, T>* pother = dynamic_cast<const Generator<Type, T>*>(&other);
				if (pother == nullptr)
					return false;
				return ((minitial_value == pother->minitial_value) && 
                        (mvalue         == pother->mvalue        ) && 
                        (mstep          == pother->mstep         ));
			}
        private:
            T minitial_value; //!< Generator initial_value.
            T mvalue        ; //!< Generator current value.
            T mstep         ; //!< Step to increment value.
      }; // class Generator

      /// Makes Generator Streamable.
      template <class Type, typename T> std::ostream& operator << (std::ostream& os, const Generator<Type, T>& g) { return g.write (os); }
      template <class Type, typename T> std::istream& operator >> (std::istream& is,       Generator<Type, T>& g) { return g.read  (is); }

      template <class Type, typename T> std::wostream& operator << (std::wostream& os, const Generator<Type, T>& g) { return g.write (os); }
      template <class Type, typename T> std::wistream& operator >> (std::wistream& is,       Generator<Type, T>& g) { return g.read  (is); }

      template <class Type, typename T>
      void to_json(Json& j, const Generator<Type, T>& g)
      {
          j["class"] = g.class_name();
          j["id"] = g.id();
          j["mvalue"] = g.get_current();
          j["mstep"] = g.mstep;

          to_json(j, *(g.VERSION));
      }

      template <class Type, typename T>
      void from_json(const Json& j, Generator<Type, T>& g)
      {
          String class_name = g.class_name();
          String json_class = j.at("class");
          if (class_name == json_class)
          {
              g.Object::set_id(j.at("id"));
              g.mvalue = j["mvalue"];
              g.mstep = j["mstep"];
              g.VERSION->from_json(j);
          }
          else throw new std::runtime_error("Generator expected class = " + class_name + " but json has " + json_class);
      }

    } // namespace cpplib;

} // namespace pensar_digital
/// \example GeneratorTest.cpp

#endif // GENERATOR_HPP_INCLUDED
