#ifndef OWNER_H
#define OWNER_H

#include <string>
using namespace std;

class STEPS;

class OWNER
{
    public:
        OWNER(STEPS& toolkit);
        virtual ~OWNER();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_owner_number(unsigned int owner_number);
        void set_owner_name(string owner_name);

        unsigned int get_owner_number() const;
        string get_owner_name() const;

        bool is_valid() const;
        void check();
        void clear();
        void report() const;
        virtual OWNER& operator=(const OWNER& owner);
    private:
        STEPS* toolkit;

        unsigned int owner_number;
        string owner_name;
};
#endif // OWNER_H
