// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef EMAIL_HPP
#define EMAIL_HPP


#include <boost/spirit/home/x3.hpp>
#include <string>

#include "contact.hpp"
#include "s.hpp"

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
        namespace contact
        {

            inline bool is_valid_local_part(const std::string& local_part)
            {
                auto iter = local_part.begin();
                return x3::parse(iter, local_part.end(), local_part) && iter == local_part.end();
            }

            inline bool is_valid_domain(const std::string& domain)
            {
                auto iter = domain.begin();
                return x3::parse(iter, domain.end(), domain) && iter == domain.end();
            }

            inline bool is_valid_email_address(const std::string& email_address) {
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

                inline void initialize(const LocalPart& lp = EMPTY, const Domain& d = EMPTY, ContactQualifier cq = ContactQualifier::Business)
                {
                    if (!is_valid_local_part(lp))
                    {
                        throw std::runtime_error("Invalid local part");
                    }
                    if (!is_valid_domain(d))
                    {
                        throw std::runtime_error("Invalid domain");
                    }
                    mlocal_part.assign (lp);
                    mdomain.assign (d);
                    mqualifier  = cq;
                }

                inline Email(const LocalPart& lp = EMPTY, const Domain& d = EMPTY, ContactQualifier cq = ContactQualifier::Business)
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
						throw std::runtime_error(W("Invalid email address"));
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

                inline S json() const noexcept
                {
                    SStream ss;
                    ss << W("{ \"class\" : \"") << pd::class_name<Email>();
                    ss << W(", \"local_part\" : ") << mlocal_part.str () << W(", \"domain\" : ") << mdomain << W(" }");
                    return ss.str();
                }

            };

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
                    throw std::runtime_error(W("Invalid email address"));
                }
                return is;
            }
        }   // namespace contact
    }       // namespace cpplib
}		    // namespace pensar_digital
#endif // EMAIL_HPP
