#include "cxs.h"
static CXS_INT cxs_nonzero (CXS_INT i, CXS_INT j, CXS_ENTRY aij, void *other)
{
    return (aij != 0) ;
}
CXS_INT cxs_dropzeros (cxs *A)
{
    return (cxs_fkeep (A, &cxs_nonzero, NULL)) ;  /* keep all nonzero entries */
}
