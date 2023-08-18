// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include "object.hpp"
#include "version_factory.hpp"
#include "constant.hpp"
#include "iconstraint.hpp"

#include <cstdarg>
#include <regex> // std::regex, std::regex_match
#include <variant> // std::variant, std::get

namespace pensar_digital
{
	namespace cpplib
	{
		inline static const String ONLY_DIGITS_REGEX = "^[0-9]*$";
		inline static const String AT_LEAST_ONE_DIGIT_REGEX = "^[0-9]+$";
		inline static const String ONLY_LETTERS_REGEX = "^[a-zA-Z]*$";
		inline static const String ONLY_LETTERS_AND_DIGITS_REGEX = "^[a-zA-Z0-9]*$";
		inline static const String ONLY_LETTERS_AND_DIGITS_AND_UNDERSCORES_REGEX = "^[a-zA-Z0-9_]*$";

		template <typename L, typename R, typename ResultType, class D>
		class Operand : public Object
		{
			public:
				inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);
				Operand (const Id aid = NULL_ID) : Object(aid) {}
				
				template <typename ... Args>
				ResultType operator () (L* left, R* right = nullptr, Args&& ... args) const
				{
					return static_cast<D*>(this)->::operator () (left, right, args ...);
				};
			virtual ~Operand() {}
		};

		template <typename T, typename ResultType, class D>
		class UnaryOperand : public Operand<T, T, ResultType, UnaryOperand<T, ResultType, D>>
		{
			private:
				typedef UnaryOperand<T, ResultType, D> UnaryOp;
				typedef Operand <T, T, ResultType, UnaryOp> Base;
			public:
				inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);

				UnaryOperand (const Id aid = NULL_ID) : Base(aid) {}
				
				virtual ~UnaryOperand() {}

				template <typename ... Args>
				ResultType operator () (T* ptr, const T* not_used = nullptr, Args&& ... args) const
				{
					return static_cast<D*>(this)->::operator () (ptr, args ...);
				};
		};
		 
		template <Checkable T>
		class NotOperand : public UnaryOperand<T, bool, NotOperand<T>>
		{
			private:
				typedef UnaryOperand<T, bool, NotOperand<T>> Base;
			public:
				inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);
				
				NotOperand (const Id aid = NULL_ID) : Base(aid) {}
			
				virtual ~NotOperand() {}

				template <typename ... Args>
				bool operator () (const T* ptr, Args&& ... args) const noexcept
				{
					return !ptr->ok (args ...);
				};
		};

		template <typename L, typename R, typename ResultType, class D>
		class BinaryOperand : public Operand<L, R, ResultType, BinaryOperand<L, R, ResultType, D>>
		{
			private:
				typedef Operand<L, R, ResultType, BinaryOperand<L, R, ResultType, D>> Base;
			public:
				inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);
				BinaryOperand (const Id aid = NULL_ID) : Base (aid) {}
				virtual ~BinaryOperand() {}

				template <typename ... Args>
				ResultType operator () (const L* left, const R* right = nullptr, Args&& ... args) const
				{
					return static_cast<D*>(this)->operator () (left, right, args ...);
				};
		};

		template <Checkable L, Checkable R>
		class AndOperand : public BinaryOperand<L, R, bool, AndOperand<L, R>>
		{
			private:
				typedef BinaryOperand<L, R, bool, AndOperand<L, R>> Base;
			
			public:
				inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);
				AndOperand (const Id aid = NULL_ID) noexcept : Base (aid) {}
			
				virtual ~AndOperand() {}

				template <typename ... Args>
				bool operator ()  (const L& left, const R& right, Args&& ... args) const noexcept
				{ 
					return (left.ok(args ...) && right.ok(args ...));
				}
		};


		template <Checkable L, Checkable R>
		class OrOperand : public BinaryOperand<L, R, bool, OrOperand<L, R>>
		{
		private:
			typedef BinaryOperand<L, R, bool, OrOperand<L, R>> Base;

		public:
			inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);
			OrOperand(const Id aid = NULL_ID) noexcept : Base(aid) {}

			virtual ~OrOperand() {}

			template <typename ... Args>
			bool operator ()  (const L& left = nullptr, const R& right = nullptr, Args&& ... args) const noexcept
			{
				return (left.ok(args ...) || right.ok(args ...));
			}
		};


		template <Checkable L, Checkable R>
		class XorOperand : public BinaryOperand<L, R, bool, XorOperand<L, R>>
		{
		private:
			typedef BinaryOperand<L, R, bool, XorOperand<L, R>> Base;

		public:
			inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);
			XorOperand(const Id aid = NULL_ID) noexcept : Base(aid) {}

			virtual ~XorOperand() {}

			template <typename ... Args>
			bool operator ()  (const L& left = nullptr, const R& right = nullptr, Args&& ... args) const noexcept
			{
				return (left.ok(args ...) ^ right.ok(args ...));
			}
		};

		/// Constraint class for the Constraint Programming Library
		/// \author Mauricio Gomes
		/// \date 2016-12-28
		/// \version 1.0
		/// \note Base class for all constraints. It must be Constrainable.
		/// DerivedConstraint is here to simulate a static polymorphism.
		template <class D>
		class Constraint : public Object
		{
			private:
			String name;

			public:
				inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);
				//typedef IConstraint<Constraint>     I; // Interface type.
				//typedef IConstraintRO<Constraint> IRO; // Read only interface type.

				/// Default constructor
				Constraint(const Id aid = NULL_ID, const String& aname = "") : Object(aid), name(aname) {}
			
				virtual ~Constraint () {}

				/// Returns the name of the constraint
				inline const String& get_name () const noexcept { return name; }

				/// Sets the name of the constraint
				inline void set_name (const String& aname) { name = aname; }

				// Implements initialize method from Initializable concept.
				virtual bool initialize(const Id& aid = NULL_ID, const String& aname = "") noexcept
				{
					Object::set_id(aid);
					name = aname;
					return true;
				}

				template <typename... Args> bool ok (Args&& ... args) const noexcept
				{
					static_assert (Checkable<D, Args>);
					return static_cast<D*>(this)->::ok (std::forward<Args>(args)...);
				};

				template <typename... Args>
				bool Or (const Constraint<D>& other, Args& ... args) const noexcept
				{
					return ok (args ...) || other.ok (args ...);
				};

				template <typename... Args>
				bool And (const Constraint<D>& other, Args& ... args) const noexcept
				{
					return ok(args ...) && other.ok(args ...);
				};

				template <typename... Args>
				bool Xor (const Constraint<D>& other, Args& ... args) const noexcept
				{
					return ok(args ...) ^ other.ok(args ...);
				};

				template <typename... Args>
				bool Not (Args& ... args) const noexcept
				{
					return !ok(args ...);
				};
		};
		
		/// Composite constraint class for the Constraint Programming Library
		/// \author Mauricio Gomes
		/// \date 2016-12-28
		/// \version 1.0
		template <Checkable L, Checkable R>
		class CompositeConstraint : public Constraint <CompositeConstraint<L, R>>
		{
			public:
				typedef std::variant <AndOperand <L, R>, OrOperand <L, R>, XorOperand <L, R>> OpType;
				typedef enum { AND, OR, XOR } OpIndex;
			private:
				typedef CompositeConstraint<L, R> C;
				typedef Constraint <C> Base;

				typedef std::shared_ptr<Base  >  BasePtr;
				typedef std::shared_ptr<L     >  LeftPtr;
				typedef std::shared_ptr<R     > RightPtr;
				const L&  left;
				const R& right;
				OpType op;
			public:
				inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);
				
				CompositeConstraint (const L&    aleft,
									 const R&  aright,
									 const OpType aop,
									 const Id aid = NULL_ID, 
									 const String& aname = "") : Base(aid, aname),
									 left(aleft), right(aright), op (aop)
				{
				}
				virtual ~CompositeConstraint() {}

				template <typename ...Args> 
				bool ok (Args&& ...args) const noexcept				
				{ 
					switch (op.index ())
{
						case AND:
							return std::get<AND>(op) (left, right, args ...);
						case OR:
							return std::get<OR>(op) (left, right, args ...);
						case XOR:
							return std::get<XOR>(op) (left, right, args ...);
					}
					return false;
				};
		};	// class CompositeConstraint

		
		template <Checkable L, Checkable R>
		CompositeConstraint<L, R> operator && (const L& left, const R& right)
		{
			typedef CompositeConstraint<L, R> C;
			return * new C (left, right);
		}

		template <Checkable L, Checkable R>
		CompositeConstraint<L, R> operator || (const L& left, const R& right)
		{
			typedef CompositeConstraint<L, R> C;
			return *new C(left, right, OrOperand<L,R>());
		}

		// String constraint.
		class StringConstraint : public Constraint <StringConstraint>
		{
			private:
				typedef Constraint<StringConstraint> Base;
				const std::regex regex;

			public:
				inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);

				/// Default constructor
				StringConstraint (const String& aregex = "", const Id aid = NULL_ID, const String& aname = "") 
					: Base(aid, aname), regex (aregex) { }

				virtual ~StringConstraint() {}

				bool ok(const String& s = "") const noexcept
				{ 
					return std::regex_search (s, regex);
				};

		};	// class StringConstraint

		// Range constraint.
		template <RangeCheckable T>	
		class RangeConstraint : public Constraint <RangeConstraint<T>>
		{
			private:
				typedef Constraint<RangeConstraint<T>> Base;
				const T min;
				const T max;

			public:
				inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);

				/// Default constructor
				RangeConstraint (const T& amin, const T& amax, const Id aid = NULL_ID, const String& aname = "") 
					: Base(aid, aname), min (amin), max (amax) { }

				RangeConstraint (const T& avalue, const Id aid = NULL_ID, const String& aname = "") 
					: Base(aid, aname), min (avalue), max (avalue) { }	

				virtual ~RangeConstraint() {}

				bool ok(const T& t = 0) const noexcept
				{
					return (t >= min) && (t <= max);
				};

		};	// class RangeConstraint
	}	// namespace cpplib
}	// namespace pensar_digital



#endif // CONSTRAINT_HPP	
