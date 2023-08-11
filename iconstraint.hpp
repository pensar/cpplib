// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef ICONSTRAINT_HPP
#define ICONSTRAINT_HPP

#include "iobject.hpp"
#include "constraint.hpp"
#include "concept.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		template <Checkable Derived>
		class IConstraintRO : public virtual IObjectRO 
		{
			public:
				inline static const Version VERSION = Version(1, 1, 1);

				~IConstraintRO () noexcept = default;

				/// Returns the name of the constraint
				virtual const String& get_name() const noexcept = 0;
		};	
		template <Checkable Derived>
		class IConstraint : public virtual IConstraintRO<Derived>, public virtual IObject	
		{
			public:
				inline static const Version VERSION = Version(1, 1, 1);

				~IConstraint () noexcept = default;

				/// Returns the name of the constraint
				virtual const String& get_name() const noexcept = 0;

				// Implements initialize method from Initializable concept.
				virtual bool initialize(const Id& aid = NULL_ID, const String& aname = "") noexcept = 0;

		};	// IConstraint.
	}	// cpplib.
}	// pensar_digital.

#endif // ICONSTRAINT_HPP