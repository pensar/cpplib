#ifndef CPP_COMMAND_HPP
#define CPP_COMMAND_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <exception>
#include <algorithm>
#include <type_traits>
#include <tuple>
#include <concepts>

#include "constant.hpp" // Id
#include "s.hpp"
#include "object.hpp"
#include "concept.hpp" // Identifiable
#include "generator.hpp"
#include "version.hpp"
#include "factory.hpp"
#include "bool.hpp"
#include "memory_buffer.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        //using namespace std;
        template<typename T>
        concept CommandConcept = requires(T t)
        {
            { t.run() } -> std::same_as<void>;
        }&& Identifiable<T>;

        template<class T>
        concept DerivedCommandConcept = requires(T t)
		{
			{ t._run() } -> std::same_as<void>;
		};

        template<typename T>
        concept UndoableCommandConcept = CommandConcept<T> && requires(T t)
        {
            { t.undo() } -> std::same_as<void>;
        };

        template<class T>
        concept DerivedUndoableCommandConcept = requires(T t)
        {
            { t._undo() } -> std::same_as<void>;
        };

        class Command : public Object
        {
            private:

            /// \brief Class Object::Data is compliant with the TriviallyCopyable concept. 
            /// \see https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable  
            struct Data : public pd::Data
            {
                Bool mok = Bool::UNKNOWN;
                Data(const Bool ok = Bool::UNKNOWN) noexcept : mok (ok) {}
            };
            // Asserts Data is a trivially copyable type.
            static_assert(TriviallyCopyable<Data>, "Data must be a trivially copyable type");
            Data mdata; //!< Member variable mdata contains the object data
            
            public:

            inline const static Data NULL_DATA = { Bool::UNKNOWN };
            using DataType = Data;
            using Factory =  pd::Factory<Command, Id, typename Command::DataType>;

            // Version of the class.
            inline static const VersionPtr VERSION = pd::Version::get(1, 1, 1);
            virtual const VersionPtr version() const noexcept { return VERSION; }

            using FactoryType = Factory;

            virtual const pd::Data* data() const noexcept { return &mdata; }
            virtual const BytePtr data_bytes() const noexcept { return (BytePtr)data(); }

            virtual size_t data_size() const noexcept { return sizeof(mdata); }
            virtual size_t size() const noexcept { return data_size() + version()->size(); }
            
            using G = Generator<Command, Id>; //!< Generator alias.

            inline static constexpr size_t DATA_SIZE = sizeof(mdata);
            inline static constexpr size_t      SIZE = Object::SIZE + DATA_SIZE + Version::SIZE + G::SIZE;

            protected:
            
            inline static G mgenerator = G();

            public:

            //inline static Factory mfactory = { 3, 10, NULL_ID, NULL_DATA }; //!< Member variable "factory"


            Command (const Id aid = NULL_ID, const Data& data = NULL_DATA) : Object(aid == NULL_ID ? mgenerator.get_id() : aid), mdata(data) {}

            // Move constructor
            Command (Command&&) = default;

            // Copy constructor
            Command (const Command&) = default;

			Command (MemoryBuffer& mb) : Object (mb)
            {
                assign_without_object (mb);
            }

            // Move assignment
            Command& operator= (Command&&) = default;

            // Copy assignment
            Command& operator= (const Command&) = default;

            //Destructor
            virtual ~Command () = default;

            // Implements initialize method from Initializable concept.
            virtual bool initialize (const Id id, const Data& data) noexcept
            {
                
                this->set_id(id == NULL_ID ? mgenerator.get_id() : id);
                mdata = data;

                return true;
            }

            virtual Command& assign_without_object(MemoryBuffer& mb) noexcept
            {
                Version v (mb);
                if (v != *VERSION)
                    log_and_throw(W("assign:Version mismatch"));
                mb.read_known_size((BytePtr)&mdata, DATA_SIZE);
				mgenerator = G(mb);
                return *this;
            }

            virtual Command& assign(MemoryBuffer& mb) noexcept
            {
                Object::assign(mb);
                return assign_without_object(mb);
            }
            inline virtual MemoryBufferPtr bytes() const noexcept
            {
				MemoryBufferPtr mb = std::make_unique<MemoryBuffer>(SIZE);  
                mb->append (*Object::bytes());
				mb->append (*VERSION->bytes());
                mb->write((BytePtr(&mdata)), DATA_SIZE);
				mb->append (*(mgenerator.bytes()));
                return mb;
			}

            inline virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order = std::endian::native) const
			{
				Object::binary_write (os, byte_order);
				//VERSION->binary_write (os, byte_order);
				//os.write((char*)&mdata, DATA_SIZE);
				mgenerator.binary_write(os, byte_order);
				return os;
			}

			inline virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order = std::endian::native)
			{
				Object::binary_read(is, byte_order);
				//VERSION->binary_read(is, byte_order);
				//is.read((char*)&mdata, DATA_SIZE);
				mgenerator.binary_read(is, byte_order);
				return is;
			}

            protected:

			virtual void _run() = 0;
            virtual void _undo() const = 0;
                    
            public:

            virtual void run () 
            {
                _run();
                mdata.mok = true;
            }

            void undo() const 
            {
                if (mdata.mok) // If the command was executed successfully, undo it.
                {
                    _undo();
                }
            }

            bool ok() const { return mdata.mok; }


            /*
            inline static typename FactoryType::P get(const Id aid = NULL_ID, const Data& data = NULL_DATA) noexcept
            {
                return mfactory.get(aid, data);
            }
            */
        };

        // NullCommand is a command that does nothing.
        class NullCommand : public Command
		{
            public:
                NullCommand () : Command(NULL_ID) { }
				NullCommand (MemoryBuffer& mb) : Command (mb) { }
				~NullCommand() = default;
				void _run() { }
				void _undo() const { }
        };
        inline static const NullCommand NULL_CMD = NullCommand();

        static_assert (Identifiable<NullCommand>);
        static_assert (CommandConcept<NullCommand>);
        static_assert (DerivedCommandConcept<NullCommand>);
        static_assert (UndoableCommandConcept<NullCommand>);
        static_assert (DerivedUndoableCommandConcept<NullCommand>);


        // CompositeCommand is a command that contains a list of commands to be executed in a transactional way.
        // If one of the commands fails, the entire transaction is rolled back.
        // The CompositeCommand is a Command and complies with the CommandConcept so itself can be part of another CompositeCommand.
		// The CompositeCommand will take ownership of the commands added to it.
		class CompositeCommand : public Command
		{
            public:
			inline static const size_t MAX_COMMANDS = 10;
	    	
            private:
                struct Data : public pd::Data
                {
                    std::array<Command*, MAX_COMMANDS> mcommands; //!< Member variable "commands" contains the list of commands to be executed.
                    size_t mindex = 0;
					void add(Command* cmd) 
                    {
						if (mindex >= MAX_COMMANDS)
						{
							log_and_throw("CompositeCommand::Data::add(Command* cmd) : Maximum number of commands reached.");
						}
                        mcommands[mindex++] = cmd; 
                    }

                    void free_commands()
                    {
                        // Deallocates commands in the array.
                        for (size_t i = 0; i < mindex; ++i)
                        {
                            delete mcommands[i];
                        }
                    }
                };
                static_assert(TriviallyCopyable<Data>, "Data must be a trivially copyable type");
                Data mdata; //!< Member variable mdata contains the object data.
            public:
                inline const static Data NULL_DATA = {};
        
            using DataType = Data;
            using Factory = pd::Factory<CompositeCommand, Id>;

            // Version of the class.
            inline static const VersionPtr VERSION = pd::Version::get(1, 1, 1);
            virtual const VersionPtr version() const noexcept { return VERSION; }

            using FactoryType = Factory;

            virtual const pd::Data* data() const noexcept { return &mdata; }
            virtual const BytePtr data_bytes() const noexcept { return (BytePtr)data(); }

            inline static constexpr size_t DATA_SIZE = sizeof(mdata);
            inline static constexpr size_t      SIZE = DATA_SIZE + Version::SIZE;

            virtual size_t data_size() const noexcept { return sizeof(mdata); }
            virtual size_t size() const noexcept { return data_size() + version()->size(); }
        public:
			CompositeCommand (const Id aid = NULL_ID) : Command (aid)
			{
			}   
			CompositeCommand (MemoryBuffer& mb) : Command(mb) { }
            ~CompositeCommand()
            {
                mdata.free_commands();
            }
            
            private:
            inline static Factory mfactory = { 3, 10, NULL_ID };

            public:

			inline virtual MemoryBufferPtr bytes() const noexcept
			{
				ByteSpan obs = Object::data_span();
				MemoryBufferPtr mb = std::make_unique<MemoryBuffer>(SIZE);
				MemoryBufferPtr version_bytes = VERSION->bytes();
				mb->write(obs.data(), obs.size());
				mb->append(*version_bytes);
				mb->write(data_bytes(), data_size());
				return mb;
			}

            // Implements initialize method from Initializable concept.
            virtual bool initialize (const Id id) noexcept
            {

                this->set_id (id == NULL_ID ? this->mgenerator.get_id() : id);

                return true;
            }

            inline static typename FactoryType::P get (const Id aid = NULL_ID) noexcept
            {
                return mfactory.get (aid);
            }
            
            void _run()
			{
				for (int_fast8_t i = 0; i < mdata.mindex; ++i)
				{
					mdata.mcommands[i]->run();
				}
			}
			void _undo() const
			{
                for (int_fast8_t i = 0; i < mdata.mindex; ++i)
                {
					mdata.mcommands[i]->undo();
				}
			}
			
            /// <summary>
			/// Adds the command to the composite.
			/// The CompositeCommand will take ownership of the command.
			/// </summary>
			/// <param name="cmd">Pointer to the command to be added.</param>
			void add (Command* cmd) { mdata.add (cmd); }
		};
 
    }
}

#endif // CPP_COMMAND_HPP

