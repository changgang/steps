#include "cxs.h"
static CXS_INT cxs_tol (CXS_INT i, CXS_INT j, CXS_ENTRY aij, void *tol)
{
    return (CXS_ABS (aij) > *((double *) tol)) ;
}
CXS_INT cxs_droptol (cxs *A, double tol)
{
    return (cxs_fkeep (A, &cxs_tol, &tol)) ;    /* keep all large entries */
}
