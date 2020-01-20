#include "cxs.h"
/* C = A' */
cxs *cxs_transpose (const cxs *A, CXS_INT values)
{
    CXS_INT p, q, j, *Cp, *Ci, n, m, *Ap, *Ai, *w ;
    CXS_ENTRY *Cx, *Ax ;
    cxs *C ;
    if (!CXS_CSC (A)) return (NULL) ;    /* check inputs */
    m = A->m ; n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    C = cxs_spalloc (n, m, Ap [n], values && Ax, 0) ;       /* allocate result */
    w = cxs_calloc (m, sizeof (CXS_INT)) ;                      /* get workspace */
    if (!C || !w) return (cxs_done (C, w, NULL, 0)) ;       /* out of memory */
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    for (p = 0 ; p < Ap [n] ; p++) w [Ai [p]]++ ;          /* row counts */
    cxs_cumsum (Cp, w, m) ;                                 /* row pointers */
    for (j = 0 ; j < n ; j++)
    {
        for (p = Ap [j] ; p < Ap [j+1] ; p++)
        {
            Ci [q = w [Ai [p]]++] = j ; /* place A(i,j) as entry C(j,i) */
            if (Cx) Cx [q] = (values > 0) ? CXS_CONJ (Ax [p]) : Ax [p] ;
        }
    }
    return (cxs_done (C, w, NULL, 1)) ;  /* success; free w and return C */
}
