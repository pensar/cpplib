// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef EMAIL_HPP
#define EMAIL_HPP


#include <boost/spirit/home/x3.hpp>
#include <string>

#include "contact.hpp"
#include "../cpplib/cpp/s.hpp"
#include "../cpplib/cpp/error.hpp"

namespace x3 = boost::spirit::x3;



/*
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
*/

#ifdef WIDE_CHAR
auto const WSP = x3::standard_wide::char_(" \t");
auto const CRLF = x3::standard_wide::lit(L"\r\n");
auto const quoted_pair = x3::standard_wide::lit(L"\\") >> x3::standard_wide::char_;
auto const quoted_string = x3::standard_wide::lit(L"\"") >> *(quoted_pair | x3::standard_wide::char_ - x3::standard_wide::char_(L"\"\r\n")) >> x3::standard_wide::lit(L"\"");
auto const atext = x3::standard_wide::alnum | x3::standard_wide::char_(L"!#$%&'*+-/=?^_`{|}~");
auto const dot_atom_text = +atext >> *(L"." >> +atext);
auto const ctext = x3::standard_wide::char_(L"!-'") | x3::standard_wide::char_(L"*-[");
auto const FWS = (*WSP >> CRLF >> +WSP);
auto const dtext = x3::standard_wide::char_(L"!-Z") | x3::standard_wide::char_(L"^-~");
auto const dcontent = dtext | quoted_pair;

x3::rule<class comment, std::wstring> const comment = "comment";
auto const comment_def = L"(" >> *(FWS >> (ctext | quoted_pair | comment)) >> FWS >> L")";

auto const CFWS = (+(FWS >> comment) >> FWS) | FWS;
auto const domain_literal = -CFWS >> L"[" >> *(FWS >> dcontent) >> FWS >> L"]" >> -CFWS;
auto const domain = dot_atom_text | domain_literal;
auto const dot_atom = -CFWS >> dot_atom_text >> -CFWS;
auto const local_part = dot_atom | quoted_string;
auto const addr_spec = local_part >> L"@" >> domain;
#else
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

auto const CFWS = (+(FWS >> comment) >> FWS) | FWS;
auto const domain_literal = -CFWS >> "[" >> *(FWS >> dcontent) >> FWS >> "]" >> -CFWS;
auto const domain = dot_atom_text | domain_literal;
auto const dot_atom = -CFWS >> dot_atom_text >> -CFWS;
auto const local_part = dot_atom | quoted_string;
auto const addr_spec = local_part >> "@" >> domain;
#endif

BOOST_SPIRIT_DEFINE(comment);


namespace pensar_digital
{
    namespace cpplib
    {
        namespace contact
        {

            inline bool is_valid_local_part(const S& email_local_part)
            {
                auto iter = email_local_part.begin();
                return x3::parse(iter, email_local_part.end(), local_part) && iter == email_local_part.end();
            }

            inline bool is_valid_domain(const S& domain)
            {
                auto iter = domain.begin();
                return x3::parse(iter, domain.end(), domain) && iter == domain.end();
            }

            inline bool is_valid_email_address(const S& email_address) 
            {
                auto iter = email_address.begin();
                return x3::parse(iter, email_address.end(), addr_spec) && iter == email_address.end();
            }

            struct Email
            {
                typedef pd::CS<0, 64> LocalPart; // The maximum length of the local part is 64 characters
                typedef pd::CS<0, 255> Domain; // The maximum length of the domain name is 255 characters    
                LocalPart mlocal_part;
                Domain    mdomain;
                ContactQualifier mqualifier;
                const inline static LocalPart NULL_LOCAL_PART = LocalPart(W("null"));
                const inline static Domain NULL_DOMAIN = Domain(W("null"));

                static inline constexpr const S null_email_str() noexcept
				{
					return W("null@null");
				}

                inline void initialize(const LocalPart& lp = NULL_LOCAL_PART, const Domain& d = NULL_DOMAIN, ContactQualifier cq = ContactQualifier::Business)
                {
                    if (!is_valid_local_part(lp))
                    {
                        runtime_error(W("Invalid local part"));
                    }
                    if (!is_valid_domain(d))
                    {
                        runtime_error(W("Invalid domain"));
                    }
                    mlocal_part.assign (lp);
                    mdomain.assign (d);
                    mqualifier  = cq;
                }

                inline Email(const LocalPart& lp = NULL_LOCAL_PART, const Domain& d = NULL_DOMAIN, ContactQualifier cq = ContactQualifier::Business)
                    : mlocal_part(lp), mdomain(d), mqualifier(cq) 
                {
                    initialize(lp, d, cq);
                }

                inline Email(const S& s, ContactQualifier cq = ContactQualifier::Business)
					: mqualifier(cq)
				{
					auto at = s.find(W('@'));
					if (at != S::npos)
					{
                        initialize(s.substr(0, at), s.substr(at + 1), cq);
					}
					else
					{
						runtime_error(W("Invalid email address"));
					}
				}

                inline Email (const C* s, ContactQualifier cq = ContactQualifier::Business)
				     : Email(S(s), cq)
				{
				}

                // Conversion to S
                inline S str() const noexcept
				{
                    return  mlocal_part.to_string () + W("@") + mdomain;
				}

                // Implicit conversion to basic_string<C>
                inline operator S() const noexcept
                {
                    return str ();
                }

                inline bool equal_local_part(const Email& other) const noexcept
                {
                    return mlocal_part == other.mlocal_part;
                }

                inline bool equal_domain(const Email& other) const noexcept
                {
                    return mdomain == other.mdomain;
                }

                inline bool operator==(const Email& other) const noexcept
                {
                    return equal_local_part(other) && equal_domain(other);
                }

                inline bool operator!=(const Email& other) const noexcept
                {
                    return !(*this == other);
                }

            }; // struct Email
             
            static inline const Email NULL_EMAIL = { Email::NULL_LOCAL_PART, Email::NULL_DOMAIN, ContactQualifier::Business };

            // Make Email OutputStreamable.
            inline OutStream& operator<<(OutStream& os, const Email& e)
            {
                os << e.mlocal_part.str () << W("@") << e.mdomain;
                return os;
            }

            // Make Email InputStreamable.
            inline InStream& operator>>(InStream& is, Email& e)
            {
                S s;
                is >> s;
                auto at = s.find(W('@'));
                if (at != S::npos)
                {
                    e.mlocal_part = s.substr(0, at);
                    e.mdomain = s.substr(at + 1);
                }
                else
                {
                    runtime_error(W("Invalid email address"));
                }
                return is;
            }    
        }   // namespace contact
    }       // namespace cpplib
}		    // namespace pensar_digital
#endif // EMAIL_HPP
