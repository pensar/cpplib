// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constant.hpp"
#include "s.hpp"
#include "clone_util.hpp"

#include "factory.hpp"
#include "log.hpp"
#include "string_def.hpp"
#include "memory_buffer.hpp"
#include "equal.hpp"

#include "concept.hpp"

#include <sstream>
#include <iostream>
#include <memory> // for std::shared_ptr
#include <concepts> // std::convertible_to
#include <string>
#include <typeinfo> // for typeid
#include <vector>
#include <span>
#include <cstddef>
#include <bit>
#include <cstring>

#include <bit> // for std::byteswap

namespace pensar_digital
{
    namespace cpplib
    {
        namespace pd = pensar_digital::cpplib;
        
        inline static void log_throw(const S& error_msg = W("")) 
        {
            // Log error.
            LOG(error_msg);
            throw std::runtime_error (error_msg);
        }

        template <class T> T& assigns (T& l, const T& r) noexcept 
        { 
            std::memcpy  (l.data (), ((T&)r).data (), sizeof(T::DataType)); 
            return l; 
        }

        template <class T> T& moves (T& l, const T& r) noexcept
        { 
            std::memmove (l.data (), ((T&)r).data (), sizeof(T::DataType));
            return l; 
        }
    
        struct ClassInfo
		{
            inline static const size_t MAX_IDENTIFIER_SIZE = 100; ///< Maximum size for identifier strings.
            using Identifier = CS<0, MAX_IDENTIFIER_SIZE>; ///< Type for identifiers, using a fixed-size character string.
            Identifier mnamespace;
            Identifier mclass_name;
			VersionInt mpublic_interface_version;
			VersionInt mprotected_interface_version;
			VersionInt mprivate_interface_version;
			inline static const VersionInt NULL_VERSION = -1; ///< Null version constant.

            ClassInfo(const S& ns = EMPTY, const S& cn = EMPTY, VersionInt pub_ver = NULL_VERSION, VersionInt pro_ver = NULL_VERSION, VersionInt pri_ver = NULL_VERSION) noexcept
                : mnamespace  (ns), 
                    mclass_name (cn), 
                    mpublic_interface_version    (pub_ver),
					mprotected_interface_version (pro_ver), 
                    mprivate_interface_version   (pri_ver) {}

            inline void write (MemoryBuffer& mb) const noexcept
            {
                mb.write((BytePtr)this, sizeof(ClassInfo));
			}

            inline void read (MemoryBuffer& mb) 
            {
                mb.read_known_size((BytePtr)this, sizeof(ClassInfo));
			}

            MemoryBuffer::Ptr bytes() const noexcept
            {
                MemoryBuffer::Ptr mb = std::make_unique<MemoryBuffer>(sizeof(ClassInfo));
                write(*mb);
                return mb;
            }

            inline bool operator==(const ClassInfo& other) const noexcept
            {
                return equal<ClassInfo> (*this, other);
            }

            inline bool operator!=(const ClassInfo& other) const noexcept
            {
                return !(*this == other);
			}

            inline void test_class_name_and_version(MemoryBuffer& mb) const
            {
                if (mb.size() < sizeof(ClassInfo))
                    log_throw(W("MemoryBuffer size is smaller than ClassInfo size."));
                
                ClassInfo info;
                info.read   (mb);
                
                if (info != *this)
                    log_throw(W("Version mismatch."));
			}

            inline const S to_s () const noexcept
            {
                SStream ss;
                ss << mnamespace << W("::") << mclass_name 
                   << W(" v") << mpublic_interface_version 
                   << W(".") << mprotected_interface_version 
                   << W(".") << mprivate_interface_version;
                return ss.str();
			}

            inline std::istream& binary_read(std::istream& is, const std::endian& byte_order)
            {
                return is.read((char*)(this), sizeof(ClassInfo));
            }

            inline std::ostream& binary_write(std::ostream& os, const std::endian& byte_order) const
            {
                return os.write((const char*) this, sizeof(ClassInfo));
            }

            inline void test_class_name_and_version (std:: istream& is, const std::endian& byte_order = std::endian::native) const
            {
                ClassInfo info;
                info.binary_read(is, byte_order);
                if (info != *this)
                    log_throw(W("Version mismatch."));
			}
        };

        static_assert(StdLayoutTriviallyCopyable<ClassInfo>, W("ClassInfo must be a trivially copyable type"));
        
        template<typename T>
        concept HasClassInfo = requires
        {
            // Check for static const C* members
            { T::INFO } -> std::same_as<ClassInfo>;

            // Ensure they are static and constant (implicit in the requires clause with ::)
                requires std::is_same_v<decltype(T::INFO), const ClassInfo>;
        };

        class Object
        {
		    public:
                inline static const ClassInfo INFO = { CPPLIB_NAMESPACE, W("Object"), 1, 1, 1 };
                inline virtual const ClassInfo* info_ptr() const noexcept { return &INFO; }
        private:
            /// \brief Class Object::Data is compliant with the TriviallyCopyable concept. 
            /// \see https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable  
            struct Data : public pd::Data
            {                
                Id mid;         //!< Unique id (among objects of the same class).
                Data(const Id& id = NULL_ID) noexcept : mid(id) {}
            };
            // Asserts Data is a trivially copyable type.
            static_assert(TriviallyCopyable<Data>, "Data must be a trivially copyable type");

            Data mdata; //!< Member variable mdata contains the object data.
        public:
			using Ptr = std::shared_ptr<Object>;
            inline const static Data NULL_DATA = { NULL_ID };
            using DataType = Data;
        private:
            using Factory = pd::Factory<Object, typename Object::DataType>;
		public:
            inline virtual pd::Data* get_null_data() const noexcept { return (pd::Data*)(&NULL_DATA); }

            using FactoryType = Factory;
            inline const BytePtr object_data_bytes() const noexcept { return (BytePtr)&mdata; }
            inline const size_t object_data_size() const noexcept { return sizeof(mdata); }

            virtual const pd::Data* data() const noexcept { return &this->mdata; }
            virtual const BytePtr data_bytes() const noexcept { return (BytePtr)&this->mdata; }

            inline static constexpr size_t DATA_SIZE = sizeof(mdata);
            inline static constexpr size_t      SIZE = DATA_SIZE + sizeof(ClassInfo);

            virtual size_t data_size() const noexcept { return sizeof(this->mdata); }
            virtual size_t size() const noexcept { return data_size() + sizeof(ClassInfo); }
        protected:
            /// Set id
            /// \param val New value to set
            void set_id(const Id& value) { mdata.mid = value; }
        private:
            inline static Factory mfactory = { 3, 10, NULL_DATA }; //!< Member variable "factory"

            // Set Factory as friend class to allow access to private members.
            friend class Factory;
        public:

            /// Default constructor.
            Object(const Data& data = NULL_DATA) noexcept
            {
                initialize(data);
            }

            /// Copy constructor
            /// \param other Object to copy from
            Object(const Object& o) { assign(o); }

            /// Move constructor
            Object(Object&& o) noexcept { assign(o); }

            Object(MemoryBuffer& mb)
            {
                object_assign(mb);
            }

            /** Default destructor */
            virtual ~Object() {}

            virtual Object& assign(const Object& o) noexcept { std::memcpy((void*)data(), ((Object&)o).data(), data_size()); return *this; }
            virtual Object& assign(const Object&& o) noexcept { std::memmove((void*)data(), ((Object&)o).data(), data_size()); return *this; }
           Object& object_assign(MemoryBuffer& mb) noexcept
            {
                INFO.test_class_name_and_version (mb);
                mb.read_known_size(object_data_bytes(), DATA_SIZE);
                return *this;
            }
            virtual Object& assign(MemoryBuffer& mb)
            {
				// Verifies if it is the correct class and version.
                if (mb.size() < SIZE)
					log_throw(W("MemoryBuffer size is smaller than Object size."));
				return object_assign (mb);
            }

 
            Object& object_assign(const Object& o) noexcept {
                mdata = o.mdata;
                return *this;
            }

            inline virtual const Object& write(MemoryBuffer& mb) const noexcept
            {
				info_ptr ()->write(mb);
                mb.write((BytePtr)(data ()), data_size ());
                return *this;
            }

            inline const Object& object_write(MemoryBuffer& mb) const noexcept
            {
                INFO.write(mb);
                mb.write((BytePtr)(&mdata), DATA_SIZE);
                return *this;
            }
            /*
            inline virtual void bytes_to_vector(ConstBytes& v) const noexcept
            {
                VERSION->bytes(v);
                size_t req_size = v.size() + data_size();
                if (v.capacity() < req_size)
                    v.resize(req_size);
                std::copy_n(reinterpret_cast<const std::byte*>(&mdata), data_size(), v.end() - data_size());
            }
            */

            inline MemoryBuffer::Ptr object_bytes() const noexcept
            {
                MemoryBuffer::Ptr mb = std::make_unique<MemoryBuffer>(SIZE);
                mb->append((BytePtr)&INFO, sizeof(ClassInfo));
                mb->append((BytePtr)(&mdata), DATA_SIZE);
                return mb;
            }

            /// \brief Returns a MemoryBuffer::Ptr with the object data.
            inline virtual MemoryBuffer::Ptr bytes() const noexcept
            {
               return object_bytes ();
            }

            // Implicit convertion to MemoryBuffer::Ptr.
            inline operator MemoryBuffer::Ptr() const noexcept
            {
                return bytes();
            }

            inline virtual ByteSpan data_span() const noexcept { return ByteSpan(data_bytes(), data_size()); }

            /// \brief Uses std::as_writable_bytes to get a span of writable bytes from the object.
            inline virtual std::span<std::byte> wbytes() noexcept
            {
                static_assert (sizeof(char) == sizeof(std::byte));

                auto byte_span = std::span<std::byte>((std::byte*)(data()), data_size());

                return std::as_writable_bytes(byte_span);
            }

            inline virtual std::string sclass_name() const
            {
                std::string s = typeid(*this).name();
                s.erase(0, sizeof("class ") - 1);
                return s;
            }

            inline virtual S class_name() const
            {
                std::string s = typeid(*this).name();
                s.erase(0, sizeof("class ") - 1);
#ifdef WIDE_CHAR
                return pd::to_wstring(s);
#else
                return s;
#endif  
            }

            // Clone method. 
            inline Object::Ptr clone() const noexcept { return pd::clone<Object>(*this, mdata.mid); }

            inline virtual Object* get_obj() const noexcept
            {
                return ((Object*)(&(*(mfactory.get(NULL_DATA)))));
            }

            /*inline virtual Object* clone() const noexcept
            {
                Object* o = get_obj();
                o->assign(*this);
                return o;
            }
            */

            inline virtual bool equals(const Object& o) const noexcept
            {
                return equal<Object>(*this, o);
            }

            /// Access object id
            /// \return The current value of id
            ///
            inline virtual const Id id() const noexcept { return mdata.mid; };

            /// \brief Access hash
            ///
            /// \return  The current value of hash
            inline virtual const Hash hash() const noexcept { return this->id(); };

            // Implements initialize method from Initializable concept.
            inline virtual bool initialize(const Data& data) noexcept
            {
                mdata = data;

                return true;
            }

            inline std::ostream& bin_write(std::ostream& os, const std::endian& byte_order = std::endian::native) const
            {
                INFO.binary_write (os, byte_order);
                os.write((const char*)(&mdata), DATA_SIZE);
                return os;
            };

            inline bool operator == (const Object& o) const { return  equals(o); }
            inline bool operator != (const Object& o) const { return !equals(o); }

            /// Conversion to string.
            /// \return A string with the object id.
            inline S to_string() const noexcept { return pd::to_string(mdata.mid); }

            /// Implicit conversion to string.
            /// \return A string with the object id.
            inline operator S () const noexcept { return to_string(); }

            /// Debug string.
            /// \return A string with the object id.
            inline virtual S debug_string() const noexcept
            {
                SStream ss;
                ss << W("id = ") << Object::to_string();
                return ss.str();

            }

            /// Assignment operator
            /// \param o Object to assign from
            /// \return A reference to this
            inline Object& operator=(const Object& o) noexcept { return assign(o); }

            /// Move assignment operator
            inline Object& operator=(Object&& o) noexcept { return assign(o); }

            static inline Factory::P  get(const Data& data = NULL_DATA)
            {
                return mfactory.get(data);
            };

            static inline Factory::P  get(const Id& id)
            {
                return mfactory.get(Data(id));
            };

            static inline Factory::P get (MemoryBuffer& mb)
            {
                Factory::P o_ptr = get();
                Object& o = *o_ptr;
                o.assign(mb);
                return o_ptr;
            };

            inline virtual InStream& read(InStream& is) { return is >> mdata.mid; }
            inline virtual OutStream& write(OutStream& os) const { return os << id(); }


            inline InStream& operator >> (InStream& is)
            {
                return read(is);
            }

            inline OutStream& operator << (OutStream& os)
            {
                return write(os);
            }

            inline std::istream& bin_read(std::istream& is, const std::endian& byte_order = std::endian::native)
            {
                INFO.test_class_name_and_version(is, byte_order); 
                is.read((char*)(&mdata), DATA_SIZE);
                return is;
            }

            // implements input stream member virtual std::istream& Object::read(std::istream& is)
            inline virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order = std::endian::native)
            {
                //read_bin_obj(is, byte_order);
                INFO.test_class_name_and_version(is, byte_order);
                is.read((char*)(&mdata), DATA_SIZE);
                info_ptr ()->test_class_name_and_version (is, byte_order);
                is.read((char*)data(), data_size());
                return is;
            };

            inline virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order = std::endian::native) const
            {
                bin_write(os, byte_order);                          // Writes Object.
                info_ptr ()->binary_write(os, byte_order);   // Writes the polymorphic Version.
                os.write((const char*)data(), data_size());         // Writes the polymorphic data.
                return os;
            }
        };  //  class Object.

        inline InStream& operator >> (InStream& is, Object& o)
        {
            return o.read(is);
        }

        inline OutStream& operator << (OutStream& os, const Object& o)
        {
            return o.write(os);
        }

// Object
        // CloneableConcept any class T with a T::Ptr clone (); method. where T::Ptr is a shared_ptr <T>.
        /*
        template <typename T>
        concept CloneableConcept = requires (T t)
        {
            { t.clone() } -> std::convertible_to<Object*>;
        };
        */


        /*
        template<typename Container>
        std::istream& binary_read(Container& c, std::istream& is, const std::endian& byte_order)
        {
            // Assuming you have a way to determine the number of elements to read.
            // For example, reading the size first:
            size_t size = 0;
            is.read(reinterpret_cast<char*>(&size), sizeof(size));
            if (byte_order != std::endian::native) {
                size = std::bit_cast<size_t>(std::byteswap<uint64_t>(std::bit_cast<uint64_t>(size)));
            }

            // Clear the existing contents of the container
            c.clear();

            // Reserve capacity if the container supports it (like std::vector)
            auto* as_vector = dynamic_cast<std::vector<Object::Ptr>*>(&c);
            if (as_vector) {
                as_vector->reserve(size);
            }

            for (size_t i = 0; i < size; ++i) {
                // Create a new Object::Ptr, read its data, and add it to the container
                Object::Ptr obj = Object::get ();
                obj->binary_read(is, byte_order); // Assuming Object has a binary_read method
                c.insert(c.end(), obj); // Insert the object into the container
            }

            return is;
        }

        template<typename Container>
        std::ostream& binary_write(const Container& c, std::ostream& os, const std::endian& byte_order)
        {
            // Write the size of the container first
            size_t size = c.size();
            if (byte_order != std::endian::native) {
                size = std::bit_cast<size_t>(byteswap(std::bit_cast<uint64_t>(size)));
            }
            os.write(reinterpret_cast<const char*>(&size), sizeof(size));

            // Write each object in the container
            for (const auto& obj : c) {
                obj->binary_write(os, byte_order); // Assuming Object has a binary_write method
            }

            return os;
        }

        template<typename Container>
        std::ostream& binary_write(const Container& c, std::ostream& os, const std::endian& byte_order)
        {
			// Write the size of the container first
			size_t size = c.size();
			if (byte_order != std::endian::native) {
				size = std::bit_cast<size_t>(byteswap(std::bit_cast<uint64_t>(size)));
			}
			os.write(reinterpret_cast<const char*>(&size), sizeof(size));

			// Write each object in the container
			for (const auto& obj : c) {
				obj->binary_write(os, byte_order); // Assuming Object has a binary_write method
			}

			return os;
		}   
        */


        //***
        // Dependency class is a Constrainable class used to define dependencies between objects.
            /*template <Versionable MainClass, Versionable RequiredClass>
            class Dependency
            {
            private:
                Version::Int required_public_interface_version;
                Version::Int required_protected_interface_version;
                Version::Int required_private_interface_version;
            public:
                Dependency(Version v) noexcept
                    : required_public_interface_version(v.get_public ()),
                    required_protected_interface_version(v.get_protected ()),
                    required_private_interface_version(v.get_private ()) {}
                virtual ~Dependency() {}
                virtual bool ok() const noexcept = 0;

                // method to set the class dependency. 
            };*/
         } // namespace cpplib
} // namespace pensar_digital
#endif // OBJECT_HPP
