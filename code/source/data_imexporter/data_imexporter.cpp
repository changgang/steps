#include "header/data_imexporter/data_imexporter.h"
#include <cstdio>
#include "header/basic/utility.h"
DATA_IMEXPORTER::DATA_IMEXPORTER()
{
    set_base_frequency_in_Hz(50.0);
    set_export_zero_impedance_line_logic(true);
}

DATA_IMEXPORTER::~DATA_IMEXPORTER()
{
    ;
}

void DATA_IMEXPORTER::set_base_frequency_in_Hz(double fbase)
{
    base_frequency_in_Hz = fbase;
}

double DATA_IMEXPORTER::get_base_frequency_in_Hz() const
{
    return base_frequency_in_Hz;
}


void DATA_IMEXPORTER::set_export_zero_impedance_line_logic(bool logic)
{
    export_zero_impedance_line = logic;
}

bool DATA_IMEXPORTER::get_export_zero_impedance_line_logic() const
{
    return export_zero_impedance_line;
}

