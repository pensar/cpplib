// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "error.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		std::ostream& operator<< (std::ostream& os, const Error<char>& e) { return e.WriteToStream  (os); }
		std::istream& operator>> (std::istream& is,		  Error<char>& e) { return e.ReadFromStream (is); }

		std::wostream& operator<< (std::wostream& os, const Error<wchar_t>& e) { return e.WriteToStream  (os); }
		std::wistream& operator>> (std::wistream& is,       Error<wchar_t>& e) { return e.ReadFromStream (is); }
	} // namespace cpplib
}     // namespace pensar_digital
