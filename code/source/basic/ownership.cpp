#include "header/basic/ownership.h"
#include <istream>
#include <iostream>
using namespace std;

OWNERSHIP::OWNERSHIP()
{
    ownership_pair.clear();
}
OWNERSHIP::~OWNERSHIP()
{
}

OWNERSHIP::OWNERSHIP(const OWNERSHIP& ownership)
{
    copy_from_const_ownership(ownership);
}

void OWNERSHIP::append_owner_and_its_fraction(unsigned int owner, double fraction)
{
	if(owner != 0 and fraction>=0.0)
    {
        if(not has_owner(owner))
            ownership_pair.insert(map<unsigned int,double>::value_type(owner, fraction));
        else
        {
            map<unsigned int, float>::iterator iter;
            iter = ownership_pair.find(owner);
            iter->second += fraction;
        }
    }
}

void OWNERSHIP::delete_owner_and_its_fraction(unsigned int owner)
{
    map<unsigned int, float>::iterator iter;
    iter = ownership_pair.find(owner);
    if(iter!=ownership_pair.end())
    {
        ownership_pair.erase(iter);
    }
}

void OWNERSHIP::normalize()
{
    map<unsigned int, float>::iterator iter;
    double sum_fraction = 0.0;
    for(iter=ownership_pair.begin(); iter!=ownership_pair.end(); ++iter)
        sum_fraction += iter->second;

    if(sum_fraction!=0.0)
    {
        for(iter=ownership_pair.begin(); iter!=ownership_pair.end(); ++iter)
            iter->second /= sum_fraction;
    }
}

bool OWNERSHIP::empty() const
{
    if(get_owner_count()==0) return true;
    else                     return false;
}

unsigned int OWNERSHIP::get_owner_count() const
{
    return ownership_pair.size();
}

vector<unsigned int> OWNERSHIP::get_all_owners() const
{
    vector<unsigned int> owners;
    map<unsigned int, float>::const_iterator it;
    for(it=ownership_pair.begin(); it!=ownership_pair.end(); ++it)
        owners.push_back(it->first);

    return owners;
}

unsigned int OWNERSHIP::get_owner_of_index(const unsigned int index) const
{
    map<unsigned int, float>::const_iterator it;
    it = ownership_pair.begin();
	unsigned int n = ownership_pair.size();
    if(it!=ownership_pair.end())
    {
		if (index < n)
		{
			std::advance(it, index);
			if (it != ownership_pair.end())
				return it->first;
			else
				return 0;
		}
		else
			return 0;
    }
	else
	{
		return 0;
	}
}

double OWNERSHIP::get_fraction_of_owner_of_index(const unsigned int index) const
{
    map<unsigned int, float>::const_iterator it;
    it = ownership_pair.begin();
	unsigned int n = ownership_pair.size();
    if(it!=ownership_pair.end())
    {
		if (index < n)
		{
			std::advance(it, index);
			if (it != ownership_pair.end())
				return it->second;
			else
				return 0.0;
		}
		else
			return 0.0;
    }
	else
	{
		return 0.0;
	}
}

vector<double> OWNERSHIP::get_all_fraction() const
{
    vector<double> fraction;
    map<unsigned int, float>::const_iterator it;
    for(it=ownership_pair.begin(); it!=ownership_pair.end(); ++it)
        fraction.push_back(it->second);

    return fraction;
}

double OWNERSHIP::get_fraction_of_owner(const unsigned int owner) const
{
    map<unsigned int, float>::const_iterator it;
    it = ownership_pair.find(owner);
    if(it!=ownership_pair.end())
        return it->second;
    else
        return 0.0;
}

void OWNERSHIP::clear()
{
    ownership_pair.clear();
}

bool OWNERSHIP::has_owner(const unsigned int owner) const
{
    map<unsigned int, float>::const_iterator it;
    it = ownership_pair.find(owner);
    if(it!=ownership_pair.end())
        return true;
    else
        return false;
}

OWNERSHIP& OWNERSHIP::operator=(const OWNERSHIP& ownership)
{
    if(this==(&ownership)) return *this;

    copy_from_const_ownership(ownership);

    return *this;
}

void OWNERSHIP::copy_from_const_ownership(const OWNERSHIP& ownership)
{
    clear();
    unsigned int n = ownership.get_owner_count();
    vector<unsigned int> all_owners = ownership.get_all_owners();
    double fraction;
    for(unsigned int i=0; i!=n; ++i)
    {
        fraction = ownership.get_fraction_of_owner(all_owners[i]);
        append_owner_and_its_fraction(all_owners[i], fraction);
    }
}
