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
    Header *bp, *p;

    if (ptr == NULL)
        return;

    bp = (Header *)ptr - 1;
    for (p = freep; !(bp > p && bp < p->s.next); p = p->s.next)
    {
        if (p >= p->s.next && (bp > p || bp < p->s.next))
            break;
    }

    if (bp + bp->s.size == p->s.next)
    {
        bp->s.size += p->s.next->s.size;
        bp->s.next = p->s.next->s.next;
    }
    else
    {
        bp->s.next = p->s.next;
    }

    if (p + p->s.size == bp)
    {
        p->s.size += bp->s.size;
        p->s.next = bp->s.next;
    }
    else
    {
        p->s.next = bp;
    }

    freep = p;
}
