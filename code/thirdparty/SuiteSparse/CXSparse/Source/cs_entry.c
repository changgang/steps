#include "cxs.h"
/* add an entry to a triplet matrix; return 1 if ok, 0 otherwise */
CXS_INT cxs_entry (cxs *T, CXS_INT i, CXS_INT j, CXS_ENTRY x)
{
    if (!CXS_TRIPLET (T) || i < 0 || j < 0) return (0) ;     /* check inputs */
    if (T->nz >= T->nzmax && !cxs_sprealloc (T,2*(T->nzmax))) return (0) ;
    if (T->x) T->x [T->nz] = x ;
    T->i [T->nz] = i ;
    T->p [T->nz++] = j ;
    T->m = CXS_MAX (T->m, i+1) ;
    T->n = CXS_MAX (T->n, j+1) ;
    return (1) ;
}
