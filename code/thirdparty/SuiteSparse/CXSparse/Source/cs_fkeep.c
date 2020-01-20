#include "cxs.h"
/* drop entries for which fkeep(A(i,j)) is false; return nz if OK, else -1 */
CXS_INT cxs_fkeep (cxs *A, CXS_INT (*fkeep) (CXS_INT, CXS_INT, CXS_ENTRY, void *), void *other)
{
    CXS_INT j, p, nz = 0, n, *Ap, *Ai ;
    CXS_ENTRY *Ax ;
    if (!CXS_CSC (A) || !fkeep) return (-1) ;    /* check inputs */
    n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    for (j = 0 ; j < n ; j++)
    {
        p = Ap [j] ;                        /* get current location of col j */
        Ap [j] = nz ;                       /* record new location of col j */
        for ( ; p < Ap [j+1] ; p++)
        {
            if (fkeep (Ai [p], j, Ax ? Ax [p] : 1, other))
            {
                if (Ax) Ax [nz] = Ax [p] ;  /* keep A(i,j) */
                Ai [nz++] = Ai [p] ;
            }
        }
    }
    Ap [n] = nz ;                           /* finalize A */
    cxs_sprealloc (A, 0) ;                   /* remove extra space from A */
    return (nz) ;
}
