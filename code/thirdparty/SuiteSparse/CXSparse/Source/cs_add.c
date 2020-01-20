#include "cxs.h"
/* C = alpha*A + beta*B */
cxs *cxs_add (const cxs *A, const cxs *B, CXS_ENTRY alpha, CXS_ENTRY beta)
{
    CXS_INT p, j, nz = 0, anz, *Cp, *Ci, *Bp, m, n, bnz, *w, values ;
    CXS_ENTRY *x, *Bx, *Cx ;
    cxs *C ;
    if (!CXS_CSC (A) || !CXS_CSC (B)) return (NULL) ;         /* check inputs */
    if (A->m != B->m || A->n != B->n) return (NULL) ;
    m = A->m ; anz = A->p [A->n] ;
    n = B->n ; Bp = B->p ; Bx = B->x ; bnz = Bp [n] ;
    w = cxs_calloc (m, sizeof (CXS_INT)) ;                       /* get workspace */
    values = (A->x != NULL) && (Bx != NULL) ;
    x = values ? cxs_malloc (m, sizeof (CXS_ENTRY)) : NULL ;    /* get workspace */
    C = cxs_spalloc (m, n, anz + bnz, values, 0) ;           /* allocate result*/
    if (!C || !w || (values && !x)) return (cxs_done (C, w, x, 0)) ;
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    for (j = 0 ; j < n ; j++)
    {
        Cp [j] = nz ;                   /* column j of C starts here */
        nz = cxs_scatter (A, j, alpha, w, x, j+1, C, nz) ;   /* alpha*A(:,j)*/
        nz = cxs_scatter (B, j, beta, w, x, j+1, C, nz) ;    /* beta*B(:,j) */
        if (values) for (p = Cp [j] ; p < nz ; p++) Cx [p] = x [Ci [p]] ;
    }
    Cp [n] = nz ;                       /* finalize the last column of C */
    cxs_sprealloc (C, 0) ;               /* remove extra space from C */
    return (cxs_done (C, w, x, 1)) ;     /* success; free workspace, return C */
}
