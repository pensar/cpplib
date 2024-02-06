// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef EMAIL_HPP
#define EMAIL_HPP


#include <boost/spirit/home/x3.hpp>

#include <string>
namespace x3 = boost::spirit::x3;

auto const WSP = x3::char_(" \t");
auto const CRLF = x3::lit("\r\n");
auto const quoted_pair = x3::lit("\\") >> x3::char_;
auto const quoted_string = x3::lit("\"") >> *(quoted_pair | x3::char_ - x3::char_("\"\r\n")) >> x3::lit("\"");
auto const atext = x3::alnum | x3::char_("!#$%&'*+-/=?^_`{|}~");
auto const dot_atom_text = +atext >> *("." >> +atext);
auto const ctext = x3::char_("!-'") | x3::char_("*-[");
auto const FWS = (*WSP >> CRLF >> +WSP);
auto const dtext = x3::char_("!-Z") | x3::char_("^-~");
auto const dcontent = dtext | quoted_pair;

x3::rule<class comment, std::string> const comment = "comment";
auto const comment_def = "(" >> *(FWS >> (ctext | quoted_pair | comment)) >> FWS >> ")";
BOOST_SPIRIT_DEFINE(comment);

auto const CFWS = (+(FWS >> comment) >> FWS) | FWS;
auto const domain_literal = -CFWS >> "[" >> *(FWS >> dcontent) >> FWS >> "]" >> -CFWS;
auto const domain = dot_atom_text | domain_literal;
auto const dot_atom = -CFWS >> dot_atom_text >> -CFWS;
auto const local_part = dot_atom | quoted_string;
auto const addr_spec = local_part >> "@" >> domain;

namespace pensar_digital
{
    namespace cpplib
    {

        bool is_valid_email_address(const std::string& email_address) {
 
          

            auto iter = email_address.begin();
            return x3::parse(iter, email_address.end(), addr_spec) && iter == email_address.end();
        }
    }
}
#endif // EMAIL_HPP
