#include "cxs.h"
/* pinv = p', or p = pinv' */
CXS_INT *cxs_pinv (CXS_INT const *p, CXS_INT n)
{
    CXS_INT k, *pinv ;
    if (!p) return (NULL) ;                     /* p = NULL denotes identity */
    pinv = cxs_malloc (n, sizeof (CXS_INT)) ;        /* allocate result */
    if (!pinv) return (NULL) ;                  /* out of memory */
    for (k = 0 ; k < n ; k++) pinv [p [k]] = k ;/* invert the permutation */
    return (pinv) ;                             /* return result */
}
