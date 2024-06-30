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
            { t.run ()      } -> std::same_as<void>;
            { t.rollback () } -> std::same_as<void>;
        } && Identifiable<T>;

        class Command : public Object   
		{
			public:
                Command (const Id aid = NULL_ID) : Object(aid == NULL_ID ? generator.get_id() : aid) {}
                //Move constructor
                Command(Command&&) = default;
                Command(const Command&) = default;
                //Move assignment
                Command& operator= (Command&&) = default;
                Command& operator= (const Command&) = default;
                //Destructor
                virtual ~Command () = default;
                virtual void run () {}
                virtual void rollback () {}
            private:
                inline static Generator<Command, Id> generator = Generator<Command, Id> ();
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

            // Add a command to the list of commands to be executed.
            template <CommandConcept T>
            void add(std::unique_ptr<T> command)
            {
                commands.push_back(std::move(command));
            }

            size_t count() const
            {
                return commands.size();
            }

            virtual void run()
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
                        rollback();
                        break;
                    }
                }
            }

            void rollback()
            {
                for (auto it = commands.rbegin(); it != commands.rend(); ++it)
                {
                    try
                    {
                        (*it)->rollback();
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
