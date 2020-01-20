#include "cxs.h"
/* post order a forest */
CXS_INT *cxs_post (const CXS_INT *parent, CXS_INT n)
{
    CXS_INT j, k = 0, *post, *w, *head, *next, *stack ;
    if (!parent) return (NULL) ;                        /* check inputs */
    post = cxs_malloc (n, sizeof (CXS_INT)) ;                /* allocate result */
    w = cxs_malloc (3*n, sizeof (CXS_INT)) ;                 /* get workspace */
    if (!w || !post) return (cxs_idone (post, NULL, w, 0)) ;
    head = w ; next = w + n ; stack = w + 2*n ;
    for (j = 0 ; j < n ; j++) head [j] = -1 ;           /* empty linked lists */
    for (j = n-1 ; j >= 0 ; j--)            /* traverse nodes in reverse order*/
    {
        if (parent [j] == -1) continue ;    /* j is a root */
        next [j] = head [parent [j]] ;      /* add j to list of its parent */
        head [parent [j]] = j ;
    }
    for (j = 0 ; j < n ; j++)
    {
        if (parent [j] != -1) continue ;    /* skip j if it is not a root */
        k = cxs_tdfs (j, k, head, next, post, stack) ;
    }
    return (cxs_idone (post, NULL, w, 1)) ;  /* success; free w, return post */
}
