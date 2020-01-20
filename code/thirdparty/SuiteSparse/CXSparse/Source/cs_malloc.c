#include "cxs.h"
#ifdef MATLAB_MEX_FILE
#define malloc mxMalloc
#define free mxFree
#define realloc mxRealloc
#define calloc mxCalloc
#endif

/* wrapper for malloc */
void *cxs_malloc (CXS_INT n, size_t size)
{
    return (malloc (CXS_MAX (n,1) * size)) ;
}

/* wrapper for calloc */
void *cxs_calloc (CXS_INT n, size_t size)
{
    return (calloc (CXS_MAX (n,1), size)) ;
}

/* wrapper for free */
void *cxs_free (void *p)
{
    if (p) free (p) ;       /* free p if it is not already NULL */
    return (NULL) ;         /* return NULL to simplify the use of cxs_free */
}

/* wrapper for realloc */
void *cxs_realloc (void *p, CXS_INT n, size_t size, CXS_INT *ok)
{
    void *pnew ;
    pnew = realloc (p, CXS_MAX (n,1) * size) ; /* realloc the block */
    *ok = (pnew != NULL) ;                  /* realloc fails if pnew is NULL */
    return ((*ok) ? pnew : p) ;             /* return original p if failure */
}
