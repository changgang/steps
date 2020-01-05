#include "cs.h"
/* solve Lx=b where x and b are dense.  x=b on input, solution on output. */
csi cs_lsolve (const cs *L, double *x)
{
    csi p, j, n, *Lp, *Li ;
    double *Lx ;
    if (!CS_CSC (L) || !x) return (0) ;                     /* check inputs */
    n = L->n ; Lp = L->p ; Li = L->i ; Lx = L->x ;
    for (j = 0 ; j < n ; ++j)
    {
        /* original version
        x [j] /= Lx [Lp [j]] ;
        for (p = Lp [j]+1 ; p < Lp [j+1] ; p++)
        {
            x [Li [p]] -= Lx [p] * x [j] ;
        }
        */
        /* update on Jan. 5, 2020 to reduce cache miss*/
        ptrdiff_t pthis = Lp[j];
        ptrdiff_t pnext = Lp[j+1];
        x [j] /= Lx [pthis] ;
        double xj = x [j];
        for (p = pthis+1 ; p < pnext ; ++p)
        {
            ptrdiff_t index = Li [p];
            if(index!=j)
                x [index] -= Lx [p] * xj ;
            else
            {
                x [index] -= Lx [p] * xj ;
                xj = x[index];
            }
        }
    }
    return (1) ;
}
