// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include "Object.hpp"

#include <cstdarg>

namespace pensar_digital
{
	namespace cpplib
	{
		using namespace pensar_digital::cpplib::obj;

		template <typename T>
		class Operand : public Object
		{
			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				Operand (const Id aid = NULL_ID) : Object(aid) {}
			virtual ~Operand() {}
		};

		template <typename T>
		
		class UnaryOperand : public Operand<T>
		{
			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				UnaryOperand (const Id aid = NULL_ID) : Operand<T>(aid) {}
			virtual ~UnaryOperand() {}
		};

		template <Negatable T>
		class NotOperand : public UnaryOperand
		{
			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				NotOperand (const Id aid = NULL_ID) : UnaryOperand(aid) {}	
			
				virtual ~NotOperand() {}

				bool operator !  (const T& term) const { return !term; }
		};	

		template <typename T>
		class BinaryOperand : public Operand<T>
		{
			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				BinaryOperand (const Id aid = NULL_ID) : Operand<T>(aid) {}
				virtual ~BinaryOperand() {}

				virtual bool operator () (const T& term1, const T& term2) const = 0;
		};

		template <Andable T>
		class AndOperand : public BinaryOperand
		{
			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				AndOperand (const Id aid = NULL_ID) : BinaryOperand(aid) {}	
			
				virtual ~AndOperand() {}

				bool operator &&  (const T& left, const T& right) const { return left && right; }
				bool operator ()  (const T& left, const T& right) const { return left && right; }
		};

		template <Orable T>
		class OrOperand : public BinaryOperand
		{
			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				OrOperand (const Id aid = NULL_ID) : BinaryOperand(aid) {}	
			
				virtual ~OrOperand() {}

				bool operator ||  (const T& left, const T& right) const { return left || right; }
				bool operator ()  (const T& left, const T& right) const { return left || right; }
		};

		template <Xorable T>
		class XorOperand : public BinaryOperand
		{
			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				XorOperand (const Id aid = NULL_ID) : BinaryOperand(aid) {}	
			
				virtual ~XorOperand() {}

				bool operator ^  (const T& left, const T& right) const { return left ^ right; }
				bool operator () (const T& left, const T& right) const { return left ^ right; }	
		};

		/// Constraint class for the Constraint Programming Library
		/// \author Mauricio Gomes
		/// \date 2016-12-28
		/// \version 1.0
		/// \note Base class for all constraints. It must be Constrainable.
		class Constraint : public Object
		{
			private:
			String name;

			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				/// Default constructor
				Constraint(const Id aid = NULL_ID, const String& aname = "") : Object(aid), name(aname) {}
			
				virtual ~Constraint() {}

				/// Returns the name of the constraint
				const String& get_name() const { return name; }

				/// Sets the name of the constraint
				void set_name(const String& aname) { name = aname; }

				virtual bool ok () const = 0;

				bool operator == (const Constraint& c) const { return get_id() == c.get_id(); }
				bool operator != (const Constraint& c) const { return get_id() != c.get_id(); }
				bool operator && (const Constraint& c) const { return ok () && c.ok (); }
				bool operator || (const Constraint& c) const { return ok () || c.ok (); }
				bool operator ^  (const Constraint& c) const { return ok () ^ c.ok (); }
				bool operator !  () const { return !ok (); }
		};
		
		/// Composite constraint class for the Constraint Programming Library
		/// \author Mauricio Gomes
		/// \date 2016-12-28
		/// \version 1.0
		class CompositeConstraint : public Constraint
		{
			private:
				const Constraint* left;
				const Constraint* right;
				const BinaryOperand<Constraint>& op;

			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				/// Default constructor
				CompositeConstraint (const Constraint* left, const Constraint* right, const BinaryOperand<Constraint>& op, const Id aid = NULL_ID, const String& aname = "") : Constraint(aid, aname), 
				left(left), right(right), op(op){}

				virtual ~CompositeConstraint() {}

				virtual bool ok() const { return op (left, right) };

				using Constraint::operator ==;
				using Constraint::operator !=;
				using Constraint::operator &&;
				using Constraint::operator ||;
				using Constraint::operator ^;
				using Constraint::operator !;
		};
	}
}



#endif // CONSTRAINT_HPP	
