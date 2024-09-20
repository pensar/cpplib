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

        template<class DerivedCommand>
        class Command : public Object
        {
            public:
				using Cmd = Command<DerivedCommand>;
            private:

            /// \brief Class Object::Data is compliant with the TriviallyCopyable concept. 
            /// \see https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable  
            struct Data : public pd::Data
            {
                Bool mok = Bool::BOOL_NULL;
                Data(const Bool ok = Bool::BOOL_NULL) noexcept : mok (ok) {}
            };
            // Asserts Data is a trivially copyable type.
            static_assert(TriviallyCopyable<Data>, "Data must be a trivially copyable type");
            Data mdata; //!< Member variable mdata contains the object data
        public:
            inline const static Data NULL_DATA = { Bool::BOOL_NULL };
            using DataType = Data;
            using Factory =  pd::Factory<Cmd, Id, typename Cmd::DataType>;

            // Version of the class.
            inline static const VersionPtr VERSION = pd::Version::get(1, 1, 1);
            virtual const VersionPtr version() const noexcept { return VERSION; }

            using FactoryType = Factory;

            virtual const pd::Data* data() const noexcept { return &mdata; }
            virtual const BytePtr data_bytes() const noexcept { return (BytePtr)data(); }

            inline static constexpr size_t DATA_SIZE = sizeof(mdata);
            inline static constexpr size_t      SIZE = DATA_SIZE + Version::SIZE;

            using  CommandArray = std::array<Byte, SIZE>;
            using  CommandArrayPtr = std::unique_ptr<CommandArray>;

            virtual size_t data_size() const noexcept { return sizeof(mdata); }
            virtual size_t size() const noexcept { return data_size() + version()->size(); }

            using G = Generator<Cmd, Id>; //!< Generator alias.

            private:
            
                inline static G mgenerator = G();
                CommandArray marray;

            public:

            inline static Factory mfactory = { 3, 10, NULL_ID, NULL_DATA }; //!< Member variable "factory"


            Command (const Id aid = NULL_ID, const Data& data = NULL_DATA) : Object(aid == NULL_ID ? mgenerator.get_id() : aid), mdata(data) {}

            // Move constructor
            Command (Command&&) = default;

            // Copy constructor
            Command (const Command&) = default;

			Command (MemoryBuffer& mb) : Object (mb)
            {
                Version v (mb);
				if (v != *VERSION)
				{
					log_and_throw ("Command (MemoryBuffer&mb) : Version mismatch.");
				}
                
				mb.read (data_bytes(), data_size());
                mgenerator = G (mb);
                //std::memcpy (&mdata.mok, bytes.data() + sizeof(Id) + sizeof(G::Data), sizeof(bool));
            }

            // Move assignment
            Command& operator= (Command&&) = default;

            // Copy assignment
            Command& operator= (const Command&) = default;

            //Destructor
            virtual ~Command () = default;

            inline virtual MemoryBufferPtr bytes() noexcept
            {
                ByteSpan obs = Object::data_span();
                MemoryBufferPtr mb = std::make_unique<MemoryBuffer> (SIZE);
                MemoryBufferPtr version_bytes = VERSION->bytes();
                mb->write  (obs.data(), obs.size());
				mb->append (version_bytes);
				mb->write  (data_bytes(), data_size());
				mb->append (mgenerator.bytes());
                return mb;
			}

            void run () requires DerivedCommandConcept<DerivedCommand>
            {
                // Run DerivedCommand though Static Polymorphism.
                static_cast<DerivedCommand*>(this)->_run();
                mdata.mok = true;
            }

            void undo() const  requires DerivedUndoableCommandConcept<DerivedCommand>
            {
                if (mdata.mok) // If the command was executed successfully, undo it.
                {
                    static_cast<const DerivedCommand*>(this)->_undo();
                }
            }

            bool ok() const { return mdata.mok; }

            inline virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order) const
            {
                Object::binary_write(os, byte_order);
                mgenerator.binary_write(os, byte_order);
                VERSION->binary_write(os, byte_order);
                static_cast<const DerivedCommand*>(this)->binary_write(os, byte_order);
                return os;
            }

            inline virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order)
            {
                Object::binary_read(is, byte_order);
                mgenerator.binary_read(is, byte_order);
                VERSION->binary_read(is, byte_order);
                static_cast<DerivedCommand*>(this)->binary_read(is, byte_order);
                return is;
            }

            inline static typename FactoryType::P get(Id aid = NULL_ID) noexcept
            {
                return mfactory.get(aid);
            }
        };

        // NullCommand is a command that does nothing.
        class NullCommand : public Command<NullCommand>
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
        // The CompositeCommand complies with the CommandConcept so itself can be part of another CompositeCommand.
        // The CompositeCommand uses variadic templates to collect the commands to be executed.
        template<typename... Commands> requires (CommandConcept<Commands> && ...)
        class CompositeCommand : public Object
        {
            private:
            
                std::tuple<Commands ...> commands;

            public:

            CompositeCommand() = default;

            CompositeCommand(const Id aid = NULL_ID, Commands... cmds)
                : Object(aid), commands(std::make_tuple(cmds...)) {}

            CompositeCommand (MemoryBuffer& mb) : Object (mb)
			{
				Version v(mb);
				if (v != *VERSION)
				{
					log_and_throw("CompositeCommand (MemoryBuffer&mb) : Version mismatch.");
				}
				// Read the commands from the memory buffer.
				std::apply([this, &mb](auto&&... cmd) { ((cmd = Command<Commands>(mb), true) && ...); }, commands);
                
				//std::apply([this, &bytes](auto&&... cmd) { ((cmd = Command<Commands>(bytes), true) && ...); }, commands);
			}

            virtual ~CompositeCommand() = default;

            using Factory = pd::Factory<CompositeCommand<Commands...>, Id>;
            
            static Factory create_factory(Commands... cmds)
            {
                return Factory(3, 10, NULL_ID, cmds... );
            }
            
            virtual MemoryBufferPtr bytes() const noexcept
			{
				MemoryBufferPtr mb = Object::bytes();
                std::apply([&mb](auto&&... cmd) { ((mb->append(cmd.bytes()), true) && ...); }, commands);
                return mb;
                //return Object::bytes() + std::apply([](auto&&... cmd) { return (cmd.bytes(), ...); }, commands);
			}

            inline static Factory mfactory = create_factory (Commands{} ...); //!< Member variable "factory"

            using FactoryType = Factory;


            inline static typename FactoryType::P get(Id aid = NULL_ID, Commands... cmds) noexcept
            {
                return mfactory.get(aid, cmds...);
            }

            inline static typename FactoryType::P get() noexcept
            {
                return mfactory.get(NULL_ID, Commands{}...);
            }

            void run ()
            {
                try
                {
                    std::apply([this](auto&&... cmd) { ((cmd.run(), true) && ...); }, commands);
                }
                catch (...)
                {
                    undo ();
                    throw;
                }
            }

            void undo () const
            {
                std::apply([](auto&&... cmd) { ((cmd.undo(), true) && ...); }, commands);
            }

            inline virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order) const
            {
                Object::binary_write(os, byte_order);
                std::apply([&os, &byte_order](auto&&... cmd) { ((cmd.binary_write(os, byte_order), true) && ...); }, commands);
                return os;
            }

            inline virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order)
            {
                Object::binary_read(is, byte_order);
                std::apply([&is, &byte_order](auto&&... cmd) { ((cmd.binary_read(is, byte_order), true) && ...); }, commands);
                return is;
            }
        };

        // Specialization for CompositeCommand with no commands.
        template<>
        class CompositeCommand<> : public Object
		{
            public:
				CompositeCommand() = default;
				CompositeCommand(const Id aid = NULL_ID) : Object(aid) { }
				virtual ~CompositeCommand() = default;

				using Factory = pd::Factory<CompositeCommand<>, Id>;
				inline static Factory mfactory = { 3, 10, NULL_ID }; //!< Member variable "factory"

				using FactoryType = Factory;

				inline static typename FactoryType::P get(Id aid = NULL_ID) noexcept
				{
					return mfactory.get(aid);
				}

				void run () { }
				void undo () const { }

				inline virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order) const
				{
					Object::binary_write(os, byte_order);
					return os;
				}

				inline virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order)
				{
					Object::binary_read(is, byte_order);
					return is;
				}
		};

        static_assert (Identifiable<CompositeCommand<>>);
        static_assert (CommandConcept<CompositeCommand<>>);
        
        // Initialize the static member
        //template <CommandConcept... Cmds>
        //CompositeCommand<Cmds...> CompositeCommand<Cmds...>::mfactory = CompositeCommand<Cmds...>::create_factory(Cmds{} ...);
    };
}

#endif // CPP_COMMAND_HPP

