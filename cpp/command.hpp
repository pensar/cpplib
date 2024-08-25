#ifndef CPP_COMMAND_HPP
#define CPP_COMMAND_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <exception>
#include <concepts>
#include <algorithm>
#include <type_traits>
#include <tuple>

#include "constant.hpp" // Id
#include "s.hpp"
#include "object.hpp"
#include "concept.hpp" // Identifiable
#include "generator.hpp"
#include "version.hpp"
#include "factory.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
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
        private:
            using G = Generator<Command, Id>; //!< Generator alias.

            inline static G mgenerator = G();
            bool mok = false;
        public:

            // Version of the class.
            inline static const VersionPtr VERSION = pd::Version::get(1, 1, 1);
            inline virtual const VersionPtr version() const noexcept { return VERSION; }

            using Factory = pd::Factory<DerivedCommand, Id>;
            inline static Factory mfactory = { 3, 10, NULL_ID }; //!< Member variable "factory"

            using FactoryType = Factory;

        public:
            Command (const Id aid = NULL_ID) : Object(aid == NULL_ID ? mgenerator.get_id() : aid) {}

            // Move constructor
            Command (Command&&) = default;

            // Copy constructor
            Command (const Command&) = default;

            // Move assignment
            Command& operator= (Command&&) = default;

            // Copy assignment
            Command& operator= (const Command&) = default;

            //Destructor
            virtual ~Command () = default;

        public:
            void run () requires DerivedCommandConcept<DerivedCommand>
            {
                // Run DerivedCommand though Static Polymorphism.
                static_cast<DerivedCommand*>(this)->_run();
                mok = true;
            }

            void undo() const  requires DerivedUndoableCommandConcept<DerivedCommand>
            {
                if (mok) // If the command was executed successfully, undo it.
                {
                    static_cast<const DerivedCommand*>(this)->_undo();
                }
            }

            bool ok() const { return mok; }

            inline virtual std::ostream& binary_write(std::ostream& os, const std::endian& byte_order) const
            {
                Object::binary_write(os, byte_order);
                mgenerator.binary_write(os, byte_order);
                VERSION->binary_write(os, byte_order);
                static_cast<const DerivedCommand*>(this)->binary_write(os, byte_order);
                return os;
            };

            inline virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order)
            {
                Object::binary_read(is, byte_order);
                mgenerator.binary_read(is, byte_order);
                VERSION->binary_read(is, byte_order);
                static_cast<DerivedCommand*>(this)->binary_read(is, byte_order);
                return is;
            };

            inline static typename FactoryType::P get(Id aid = NULL_ID) noexcept
            {
                return mfactory.get(aid);
            }
        };

        // CompositeCommand is a command that contains a list of commands to be executed in a transactional way.
        // If one of the commands fails, the entire transaction is rolled back.
        // The CompositeCommand complies with the CommandConcept so itself can be part of another CompositeCommand.
        // The CompositeCommand uses variadic templates to collect the commands to be executed.
        template<CommandConcept... Commands>
        class CompositeCommand : public Object
        {
        private:
            std::tuple<Commands...> commands;

        public:
            CompositeCommand() = default;

            CompositeCommand(const Id aid = NULL_ID, Commands... cmds)
                : Object(aid), commands(std::make_tuple(cmds...)) {}

            virtual ~CompositeCommand() = default;

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
        static_assert (Identifiable<CompositeCommand<>>);
        static_assert (CommandConcept<CompositeCommand<>>);
    }
}

#endif // CPP_COMMAND_HPP

