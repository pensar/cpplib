// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constant.hpp"
#include "s.hpp"
#include "clone_util.hpp"
#include "version.hpp"
#include "factory.hpp"
#include "log.hpp"
#include "string_def.hpp"
#include "memory_buffer.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <memory> // for std::shared_ptr
#include <concepts>
#include <exception>
#include <string>
#include <typeinfo> // for typeid
#include <vector>
#include <span>
#include <cstddef>
#include <bit>

namespace pensar_digital
{
    namespace cpplib
    {
        namespace pd = pensar_digital::cpplib;
        class Object;
        typedef std::shared_ptr<Object> ObjectPtr;
        
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

        class Object
        {
            private:
                /// \brief Class Object::Data is compliant with the TriviallyCopyable concept. 
                /// \see https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable  
                struct Data : public pd::Data
                {
                    Id mid;         //!< Unique id (among same class).
                    Data (const Id& id = NULL_ID) noexcept : mid(id) {}
                };
                // Asserts Data is a trivially copyable type.
                static_assert(TriviallyCopyable<Data>, "Data must be a trivially copyable type");
                Data mdata; //!< Member variable mdata contains the object data.
            public:
                inline const static Data NULL_DATA = { NULL_ID};
                using DataType = Data;
                using Factory = pd::Factory<Object, typename Object::DataType> ;
                
                // Version of the class.
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                virtual const VersionPtr version () const noexcept { return VERSION; }

                using FactoryType = Factory;

                virtual const pd::Data* data() const noexcept { return &mdata; }
                virtual const BytePtr data_bytes() const noexcept { return (BytePtr)data(); }

				inline static constexpr size_t DATA_SIZE = sizeof(mdata);
                inline static constexpr size_t      SIZE = DATA_SIZE + Version::SIZE;
				
                virtual size_t data_size() const noexcept { return sizeof(mdata); }
				virtual size_t size() const noexcept { return data_size() + version()->size () ; }
                
                /// Set id
                /// \param val New value to set
                void set_id(const Id& value) { mdata.mid = value; }
            private:
                inline static Factory mfactory = { 3, 10, NULL_DATA }; //!< Member variable "factory"
                
                // Set Factory as friend class to allow access to private members.
                friend class Factory;
            protected:

                /// \brief Compare objects.
                ///
                /// \param The object to be compared with.
                /// \return true if they are equal, false otherwise.
                /// \see equals
                ///
                inline virtual bool _equals(const Object& o) const 
                {
                    const Object* pother = dynamic_cast<const Object*>(&o);
                    if (pother == nullptr)
						return false;

                    return ! (std::memcmp (data (), o.data (), data_size ())); 
                }

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

                Object (MemoryBuffer& mb)
                {
					assign (mb);
                }

                /** Default destructor */
                virtual ~Object() {}

                virtual Object& assign (const Object&  o) noexcept { std::memcpy  ((void*)data(), ((Object&)o).data(), data_size ()); return *this; }
                virtual Object& assign (const Object&& o) noexcept { std::memmove ((void*)data(), ((Object&)o).data(), data_size ()); return *this; }
				virtual Object& assign (MemoryBuffer& mb) 
				{
					Version v(mb);
                    Version v2 = *VERSION;
					if (v != v2)
						log_throw(W("Version mismatch."));
					mb.read_known_size ((BytePtr)(&mdata), DATA_SIZE);
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

				inline virtual MemoryBufferPtr bytes () const noexcept
				{
                    MemoryBufferPtr mb = std::make_unique<MemoryBuffer> (SIZE);
                    MemoryBufferPtr version_bytes = VERSION->bytes();
                    mb->copy (*version_bytes);
                    mb->write ((BytePtr(&mdata)), DATA_SIZE);
                    return mb;
				}

                inline virtual ByteSpan data_span () const noexcept { return ByteSpan (data_bytes (), data_size()); }
                
                /// \brief Uses std::as_writable_bytes to get a span of writable bytes from the object.
                virtual std::span<std::byte> wbytes() noexcept
                {
                    static_assert (sizeof(char) == sizeof(std::byte));

                    auto byte_span = std::span<std::byte>((std::byte*)(data()), data_size());

                    return std::as_writable_bytes(byte_span);
				}
                virtual std::string sclass_name () const
				{
					std::string s = typeid(*this).name();
					s.erase(0, sizeof("class ") - 1);
					return s;
				}

                virtual S class_name() const
                {
                    std::string s = typeid(*this).name();
                    s.erase(0, sizeof("class ") - 1);
                    #ifdef WIDE_CHAR
					    return pd::to_wstring (s);
					#else
                        return s;
                    #endif  
                }

                // Clone method. 
                ObjectPtr clone() const noexcept { return pd::clone<Object>(*this, mdata.mid); }
                
                /// Check if passed object equals self.
                /// Derived classes must implement the _equals method. The hash compare logic is made on equals.
                /// _equals is called from template method equals and should only implement the specific comparison.
                /// \see _equals
                /// \return true if objects have the same id, false otherwise.
                bool equals(const Object& o) const noexcept
                {
                    if (hash() != o.hash())
                        return false;
                    return _equals(o);
                }

                /// Access object id
                /// \return The current value of id
                ///
                virtual const Id id () const noexcept { return mdata.mid; };

                /// \brief Access hash
                ///
                /// \return  The current value of hash
                virtual inline const Hash hash() const noexcept { return this->id(); };

                // Implements initialize method from Initializable concept.
                virtual bool initialize (const Data& data) noexcept
                { 
                    mdata = data;
                    
                    return true; 
                }

                //virtual void read_bin_obj(std::istream& is, const std::endian& byte_order = std::endian::native);

                void read_bin_version(std::istream& is, const Version& version, const std::endian& byte_order = std::endian::native);

                virtual  std::istream&  binary_read (std::istream&  is, const std::endian& byte_order = std::endian::native);
                inline std::istream& bin_read(std::istream& is, const std::endian& byte_order = std::endian::native)
                {
                    read_bin_version(is, *VERSION, byte_order);
                    is.read((char*)(&mdata), DATA_SIZE);
                    return is;
                };


                virtual std::ostream& binary_write (std::ostream& os, const std::endian& byte_order = std::endian::native) const;
                inline std::ostream& bin_write(std::ostream& os, const std::endian& byte_order = std::endian::native) const
                {
                    VERSION->binary_write(os, byte_order);
                    os.write((const char*)(&mdata), DATA_SIZE);
                    return os;
                };

                bool operator == (const Object& o) const { return  equals(o); }
                bool operator != (const Object& o) const { return !equals(o); }

                /// Conversion to string.
                /// \return A string with the object id.
                S to_string() const noexcept { return pd::to_string(mdata.mid); }

                /// Implicit conversion to string.
                /// \return A string with the object id.
                operator S () const noexcept { return to_string(); }

                /// Debug string.
                /// \return A string with the object id.
                virtual S debug_string() const noexcept
                {
                    SStream ss;
                    ss << W("id = ") << Object::to_string();
                    return ss.str();

                }

                /// Assignment operator
                /// \param o Object to assign from
                /// \return A reference to this
                Object& operator=(const Object& o) noexcept { return assign(o); }

                /// Move assignment operator
                Object& operator=(Object&& o) noexcept { return assign(o); }

                static inline Factory::P  get(const Data& data = NULL_DATA)
                {
                    return mfactory.get(data);
                };

                static inline Factory::P  get(const Id& id)
				{
					return mfactory.get(Data (id));
				};

                inline virtual InStream&  read  (InStream&  is)       { return is >> mdata.mid; }
                inline virtual OutStream& write (OutStream& os) const { return os <<     id (); }
                 
        }; // Object
        
        template<typename Container>
        std::istream& binary_read(Container& c, std::istream& is, const std::endian& byte_order)
        {
            // Assuming you have a way to determine the number of elements to read.
            // For example, reading the size first:
            size_t size = 0;
            is.read(reinterpret_cast<char*>(&size), sizeof(size));
            if (byte_order != std::endian::native) {
                size = std::bit_cast<size_t>(std::byteswap(std::bit_cast<uint64_t>(size)));
            }

            // Clear the existing contents of the container
            c.clear();

            // Reserve capacity if the container supports it (like std::vector)
            auto* as_vector = dynamic_cast<std::vector<ObjectPtr>*>(&c);
            if (as_vector) {
                as_vector->reserve(size);
            }

            for (size_t i = 0; i < size; ++i) {
                // Create a new ObjectPtr, read its data, and add it to the container
                ObjectPtr obj = Object::get ();
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
				size = std::bit_cast<size_t>(std::byteswap(std::bit_cast<uint64_t>(size)));
			}
			os.write(reinterpret_cast<const char*>(&size), sizeof(size));

			// Write each object in the container
			for (const auto& obj : c) {
				obj->binary_write(os, byte_order); // Assuming Object has a binary_write method
			}

			return os;
		}   

        inline InStream& operator >> (InStream& is, Object& o) 
        { 
			return o.read (is);
        }
    
        inline OutStream& operator << (OutStream& os, const Object& o)
        { 
            return o.write (os);
        }

            // Dependency class is a Constrainable class used to define dependencies between objects.
            /*template <Versionable MainClass, Versionable RequiredClass>
            class Dependency
            {
            private:
                VersionInt required_public_interface_version;
                VersionInt required_protected_interface_version;
                VersionInt required_private_interface_version;
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
