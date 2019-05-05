#ifndef BASE_H
#define BASE_H

class POWER_SYSTEM_DATABASE;
class STEPS;

class BASE
{
    public:
        BASE();
        virtual ~BASE();

        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        virtual bool is_valid() const = 0;
        virtual void check() = 0;
        virtual void clear()  = 0;
    private:
        STEPS* toolkit;
};

#endif // BASE_H
