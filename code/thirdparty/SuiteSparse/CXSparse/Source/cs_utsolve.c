#include "cxs.h"
/* solve U'x=b where x and b are dense.  x=b on input, solution on output. */
CXS_INT cxs_utsolve (const cxs *U, CXS_ENTRY *x)
{
    CXS_INT p, j, n, *Up, *Ui ;
    CXS_ENTRY *Ux ;
    if (!CXS_CSC (U) || !x) return (0) ;                     /* check inputs */
    n = U->n ; Up = U->p ; Ui = U->i ; Ux = U->x ;
    for (j = 0 ; j < n ; j++)
    {
        for (p = Up [j] ; p < Up [j+1]-1 ; p++)
        {
            x [j] -= CXS_CONJ (Ux [p]) * x [Ui [p]] ;
        }
        x [j] /= CXS_CONJ (Ux [Up [j+1]-1]) ;
    }
    return (1) ;
}
