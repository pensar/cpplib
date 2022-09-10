#ifndef TYPE_MANIPULATION_HPP_INCLUDED
#define TYPE_MANIPULATION_HPP_INCLUDED

namespace pensar_digital
{
    template <int v>
    struct Int2Type
    {
        enum { value = v };
    };
}

#endif // TYPE_MANIPULATION_HPP_INCLUDED
