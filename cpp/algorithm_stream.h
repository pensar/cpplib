#ifndef ALGORITHM_STREAM_H_INCLUDED
#define ALGORITHM_STREAM_H_INCLUDED

#include<istream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace pensar_digital
{
  namespace algorithm_stream
  {
    template <typename C = char>
    struct Counter
    {
      unsigned i;
      void operator (C c)() {++i;}
    };


    template <typename C = char, class Function = Counter, class Tr=char_traits<C>>
    for_each (const std::basic_istream<C, Tr>& is, Function f)
    {
      C c;
      while (is.get(c))
        f(c);
    }

    /// Applies function f to each char in the file.
    /// \param file the complete path for the file or the file name (default dir is platform dependent).
    /// \throws std::exception if file cannot be openned.
    template<typename C = char, typename Function = Counter<C>>
    int count (const fs::path& file, Function f = DoNothing<String>())
    {
        fs::ifstream in(file);
        if (in)
            parse_stream (in, f);
        else
        {
            std::stringstream ss;
            ss << __FILE__ << "\t" << __LINE__ << "\t Nao foi possivel abrir " << file.file_string () << std::endl;
            std::cerr << ss.str ();
            std::logic_error e(ss.str ());
            throw e;
        }
    }
  }
}

#endif // ALGORITHM_STREAM_H_INCLUDED
