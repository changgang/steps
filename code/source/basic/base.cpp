#include "header/basic/base.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <sstream>

using namespace std;

BASE::BASE()
{
    toolkit = NULL;
}

BASE::~BASE()
{
    ;
}

void BASE::set_toolkit(STEPS& toolkit)
{
    this->toolkit = &toolkit;
}

STEPS& BASE::get_toolkit() const
{
    if(toolkit==NULL)
    {
        ostringstream osstream;
        osstream<<"Error. Toolkit is not set in BASE::toolkit";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    }

    return (*toolkit);
}

