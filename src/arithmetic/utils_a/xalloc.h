#ifndef XALLOC_H
#define XALLOC_H

#include <stddef.h>

/**
** \brief Safe xmalloc wrapper
** \param size the size to allocate
** \return a pointer to the allocated memory
*/
void *xmalloc(size_t size);

/**
** \brief Safe xrealloc wrapper
** \param ptr the pointer to reallocate
** \param size the new size to allocate
** \return a pointer to the allocated memory
*/
void *xrealloc(void *ptr, size_t size);

#endif /* ! XALLOC_H */
