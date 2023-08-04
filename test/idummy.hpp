// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef IDummy_HPP
#define IDummy_HPP

#include "constant.hpp"
#include "string_def.hpp"
#include "header_lib/json.hpp"
#include "version.hpp"
#include "iobject.hpp"

#include <iostream>
#include <memory> // for std::shared_ptr

namespace pensar_digital
{
    namespace cpplib
    {
        class IDummy;
        class IDummy_RO;
        typedef std::shared_ptr<IDummy_RO> IDummy_RO_Ptr;

        // / \brief Base class for all Dummys. Read only interface of Dummy.
        class IDummy_RO: public virtual IObject_RO
        {
        public:
            inline static const structVersion VERSION = structVersion(1, 1, 1);

            virtual String get_name() const noexcept = 0;
        };

        class IDummy : public IDummy_RO
        {
        public:
            inline static const structVersion VERSION = structVersion(1, 1, 1);

        };
        //extern Factory<IDummy, Id, String> Dummy_factory;

    }  // namespace cpplib
}      // namespace pensar_digital
#endif // Dummy_HPP
