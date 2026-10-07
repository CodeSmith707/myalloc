#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#define NALLOC 1024 // Minimum number of units to request

typedef union Header {
    struct
    {
        size_t size;
        union Header *next;
    } s;
    max_align_t align; // Forces alignment
} Header;

static Header base;
static Header *freep = NULL;

static Header *morecore(size_t nunits)
{
    char *cp;
    Header *up;

    if (nunits < NALLOC)
        nunits = NALLOC;

    cp = sbrk(nunits * sizeof(Header));
    if (cp == (char *)-1)
        return NULL; // sbrk failed

    up = (Header *)cp;
    up->s.size = nunits;
    free((void *)(up + 1));
    return freep;
}

void *myalloc(size_t size)
{
    Header *p, *prevp;
    size_t nunits = size + sizeof(Header) - 1;

    // Previous list doesn't yet exist
    if ((prevp = freep) == NULL)
    {
        base.s.next = freep = prevp = &base;
        base.s.size = 0;
    }

    for (p = prevp->s.next;; prevp = p, p = p->s.next)
    {
        // If it is big enough
        if (p->s.size >= nunits)
        {
            // If it is exactly the right size
            if (p->s.size == nunits)
            {
                prevp->s.next = p->s.next;
            }
            else
            {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1);
        }
        if (p == freep)
        {
            if ((p = morecore(nunits)) == NULL)
            {
                return NULL; // No memory available
            }
        }
    }
}

void free(void *ptr)
{
}