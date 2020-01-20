#include "cxs.h"
/* x = x + beta * A(:,j), where x is a dense vector and A(:,j) is sparse */
CXS_INT cxs_scatter (const cxs *A, CXS_INT j, CXS_ENTRY beta, CXS_INT *w, CXS_ENTRY *x, CXS_INT mark,
    cxs *C, CXS_INT nz)
{
    CXS_INT i, p, *Ap, *Ai, *Ci ;
    CXS_ENTRY *Ax ;
    if (!CXS_CSC (A) || !w || !CXS_CSC (C)) return (-1) ;     /* check inputs */
    Ap = A->p ; Ai = A->i ; Ax = A->x ; Ci = C->i ;
    for (p = Ap [j] ; p < Ap [j+1] ; p++)
    {
        i = Ai [p] ;                            /* A(i,j) is nonzero */
        if (w [i] < mark)
        {
            w [i] = mark ;                      /* i is new entry in column j */
            Ci [nz++] = i ;                     /* add i to pattern of C(:,j) */
            if (x) x [i] = beta * Ax [p] ;      /* x(i) = beta*A(i,j) */
        }
        else if (x) x [i] += beta * Ax [p] ;    /* i exists in C(:,j) already */
    }
    return (nz) ;
}
