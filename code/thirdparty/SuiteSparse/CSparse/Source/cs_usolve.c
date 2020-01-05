#include "cs.h"
/* solve Ux=b where x and b are dense.  x=b on input, solution on output. */
csi cs_usolve (const cs *U, double *x)
{
    csi p, j, n, *Up, *Ui ;
    double *Ux ;
    if (!CS_CSC (U) || !x) return (0) ;                     /* check inputs */
    n = U->n ; Up = U->p ; Ui = U->i ; Ux = U->x ;
    for (j = n-1 ; j >= 0 ; j--)
    {
        /* original version
        x [j] /= Ux [Up [j+1]-1] ;
        for (p = Up [j] ; p < Up [j+1]-1 ; p++)
        {
            x [Ui [p]] -= Ux [p] * x [j] ;
        }
        */
        /* update on Jan. 5, 2020 to reduce cache miss*/
        ptrdiff_t pthis = Up[j];
        ptrdiff_t pnext = Up[j+1];
        x [j] /= Ux [pnext-1] ;
        double xj = x[j];
        for (p = pthis ; p < pnext-1 ; p++)
        {
            ptrdiff_t index = Ui [p];
            if(index != j)
                x [index] -= Ux [p] * xj ;
            else
            {
                x [index] -= Ux [p] * xj ;
                xj = x[index];
            }
        }
    }
    return (1) ;
}
