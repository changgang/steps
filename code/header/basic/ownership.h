#ifndef OWNERSHIP_H
#define OWNERSHIP_H

#include <vector>
#include <map>
using namespace std;

class OWNERSHIP
{
    public:
        OWNERSHIP();
        virtual ~OWNERSHIP();
        OWNERSHIP(const OWNERSHIP& ownership);

        void append_owner_and_its_fraction(unsigned int owner, double fraction);
        void delete_owner_and_its_fraction(unsigned int owner);
        void normalize();

        bool empty() const;
        unsigned int get_owner_count() const;

        vector<unsigned int>    get_all_owners() const;
        unsigned int get_owner_of_index(const unsigned int index) const;
        vector<double> get_all_fraction() const;
        double get_fraction_of_owner(const unsigned int owner) const;
        double get_fraction_of_owner_of_index(const unsigned int index) const;

        void clear();

        virtual OWNERSHIP& operator=(const OWNERSHIP& ownership);

    private:
        void copy_from_const_ownership(const OWNERSHIP& ownership);

        bool has_owner(const unsigned int owner) const;
        map<unsigned int,float> ownership_pair;
};

#endif // OWNERSHIP_H
