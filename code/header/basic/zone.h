#ifndef ZONE_H
#define ZONE_H

#include "header/basic/base.h"

#include <string>
using namespace std;

class ZONE : public BASE
{
    public:
        ZONE();
        virtual ~ZONE();

        void set_zone_number(size_t zone_number);
        void set_zone_name(string zone_name);

        size_t get_zone_number() const;
        string get_zone_name() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        void report() const;
        virtual ZONE& operator=(const ZONE& zone);
    private:

        size_t zone_number;
        string zone_name;
};
#endif // ZONE_H
