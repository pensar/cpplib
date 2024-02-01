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
      template <typename Type = Id, typename T = Id>
      class Generator : public Object     
      {
        public:
            typedef Type UsedByType;
            typedef    T     IdType;

            typedef std::shared_ptr<Generator<Type, T>> GeneratorPtr;
            typedef pd::Factory<Generator<Type, T>, T, T, T> Factory;

            inline static Factory mfactory = { 3, 10, null_value<T>(), 0, 1}; //!< Member variable "factory"
            inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);

            /// \brief Constructs a Generator.
            /// \param [in] initial_value Initial value for the generator, defaults to 0.
            /// \param [in] astep Step to be used when incrementing the generator, defaults to 1.
            Generator (T aid = null_value<T>(), T initial_value = 0, T step = 1) noexcept : Object(aid == null_value<T>() ? 0 : aid), mdata(initial_value, step) {};

            virtual ~Generator () = default;

            /// \brief Increments value and return the new value.
            /// \return The new value.
            inline virtual T get_id () { mdata.mvalue += mdata.mstep; return mdata.mvalue; }

            /// \brief Gets the next value without incrementing the current one.
            /// \return The next value.
            inline virtual const T next() { return (mdata.mvalue + mdata.mstep); }

            /// \brief Gets the current value.
            /// \return The current value.
            inline virtual const T current () const { return mdata.mvalue; }
            
            /// \brief Initialize a Generator.
            /// \param [in] initial_value Initial value for the generator, defaults to 0.
            /// \param [in] astep Step to be used when incrementing the generator, defaults to 1.
            virtual bool initialize(T aid = null_value<T>(), T initial_value = 0, T step = 1) noexcept
            {
                bool ok = Object::initialize(aid == null_value<T>() ? 0 : aid);
                mdata.minitial_value = initial_value;
                mdata.mvalue = initial_value;
                mdata.mstep = step;
                return ok;
            }
            
            /// \brief Create a new Generator using the factory method.

            /// \brief Set value. Next call to get will get value + 1.
            /// \param val New value to set
            inline virtual void set_value(T val) { mdata.mvalue = val; }

            // Conversion to json string.
            inline virtual String json() const noexcept
            {
                std::stringstream ss;
                ss << pd::json<Generator<Type, T>>(*this);
                ss << ", \"minitial_value\" : " << mdata.minitial_value << ", \"mvalue\" : " << mdata.mvalue << ", \"mstep\" : " << mdata.mstep << " }";
                return ss.str();
            }

            virtual std::istream& read(std::istream& is, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native)
            {
                if (amode == BINARY)
                {
                    //read_bin_obj (is, byte_order);
                    //read_bin_version (is, byte_order);
                    //binary_read<decltype (mdata.minitial_value)> (is, mdata.minitial_value, byte_order);
                    //binary_read<decltype (mdata.mvalue        )> (is, mdata.mvalue        , byte_order);   
                    //binary_read<decltype (mdata.mstep         )> (is, mdata.mstep         , byte_order);   
                    Object::read (is, BINARY, byte_order);
                    read_bin_version(is, *VERSION, byte_order);
                    is.read((char*)data(), data_size());
                }
                else // json format
                {
                    Json j;
                    Id id = NULL_ID;
                    VersionPtr v;
                    read_json<Generator<Type, T>>(is, *this, &id, &v, &j);
                    mdata.minitial_value = j["minitial_value" ];
                    mdata.mvalue         = j["mvalue"];
                    mdata.mstep          = j["mstep"  ];
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
                    VERSION->write(os, amode, byte_order);
                    os.write((const char*)data(), data_size());
                    //binary_write<decltype (mdata.minitial_value)> (os, mdata.minitial_value, byte_order);
                    //binary_write<decltype (mdata.mvalue)>         (os, mdata.mvalue        , byte_order);
                    //binary_write<decltype (mdata.mstep )>         (os, mdata.mstep         , byte_order);
                    //VERSION->write(os, amode, byte_order);
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
                xml += "<initial_value>" + pd::to_string<Id>(mdata.minitial_value, '.') + "</initial_value>";
                xml += "<value>"         + pd::to_string<Id>(mdata.mvalue, '.') + "</value>";
                xml += "<step>"          + pd::to_string<Id>(mdata.mstep, '.') + "</step>";
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
                    mdata.minitial_value = atoi(n.getText());

                n = node.getChildNode("value");
                if (!n.isEmpty())
                    mdata.mvalue = atoi (n.getText());

                n = node.getChildNode("step");
				if (!n.isEmpty()) 
                    mdata.mstep = atoi (n.getText());
            }

            Generator<Type, T>& parse_json(const String& s)
			{
				Json j = Json::parse(s);
				from_json<Type, T>(j, *this);
				return *this;
			}   
            
            static inline Factory::P  get (T aid = null_value<T>(), T initial_value = 0, T step = 1) noexcept
            {
                return mfactory.get (aid, initial_value, step);
            };

            Factory::P clone()
            {
                return get (get_id (), mdata.minitial_value, mdata.mstep);
            };

            inline static Factory::P get(const Json& j)
            {
                String json_class = j.at("class");
                if (json_class != pd::class_name<Generator<Type, T>>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<Generator<Type, T>>());

                typename Factory::P ptr = get (j.at("mid"), j.at("initial_value"), j.at("step"));

                VersionPtr v = Version::get(j["VERSION"]);

                if (*(ptr->VERSION) != *v)
                    throw std::runtime_error("Generator::Factory::parse_json: version mismatch.");

                return ptr;
            }

            inline static Factory::P get(const String& sjson)
            {
                Json j;
                T id = pd::id<Generator<Type, Id>>(sjson, &j);
                T initial_value = j.at("initial_value");
                T step          = j.at("step"         );
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
				return ((mdata.minitial_value == pother->mdata.minitial_value) && 
                        (mdata.mvalue         == pother->mdata.mvalue        ) && 
                        (mdata.mstep          == pother->mdata.mstep         ));
			}
        private:
            struct Data : public pd::Data
			{
				T minitial_value; //!< Generator initial_value.
				T mvalue        ; //!< Generator current value.
				T mstep         ; //!< Step to increment value.
                Data(T initial_value = 0, T step = 1) : minitial_value(initial_value), mvalue (initial_value), mstep (step) {}
            }; // struct Data
                Data mdata;
            public:
                typedef Data Datatype;
                Data* data() { return &mdata; } 
                virtual const pd::Data* data() const noexcept { return &mdata; }
                virtual size_t data_size() const noexcept { return sizeof(mdata); }
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
          j["mstep"] = g.mdata.mstep;

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
              g.mdata.mvalue = j["mvalue"];
              g.mdata.mstep = j["mstep"];
              g.VERSION->from_json(j);
          }
          else throw new std::runtime_error("Generator expected class = " + class_name + " but json has " + json_class);
      }

    } // namespace cpplib;

} // namespace pensar_digital
/// \example GeneratorTest.cpp

#endif // GENERATOR_HPP_INCLUDED
