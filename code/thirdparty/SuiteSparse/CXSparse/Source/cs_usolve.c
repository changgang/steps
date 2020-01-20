#include "cxs.h"
/* solve Ux=b where x and b are dense.  x=b on input, solution on output. */
CXS_INT cxs_usolve (const cxs *U, CXS_ENTRY *x)
{
    CXS_INT p, j, n, *Up, *Ui ;
    CXS_ENTRY *Ux ;
    if (!CXS_CSC (U) || !x) return (0) ;                     /* check inputs */
    n = U->n ; Up = U->p ; Ui = U->i ; Ux = U->x ;
    for (j = n-1 ; j >= 0 ; j--)
    {
        x [j] /= Ux [Up [j+1]-1] ;
        for (p = Up [j] ; p < Up [j+1]-1 ; p++)
        {
            x [Ui [p]] -= Ux [p] * x [j] ;
        }
    }
    return (1) ;
}
