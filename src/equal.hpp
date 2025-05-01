#ifndef EQUAL_HPP
#define EQUAL_HPP

#include "concept.hpp"
#include "constant.hpp"

namespace pensar_digital
{
	namespace cpp
	{
		// HasStdLayoutTriviallyCopyableData concept.
		template <class T>
		concept HasStdLayoutTriviallyCopyableData = requires (T t)
		{
			{ t.data() } -> std::convertible_to<const pensar_digital::cpplib::Data*>;
			{ t.data_size() } -> std::convertible_to<size_t>;
		} && pensar_digital::cpplib::TriviallyCopyable<typename T::DataType>;

		template <class T>
		concept TriviallyHashComparable = HasStdLayoutTriviallyCopyableData<T> && pensar_digital::cpplib::Hashable<T>;

		/// \brief Compares two objects of the same type for equality.
		/// \param lhs The left-hand side object.
		/// \param rhs The right-hand side object.
		/// \return true if the objects are equal, false otherwise.
		template <HasStdLayoutTriviallyCopyableData T>
		bool trivially_equal(const T& l, const T& r)
		{
			const T* lp = dynamic_cast<const T*>(&l);
			const T* rp = dynamic_cast<const T*>(&r);
			if ((lp == nullptr) && (rp == nullptr))
				return true;
			if (((lp == nullptr) && (rp != nullptr)) || ((lp != nullptr) && (rp == nullptr)))
				return false;

			return !(std::memcmp(l.data(), r.data(), l.data_size()));
		}

		template <TriviallyHashComparable T>
		bool equal(const T& l, const T& r)
		{
			if (l.hash() != r.hash())
				return false;
			return trivially_equal<T>(l, r);
		}

	}
}
#endif	// EQUAL_HPP
