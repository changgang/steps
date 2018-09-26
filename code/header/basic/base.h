#ifndef BASE_H
#define BASE_H

class POWER_SYSTEM_DATABASE;

class BASE
{
    public:
        BASE();
        virtual ~BASE();

        virtual bool is_valid() const = 0;
        virtual void check() = 0;
        virtual void clear()  = 0;
};

#endif // BASE_H
