#include "cxs.h"
/* y = A*x+y */
CXS_INT cxs_gaxpy (const cxs *A, const CXS_ENTRY *x, CXS_ENTRY *y)
{
    CXS_INT p, j, n, *Ap, *Ai ;
    CXS_ENTRY *Ax ;
    if (!CXS_CSC (A) || !x || !y) return (0) ;       /* check inputs */
    n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    for (j = 0 ; j < n ; j++)
    {
        for (p = Ap [j] ; p < Ap [j+1] ; p++)
        {
            y [Ai [p]] += Ax [p] * x [j] ;
        }
    }
    return (1) ;
}
