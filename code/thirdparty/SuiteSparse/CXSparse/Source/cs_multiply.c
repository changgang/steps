#include "cxs.h"
/* C = A*B */
cxs *cxs_multiply (const cxs *A, const cxs *B)
{
    CXS_INT p, j, nz = 0, anz, *Cp, *Ci, *Bp, m, n, bnz, *w, values, *Bi ;
    CXS_ENTRY *x, *Bx, *Cx ;
    cxs *C ;
    if (!CXS_CSC (A) || !CXS_CSC (B)) return (NULL) ;      /* check inputs */
    if (A->n != B->m) return (NULL) ;
    m = A->m ; anz = A->p [A->n] ;
    n = B->n ; Bp = B->p ; Bi = B->i ; Bx = B->x ; bnz = Bp [n] ;
    w = cxs_calloc (m, sizeof (CXS_INT)) ;                    /* get workspace */
    values = (A->x != NULL) && (Bx != NULL) ;
    x = values ? cxs_malloc (m, sizeof (CXS_ENTRY)) : NULL ; /* get workspace */
    C = cxs_spalloc (m, n, anz + bnz, values, 0) ;        /* allocate result */
    if (!C || !w || (values && !x)) return (cxs_done (C, w, x, 0)) ;
    Cp = C->p ;
    for (j = 0 ; j < n ; j++)
    {
        if (nz + m > C->nzmax && !cxs_sprealloc (C, 2*(C->nzmax)+m))
        {
            return (cxs_done (C, w, x, 0)) ;             /* out of memory */
        }
        Ci = C->i ; Cx = C->x ;         /* C->i and C->x may be reallocated */
        Cp [j] = nz ;                   /* column j of C starts here */
        for (p = Bp [j] ; p < Bp [j+1] ; p++)
        {
            nz = cxs_scatter (A, Bi [p], Bx ? Bx [p] : 1, w, x, j+1, C, nz) ;
        }
        if (values) for (p = Cp [j] ; p < nz ; p++) Cx [p] = x [Ci [p]] ;
    }
    Cp [n] = nz ;                       /* finalize the last column of C */
    cxs_sprealloc (C, 0) ;               /* remove extra space from C */
    return (cxs_done (C, w, x, 1)) ;     /* success; free workspace, return C */
}
