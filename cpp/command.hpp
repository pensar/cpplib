#ifndef CPP_COMMAND_HPP
#define CPP_COMMAND_HPP

#include <iostream>
#include <vector>
#include <memory>
// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include <exception>
#include <concepts>

#include "constant.hpp" // Id
#include "s.hpp"
#include "object.hpp"
#include "concept.hpp" // Identifiable


namespace pensar_digital
{
    namespace cpplib
    {

        template<typename T>
        concept CommandConcept = requires(T t) 
        {
            { t.run () } -> std::same_as<void>;
        } && Identifiable<T>;

        template<typename T>
        concept UndoableCommandConcept = CommandConcept<T> && requires(T t) 
		{
			{ t.undo () } -> std::same_as<void>;
		};

        class Command : public Object   
		{
            private:
                using G = Generator<Command, Id>; //!< Generator alias.

                inline static G mgenerator = G();
                struct Data : public pd::Data
                {
                    bool mok = false;
                };
                // Asserts Data is a trivially copyable type.
                static_assert(TriviallyCopyable<Data>, "Data must be a trivially copyable type");
                Data mdata; //!< Member variable mdata contains the object data.
            public:
                typedef Data DataType;

                // Version of the class.
                inline static const VersionPtr VERSION = pd::Version::get(1, 1, 1);
                inline virtual const VersionPtr version () const noexcept { return VERSION; }

                typedef Factory FactoryType;

            public:
                Command (const Id aid = NULL_ID) : Object(aid == NULL_ID ? mgenerator.get_id() : aid) {}
                
                // Move constructor
                Command(Command&&) = default;
                
                // Copy constructor
                Command(const Command&) = default;
                
                // Move assignment
                Command& operator= (Command&&) = default;
                
                // Copy assignment
                Command& operator= (const Command&) = default;
                
                //Destructor
                virtual ~Command () = default;

                virtual const pd::Data* data() const noexcept { return &mdata; }
                virtual const BytePtr bytes() const noexcept { return (BytePtr)data(); }
                virtual size_t data_size() const noexcept { return sizeof(mdata); }
            protected:
                virtual void _run () = 0;
                virtual void _undo () const = 0;
            public:
                void run () 
                { 
                    _run();
                    mdata.mok = true; 
                }
                
                void undo () const 
                { 
                    if (mdata.mok) // If the command was executed successfully, undo it.
                        _undo (); 
                }

                bool ok () const { return mdata.mok; }

                inline virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order) const
				{
					Object::binary_write(os, byte_order);
                    mgenerator.binary_write (os, byte_order);
					VERSION->binary_write(os, byte_order);
					os.write((const char*)data(), data_size());
					return os;
				};

                inline virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order)
                {
                    Object::binary_read(is, byte_order);
                    mgenerator.binary_read(is, byte_order);
                    VERSION->binary_read(is, byte_order);
                    is.read((char*)data(), data_size());
                    return is;
                };
        };

        static_assert(CommandConcept<Command>, "Command is not a CommandConcept.");

        class CompositeCommand : public Command
        {
        private:
            std::vector<std::unique_ptr<Command>> commands;

        public:
            CompositeCommand(const Id aid = NULL_ID) : Command(aid) {}
            virtual ~CompositeCommand() = default;
            bool ok = true;

            inline virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order) const
            {
				Object::binary_write(os, byte_order);
				for (const auto& command : commands)
				{
					command->binary_write(os, byte_order);
				}
				return os;
			};

            inline virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order)
			{
                Object::binary_read(is, byte_order);
                for (const auto& command : commands)
                {
                    command->binary_read(is, byte_order);
			    }
                return is;
            };

            // Add a command to the list of commands to be executed.
            template <CommandConcept T, typename... Args>
            Id add(Args&&... args)
            {
                auto command = std::make_unique<T>(std::forward<Args>(args)...);
                Id id = command->id();
                commands.push_back(std::move(command));
                return id;
            }

            void remove(const Id id)
            {
                auto it = std::find_if(commands.begin(), commands.end(), [id](const std::unique_ptr<Command>& command)
                    {
                        return command->id() == id;
                    });

                if (it != commands.end())
                {
                    commands.erase(it);
                }
            }

            void clear()
			{
				commands.clear();
			}

            size_t count() const
            {
                return commands.size();
            }

        protected:
            void _run () override
            {
                for (auto& command : commands)
                {
                    try
                    {
                        command->run();
                    }
                    catch (...)
                    {
                        ok = false;
                        undo ();
                        break;
                    }
                }
            }

        protected:
            void _undo () const override
            {
                for (auto it = commands.rbegin(); it != commands.rend(); ++it)
                {
                    try
                    {
                        (*it)->undo ();
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Rollback failed: " << e.what() << std::endl;
                    }
                }
            }
        };
    }
}
#endif
