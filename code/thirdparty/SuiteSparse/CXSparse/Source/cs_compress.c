#include "cxs.h"
/* C = compressed-column form of a triplet matrix T */
cxs *cxs_compress (const cxs *T)
{
    CXS_INT m, n, nz, p, k, *Cp, *Ci, *w, *Ti, *Tj ;
    CXS_ENTRY *Cx, *Tx ;
    cxs *C ;
    if (!CXS_TRIPLET (T)) return (NULL) ;                /* check inputs */
    m = T->m ; n = T->n ; Ti = T->i ; Tj = T->p ; Tx = T->x ; nz = T->nz ;
    C = cxs_spalloc (m, n, nz, Tx != NULL, 0) ;          /* allocate result */
    w = cxs_calloc (n, sizeof (CXS_INT)) ;                   /* get workspace */
    if (!C || !w) return (cxs_done (C, w, NULL, 0)) ;    /* out of memory */
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    for (k = 0 ; k < nz ; k++) w [Tj [k]]++ ;           /* column counts */
    cxs_cumsum (Cp, w, n) ;                              /* column pointers */
    for (k = 0 ; k < nz ; k++)
    {
        Ci [p = w [Tj [k]]++] = Ti [k] ;    /* A(i,j) is the pth entry in C */
        if (Cx) Cx [p] = Tx [k] ;
    }
    return (cxs_done (C, w, NULL, 1)) ;      /* success; free w and return C */
}
