// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "object.hpp"
#include "igenerator.hpp"
#include "constant.hpp"
#include "json_util.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        template <class T> class Generator;
        
        /// Makes Generator Streamable.
        template <class T> std::ostream& operator << (std::ostream& os, const IGenerator<T>& g) { return g.write(os); }
        template <class T> std::istream& operator >> (std::istream& is, IGenerator<T>& g) { return g.read(is); }

        /*
        template <class T>
        void to_json(Json& j, const IGenerator<T>& g)
        {
            j["class"     ] = g.class_name();
            j["id"        ] = g.get_id();
            j["mprivate"  ] = g.VERSION->get_private();
            j["mprotected"] = g.VERSION->get_protected();
            j["mpublic"   ] = g.VERSION->get_public();
        }

        template <class T>
        void from_json(const Json& j, Generator<T>& g)
        {
            String class_name = g.class_name();
            String json_class = j.at("class");
            if (class_name == json_class)
            {
                g.Object::set_id(j.at("id"));
                //g.name = j.at("name");
            }
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
        */

      /// Generator is meant to be used as unique identifier generator for other classes.
      ///
      /// Usually this is used as a static member for an entity class in conjunction with the Id mixin class like in the Person class:
      /// \code {.cpp}
      ///  template <class String = std::wstring, class T = int> class Person : public Name<String>, public Id<T>
      ///  {
      ///    public:
      ///      Person(const String& aname = L"", const T aid = -1): Name<String>(aname), Id<T>((aid == -1) ? generator.get() : aid){};
      ///      virtual ~Person(){};
      ///
      ///    static Generator<T> generator;
      ///
      ///  };
      /// \endcode
      template <class T>
      class Generator : public virtual Object//, public virtual IGenerator<T>
      {
        public:
            inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);

            typedef IGenerator<T>     I; // Interface type.
            typedef IGeneratorRO<T> IRO; // Read only interface type.

            /// \brief Constructs a Generator.
            /// \param [in] initial_value Initial value for the generator, defaults to 0.
            /// \param [in] astep Step to be used when incrementing the generator, defaults to 1.
            Generator(Id aid = NULL_ID, Id initial_value = 0, Id step = 1) : Object(aid), mvalue(initial_value), mstep(step) {};

            virtual ~Generator () = default;

            /// \brief Increments value and return the new value.
            /// \return The new value.
            virtual const Id get() { mvalue += mstep; return mvalue; }

            /// \brief Gets the next value without incrementing the current one.
            /// \return The next value.
            virtual const Id get_next() { return (mvalue + mstep); }

            /// \brief Gets the current value.
            /// \return The current value.
            virtual const Id get_current () const { return mvalue; }

            /// \brief Set value. Next call to get will get value + 1.
            /// \param val New value to set
            virtual void set_value(Id val) { mvalue = val; }

            // Conversion to json string.
            virtual String json() const noexcept
            {
                std::stringstream ss (pd::json<Generator<T>>(*this));
                ss << ", \"mvalue\": " << mvalue << ", \"mstep\": " << mstep << "}";
                return ss.str();
            }

            virtual std::istream& read (std::istream& is, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN)
            {
                if (amode == BINARY)
                {
                    // todo: implement binary read.
                }
                else // json format
                {
                    Json j;
                    Id id;
                    IVersionPtr v;
                    read_json<Generator<T>>(is, *this, &id, &v, &j);
                }
                return is;
            };

            virtual std::ostream& write (std::ostream& os, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN) const
            {
                if (amode == BINARY)
                {
                    // todo: implement binary write.
                }
                else // json format
                {
                    os << json ();
                }
                return os;
            };
            
            //friend void from_json<T>(const Json& j, Generator<T>& g);
            
            void set_id (const Id& aid) { Object::set_id (aid); }

            // Convertion to xml string.
            virtual String xml() const noexcept
            {
                String xml = ObjXMLPrefix() + ">";
                //xml += VERSION->xml(); //todo.
                xml += "<value>" + pd::to_string(mvalue) + "</value>";
                xml += "<step>" + pd::to_string(mstep) + "</step>";
                xml += "</object>";
                return xml;
            }   
            
            // Convertion from xml string.
            virtual void from_xml(const String& sxml)
            {
                XMLNode node = parse_object_tag(sxml);
                // todo: check version.
                XMLNode n = node.getChildNode("value");
                if (!n.isEmpty())
                    mvalue = atoi (n.getText());

                n = node.getChildNode("step");
				if (!n.isEmpty()) 
                    mstep = atoi (n.getText());
            }

        private:
            Id mvalue; //!< Member variable "id"
            Id mstep;  //!< Step to increment value.
      }; // class Generator
    } // namespace cpplib
} // namespace pensar_digital
/// \example GeneratorTest.cpp

#endif // GENERATOR_HPP_INCLUDED
