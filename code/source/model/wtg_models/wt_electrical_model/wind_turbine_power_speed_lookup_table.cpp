#include "header/model/wt_electrical_model/wind_turbine_power_speed_lookup_table.h"
#include "header/basic/utility.h"
WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE()
{
    clear();
}

WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::~WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE()
{
    ;
}

WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE(const WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& table)
{
    copy_from_const_table(table);
}

WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::operator=(const WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& table)
{
    if(this==(&table)) return *this;

    copy_from_const_table(table);

    return *this;
}

void WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::copy_from_const_table(const WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& table)
{
    this->power_table = table.get_wind_turbine_power_table();
    this->speed_table = table.get_wind_turbine_speed_table();
}

void WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::clear()
{
    power_table.clear();
    speed_table.clear();
}

void WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::add_wind_turbine_power_and_speed_pair_in_pu(double power, double speed)
{
    size_t n = power_table.size();
    if(n==0)
    {
        power_table.push_back(power);
        speed_table.push_back(speed);
    }
    else
    {
        if(n==1)
        {
            if(power<power_table[0] and speed<speed_table[0])
            {
                power_table.insert(power_table.begin(),power);
                speed_table.insert(speed_table.begin(),speed);
            }
            else
            {
                if(power>power_table[0] and speed>speed_table[0])
                {
                    power_table.push_back(power);
                    speed_table.push_back(speed);
                }
            }
        }
        else
        {
            double min_power = power_table[0];
            double min_speed = speed_table[0];
            double max_power = power_table[n-1];
            double max_speed = speed_table[n-1];
            if(power<min_power and speed<min_speed)
            {
                power_table.insert(power_table.begin(), power);
                speed_table.insert(speed_table.begin(), speed);
            }
            else
            {
                if(power>max_power and speed>max_speed)
                {
                    power_table.push_back(power);
                    speed_table.push_back(speed);
                }
                else
                {
                    for(size_t i=1; i!=n; ++i)
                    {
                        if(power>power_table[i-1] and power<power_table[i] and
                           speed>speed_table[i-1] and speed<speed_table[i])
                        {
                            power_table.insert(power_table.begin()+i, power);
                            speed_table.insert(speed_table.begin()+i, speed);
                            break;
                        }
                    }
                }
            }
        }
    }
    size_t new_n = power_table.size();
    if(new_n==n)
    {
        ostringstream sstream;
        sstream<<"The wind power generation and turbine speed pair ("<<power<<", "<<speed<<") is invalid. No wind power generation and speed lookup table is updated.";
        show_information_with_leading_time_stamp(sstream);
    }
}

double WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::get_reference_speed_with_power_in_pu(double power)
{
    size_t n = power_table.size();
    if(n<2)
        return 0.0;

    if(power<=power_table[0])
        return speed_table[0];
    else
    {
        if(power>=power_table[n-1])
            return speed_table[n-1];
        else
        {
            for(size_t i=1; i!=n; ++i)
            {
                if(power>=power_table[i-1] and power<power_table[i])
                {
                    double slope = (speed_table[i]-speed_table[i-1])/(power_table[i]-power_table[i-1]);
                    return speed_table[i-1]+slope*(power-power_table[i-1]);
                }
            }
            return 0.0;
        }
    }
}

double WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::get_reference_power_with_speed_in_pu(double speed)
{
    size_t n = speed_table.size();
    if(n<2)
        return 0.0;

    if(speed<=speed_table[0])
        return power_table[0];
    else
    {
        if(speed>=speed_table[n-1])
            return power_table[n-1];
        else
        {
            for(size_t i=1; i!=n; ++i)
            {
                if(speed>=speed_table[i-1] and speed<speed_table[i])
                {
                    double slope = (power_table[i]-power_table[i-1])/(speed_table[i]-speed_table[i-1]);
                    return power_table[i-1]+slope*(speed-speed_table[i-1]);
                }
            }
            return 0.0;
        }
    }
}

vector<double> WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::get_wind_turbine_power_table() const
{
    return power_table;
}

vector<double> WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE::get_wind_turbine_speed_table() const
{
    return speed_table;
}
