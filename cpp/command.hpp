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
            { t.undo () } -> std::same_as<void>;
        } && Identifiable<T>;

        class Command : public Object   
		{
			public:
                Command (const Id aid = NULL_ID) : Object(aid == NULL_ID ? mgenerator.get_id() : aid), mok(false) {}
                
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

            protected:
                // Execute the command. It is a template method guaranteeing that the command sets ok = true if run successfully.
                virtual void execute  () { mok = true  ; }
                
                // Rollback the command. It is a template method guaranteeing that undo only runs if the command was executed successfully.
                virtual void rollback () { mok = false; }
            public:
                void run() { execute (); mok = true; }
                void undo () { if (mok) rollback (); }
                bool ok () const { return mok; }
            private:
                inline static Generator<Command, Id> mgenerator = Generator<Command, Id> ();
                bool mok = false;
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
            virtual void execute ()
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
            virtual void rollback()
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
