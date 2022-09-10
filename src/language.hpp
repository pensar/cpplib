#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

#include <Id.hpp>
#include "string_def.hpp"
#include "generator.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        template <class String = string, class T = int>
        class Language : public Id<T>
        {
            public:
                /// \brief Default constructor.
                Language(T aid = -1) : Id<T> (aid = -1 ? generator.get() : aid);
                /** Default destructor */
                virtual ~Language();
                /** Copy constructor
                 *  \param other Object to copy from
                 */
                Language(const Language& other);
                /** Assignment operator
                 *  \param other Object to assign from
                 *  \return A reference to this
                 */
                Language& operator=(const Language& other);
            protected:
            private:
                static Generator<T> generator;
        };
    }
}

#endif // LANGUAGE_HPP
