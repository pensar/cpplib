#ifndef IDGENERATOR_HPP
#define IDGENERATOR_HPP


template <class T, typename Id = int64_t>
class IdGenerator
{
    public:
        IdGenerator() {}
        virtual ~IdGenerator() {}

        static Id get_id () { return ++id;}
    private:
        static Id id;
};



#endif // IDGENERATOR_HPP
