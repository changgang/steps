#include "cs.h"
/* x(p) = b, for dense vectors x and b; p=NULL denotes identity */
csi cs_ipvec (const csi *p, const double *b, double *x, csi n)
{
    csi k ;
    if (!x || !b) return (0) ;                              /* check inputs */
    /* original version
    for (k = 0 ; k < n ; k++) x [p ? p [k] : k] = b [k] ;
    */
    /* update on Jan. 5, 2020 to reduce cache miss*/
    if(p!=NULL)
    {
        for (k = 0 ; k < n ; ++k)
        {
            ptrdiff_t index = p[k];
            x [index] = b [k] ;
        }
    }
    else
    {
        for (k = 0 ; k < n ; ++k)
            x [k] = b [k] ;
    }
    return (1) ;
}
