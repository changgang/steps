#ifndef OWNER_H
#define OWNER_H

#include "header/basic/base.h"

#include <string>
using namespace std;

class OWNER : public BASE
{
    public:
        OWNER();
        virtual ~OWNER();

        void set_owner_number(size_t owner_number);
        void set_owner_name(string owner_name);

        size_t get_owner_number() const;
        string get_owner_name() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        void report() const;
        virtual OWNER& operator=(const OWNER& owner);
    private:
        size_t owner_number;
        string owner_name;
};
#endif // OWNER_H
