#ifndef ZONE_H
#define ZONE_H

#include <string>
using namespace std;

class STEPS;

class ZONE
{
    public:
        ZONE(STEPS& toolkit);
        virtual ~ZONE();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_zone_number(unsigned int zone_number);
        void set_zone_name(string zone_name);

        unsigned int get_zone_number() const;
        string get_zone_name() const;

        bool is_valid() const;
        void check();
        void clear();
        void report() const;
        virtual ZONE& operator=(const ZONE& zone);
    private:
        STEPS* toolkit;

        unsigned int zone_number;
        string zone_name;
};
#endif // ZONE_H
