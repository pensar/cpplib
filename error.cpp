// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "error.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		std::ostream& operator<< (std::ostream& os, const Error& e) { return e.WriteToStream  (os); }
		std::istream& operator>> (std::istream& is,		  Error& e) { return e.ReadFromStream (is); }
	} // namespace cpplib
}     // namespace pensar_digital
