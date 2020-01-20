#include "cxs.h"
/* solve L'x=b where x and b are dense.  x=b on input, solution on output. */
CXS_INT cxs_ltsolve (const cxs *L, CXS_ENTRY *x)
{
    CXS_INT p, j, n, *Lp, *Li ;
    CXS_ENTRY *Lx ;
    if (!CXS_CSC (L) || !x) return (0) ;                     /* check inputs */
    n = L->n ; Lp = L->p ; Li = L->i ; Lx = L->x ;
    for (j = n-1 ; j >= 0 ; j--)
    {
        for (p = Lp [j]+1 ; p < Lp [j+1] ; p++)
        {
            x [j] -= CXS_CONJ (Lx [p]) * x [Li [p]] ;
        }
        x [j] /= CXS_CONJ (Lx [Lp [j]]) ;
    }
    return (1) ;
}
