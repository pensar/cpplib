// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "error.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		OutStream& operator<< (OutStream& os, const Error& e) { return e.WriteToStream  (os); }
		InStream&  operator>> (InStream&  is,	    Error& e) { return e.ReadFromStream (is); }

	} // namespace cpplib
}     // namespace pensar_digital
