

#include <unistd.h>
#include "xalloc.h"

typedef long Align;    /* for alignment to long boundary */

union header {   /* block header: */
	struct {
		union header *ptr;  /* next block if on free list */
		size_t size;     /* size of this block */
	} s;
	Align x;             /* force alignment of blocks */
};

typedef union header Header;

static Header base;   /* empty list to get started */

static Header  *freep = NULL;  /* start of the free list */

#define NALLOC 1024

static Header *morecore(size_t nu)
{
	char *cp;
	Header *up;

	if (nu < NALLOC)	
		nu = NALLOC;
	cp= sbrk(nu * sizeof(Header));
	if (cp == (char *) -1) /* no space at all */
		return NULL;
	up = (Header *) cp;
	up ->s.size = nu;
	xfree((void *)(up+1));
	return freep;
}

/* xmalloc: general-purpose storage allocator */
void *xmalloc (size_t nbytes)
{
	Header  *p, *prevp;
	size_t nunits;
	/* 
       Calcula el número de Aligns necesarios para datos + cabecera.
       - Se redondea nbytes al múltiplo de sizeof(Align)
       - Se añade el espacio para la cabecera (en Aligns)
    */
	nunits = (nbytes + sizeof(Align) - 1) / sizeof(Align) + sizeof(Header)/sizeof(Align);

	if (( prevp = freep) == NULL ) { /* no free list yet */
		base.s.ptr = freep = prevp = & base; 
		base.s.size = 0;
	}

	for (p= prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {  /* big enough */
			if (p->s.size == nunits)  /* exactly */
				prevp->s.ptr = p->s.ptr;
			else {  /* allocate tail end */
				p->s.size -= nunits;
				p = (Header *)((Align *)p + p->s.size); // cambio: convertimos p a Align * antes de sumar, y luego volvemos a Header *
				p->s.size = nunits;
			}
			freep = prevp; /* estrategia next-fit */
			return (void *)((Align *)(p + 1)); // apunta después de la cabecera
		}
		/* Si ha dado toda la vuelta pide mas memoria y vuelve
		   a empezar */
		if (p == freep) /* wrapped around free list */
			if ((p = morecore(nunits)) == NULL)
				return NULL;  /* none left */
	}
}


/* xfree: put block ap in the free list */
void xfree(void *ap)
{
	Header *bp, *p;

	bp = (Header *)ap - 1;  /* point to block header */

	for (p= freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;  /* freed block at start or end of arena */ 

	/* Comprueba compactacion con hueco posterior */
	if (bp + bp->s.size == p->s.ptr) {  /* join to upper nbr */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else
		 bp->s.ptr = p->s.ptr;

	/* Comprueba compactacion con hueco anterior */
	if (p + p->s.size == bp) {         /* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else
		p->s.ptr = bp;

	freep = p; /* estrategia next-fit */
}

void *xrealloc(void * ptr, size_t size)
{
	return NULL;
}
