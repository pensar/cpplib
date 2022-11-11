#ifndef LOCALE_HPP
#define LOCALE_HPP

#include <id.hpp>

template <class String = string>
class Locale : public Id
{
  public:
    /** Default constructor */
    Locale (const typename Id::value_type aid = -1,);
    /** Default destructor */
    virtual ~locale();
    /** Copy constructor
     *  \param other Object to copy from
     */
    locale(const locale& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    locale& operator=(const locale& other);
    /** Access name
     * \return The current value of name
     */
    String Getname() { return name; }
    /** Access language
     * \return The current value of language
     */
    Language* Getlanguage() { return language; }
    /** Access country
     * \return The current value of country
     */
    Country* Getcountry() { return country; }
  protected:
  private:
    Language* language; //!< Member variable "language"
    Country* country; //!< Member variable "country"
};

#endif // LOCALE_HPP
