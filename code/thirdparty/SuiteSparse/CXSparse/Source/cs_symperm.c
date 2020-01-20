#include "cxs.h"
/* C = A(p,p) where A and C are symmetric the upper part stored; pinv not p */
cxs *cxs_symperm (const cxs *A, const CXS_INT *pinv, CXS_INT values)
{
    CXS_INT i, j, p, q, i2, j2, n, *Ap, *Ai, *Cp, *Ci, *w ;
    CXS_ENTRY *Cx, *Ax ;
    cxs *C ;
    if (!CXS_CSC (A)) return (NULL) ;                    /* check inputs */
    n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    C = cxs_spalloc (n, n, Ap [n], values && (Ax != NULL), 0) ; /* alloc result*/
    w = cxs_calloc (n, sizeof (CXS_INT)) ;                   /* get workspace */
    if (!C || !w) return (cxs_done (C, w, NULL, 0)) ;    /* out of memory */
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    for (j = 0 ; j < n ; j++)           /* count entries in each column of C */
    {
        j2 = pinv ? pinv [j] : j ;      /* column j of A is column j2 of C */
        for (p = Ap [j] ; p < Ap [j+1] ; p++)
        {
            i = Ai [p] ;
            if (i > j) continue ;       /* skip lower triangular part of A */
            i2 = pinv ? pinv [i] : i ;  /* row i of A is row i2 of C */
            w [CXS_MAX (i2, j2)]++ ;     /* column count of C */
        }
    }
    cxs_cumsum (Cp, w, n) ;              /* compute column pointers of C */
    for (j = 0 ; j < n ; j++)
    {
        j2 = pinv ? pinv [j] : j ;      /* column j of A is column j2 of C */
        for (p = Ap [j] ; p < Ap [j+1] ; p++)
        {
            i = Ai [p] ;
            if (i > j) continue ;       /* skip lower triangular part of A*/
            i2 = pinv ? pinv [i] : i ;  /* row i of A is row i2 of C */
            Ci [q = w [CXS_MAX (i2, j2)]++] = CXS_MIN (i2, j2) ;
            if (Cx) Cx [q] = (i2 <= j2) ? Ax [p] : CXS_CONJ (Ax [p]) ;
        }
    }
    return (cxs_done (C, w, NULL, 1)) ;  /* success; free workspace, return C */
}
