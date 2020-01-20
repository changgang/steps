#include "cxs.h"
/* x(p) = b, for dense vectors x and b; p=NULL denotes identity */
CXS_INT cxs_ipvec (const CXS_INT *p, const CXS_ENTRY *b, CXS_ENTRY *x, CXS_INT n)
{
    CXS_INT k ;
    if (!x || !b) return (0) ;                              /* check inputs */
    for (k = 0 ; k < n ; k++) x [p ? p [k] : k] = b [k] ;
    return (1) ;
}
