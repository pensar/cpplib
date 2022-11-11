#ifndef CREATOR_HPP_INCLUDED
#define CREATOR_HPP_INCLUDED

/// Plain new operator creator.
/// class T must have a default constructor and a T* Create () method.
template <class T>
struct NewCreator
{
    static T* = Create ()
    {
        return new T;
    }
};

/// Creates object by using malloc function.
/// class T must have a default constructor and a T* Create () method.
template <class T>
struct MallocCreator
{
    static T* = Create ()
    {
        void* buffer = std::malloc (sizeof (T));
        return (buffer ? new (buf) T : 0);
    }
};

/// Creates an object by cloning a template object.
/// class T must have a default constructor and a T* Create () method.
template <class T>
struct CloneCreator
{
    /// Constructor
    /// \param a_obj The template object. Defaults to nullptr.
    /// \param owns_template_obj True if template obj is owned by this CloneCreator. Defaults to true.
    CloneCreator (T* a_obj = nullptr; bool owns_template_obj = true)
    : obj (a_obj),
      owns(owns_template_obj) {}

    T* Create ()
    {
        return obj ? obj->clone () : nullptr;
    }

    T* get_template_object () const {return obj;}

    /// Sets the prototype to be used as a template.
    /// \param const T* The pointer to be stored as a template object.
    /// \remarks CloneCreator does not take ownership of the template object.
    void set_template_object (const T* a_obj)
    {
        if (owns)
            delete obj;
        obj = a_obj;
    }

   protected:
    ~CloneCreator () {if (owns) delete obj;}
   private:
    T* obj;
    bool owns;
};

/// Returns a mockup object instead of creating a new one.
/// class T must have a default constructor and a T* Create () method.
template <class T>
struct MockupCreator
{
    /// Constructor
    /// \param a_obj The mockup object. Defaults to nullptr.
    /// \param owns_template_obj If mockup obj is owned by this MockupCreator. Defaults to true.
    MockupCreator (T* a_obj = nullptr; bool owns_template_obj = true)
    : obj (a_obj),
      owns(owns_template_obj) {}

    T* Create ()
    {
        return obj;
    }

    T* get_mockup_object () const {return obj;}

    /// Sets the prototype to be used as a template.
    /// \param const T* The pointer to be stored as a template object.
    /// \remarks MockupCreator does not take ownership of the template object.
    void set_mockup_object (const T* a_obj)
    {
        if (owns)
            delete obj;
        obj = a_obj;
    }

   protected:
    ~MockupCreator () {if (owns) delete obj;}

   private:
    T* obj;
    bool owns;
};

#endif // CREATOR_HPP_INCLUDED
