#include "cxs.h"
/* x = b(p), for dense vectors x and b; p=NULL denotes identity */
CXS_INT cxs_pvec (const CXS_INT *p, const CXS_ENTRY *b, CXS_ENTRY *x, CXS_INT n)
{
    CXS_INT k ;
    if (!x || !b) return (0) ;                              /* check inputs */
    for (k = 0 ; k < n ; k++) x [k] = b [p ? p [k] : k] ;
    return (1) ;
}
