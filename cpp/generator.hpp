// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "object.hpp"
#include "constant.hpp"
#include "string_def.hpp"
#include "s.hpp"
#include "factory.hpp"
#include "version.hpp"

#include <memory>   // shared_ptr
#include <iosfwd>   // istream, ostream
#include <bit>      // endian

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
      ///      Person(const String& aname = W("", const T aid = -1): Name<String>(aname), Id<T>((aid == NULL_ID) ? generator.get_id () : aid){};
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

            using G = Generator<Type, T>; //!< Generator alias.
            using GeneratorPtr = std::shared_ptr<G>;
            using Factory = pd::Factory<G, T, T, T>;

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

            virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order = std::endian::native)
            {
                    //read_bin_obj (is, byte_order);
                    //read_bin_version (is, byte_order);
                    //binary_read<decltype (mdata.minitial_value)> (is, mdata.minitial_value, byte_order);
                    //binary_read<decltype (mdata.mvalue        )> (is, mdata.mvalue        , byte_order);   
                    //binary_read<decltype (mdata.mstep         )> (is, mdata.mstep         , byte_order);   
                Object::binary_read (is, byte_order);
                read_bin_version(is, *VERSION, byte_order);
                is.read((char*)data(), data_size());
                return is;
            };

            virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order = std::endian::native) const
            {
                Object::binary_write(os, byte_order);
                VERSION->binary_write(os, byte_order);
                os.write((const char*)data(), data_size());
                //binary_write<decltype (mdata.minitial_value)> (os, mdata.minitial_value, byte_order);
                //binary_write<decltype (mdata.mvalue)>         (os, mdata.mvalue        , byte_order);
                //binary_write<decltype (mdata.mstep )>         (os, mdata.mstep         , byte_order);
                //VERSION->write(os, amode, byte_order);
                return os;
            };

            void set_id (const T& aid) { Object::set_id (aid); }

           
            static inline Factory::P  get (T aid = null_value<T>(), T initial_value = 0, T step = 1) noexcept
            {
                return mfactory.get (aid, initial_value, step);
            };

            Factory::P clone()
            {
                return get (get_id (), mdata.minitial_value, mdata.mstep);
            };

            inline virtual InStream& read(InStream& is) 
            {
                Object::read(is);   
                is >> mdata.minitial_value; 
                is >> mdata.mstep;
                is >> mdata.mvalue;
                return is;
            }

            inline virtual OutStream& write (OutStream& os) const { Object::write(os); return os << mdata.minitial_value
                                                                                                 << mdata.mstep << W(" ")
                                                                                                 << mdata.mvalue; }

            //friend InStream& operator >> <Type, T> (InStream& is, G& p);
            //friend OutStream& operator<< <Type, T> (OutStream& os, const G& p);

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
      template <class Type, typename T> OutStream& operator << (OutStream& os, const Generator<Type, T>& g) { return g.write (os); }
      template <class Type, typename T> InStream&  operator >> (InStream&  is,       Generator<Type, T>& g) { return g.read  (is); }

     } // namespace cpplib;

} // namespace pensar_digital
/// \example GeneratorTest.cpp

#endif // GENERATOR_HPP_INCLUDED
