#ifndef OWNERSHIP_H
#define OWNERSHIP_H

#include <vector>
#include <map>
using namespace std;

class OWNERSHIP
{
    public:
        OWNERSHIP();
        OWNERSHIP(const OWNERSHIP& ownership);

        void append_owner_and_its_fraction(size_t owner, double fraction);
        void delete_owner_and_its_fraction(size_t owner);
        void normalize();

        bool empty() const;
        size_t get_owner_count() const;

        vector<size_t>    get_all_owners() const;
        size_t get_owner_of_index(const size_t index) const;
        vector<double> get_all_fraction() const;
        double get_fraction_of_owner(const size_t owner) const;
        double get_fraction_of_owner_of_index(const size_t index) const;

        void clear();

        OWNERSHIP& operator=(const OWNERSHIP& ownership);

    private:
        void copy_from_const_ownership(const OWNERSHIP& ownership);

        bool has_owner(const size_t owner) const;
        map<size_t,double> ownership_pair;
};

#endif // OWNERSHIP_H
