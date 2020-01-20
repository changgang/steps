#include "cxs.h"
/* C = A(p,q) where p and q are permutations of 0..m-1 and 0..n-1. */
cxs *cxs_permute (const cxs *A, const CXS_INT *pinv, const CXS_INT *q, CXS_INT values)
{
    CXS_INT t, j, k, nz = 0, m, n, *Ap, *Ai, *Cp, *Ci ;
    CXS_ENTRY *Cx, *Ax ;
    cxs *C ;
    if (!CXS_CSC (A)) return (NULL) ;    /* check inputs */
    m = A->m ; n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    C = cxs_spalloc (m, n, Ap [n], values && Ax != NULL, 0) ;  /* alloc result */
    if (!C) return (cxs_done (C, NULL, NULL, 0)) ;   /* out of memory */
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    for (k = 0 ; k < n ; k++)
    {
        Cp [k] = nz ;                   /* column k of C is column q[k] of A */
        j = q ? (q [k]) : k ;
        for (t = Ap [j] ; t < Ap [j+1] ; t++)
        {
            if (Cx) Cx [nz] = Ax [t] ;  /* row i of A is row pinv[i] of C */
            Ci [nz++] = pinv ? (pinv [Ai [t]]) : Ai [t] ;
        }
    }
    Cp [n] = nz ;                       /* finalize the last column of C */
    return (cxs_done (C, NULL, NULL, 1)) ;
}
