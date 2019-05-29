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

bool BASE::is_toolkit_set() const
{
    return toolkit!=NULL;
}

STEPS& BASE::get_toolkit(const string calling_function) const
{
    if(is_toolkit_set())
        return (*toolkit);
    else
    {
        ostringstream osstream;
        osstream<<"Error. Toolkit is not set in BASE::toolkit when calling the following function:\n"
                <<calling_function<<"\n"
                <<"Report to developer immediately for further help.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return (*toolkit);
    }
}

