
#include "rmsvw.h"


int
crand(void)
{
	static int ctx=0;
#ifndef  USE_WEAK_SEEDING
/*
 * Historic implementation compatibility.
 * The random sequences do not vary much with the seed,
 * even with overflowing.
 */
	return ((ctx = ctx * 1103515245 + 12345) % ((u_long)RAND_MAX + 1));
#else   /* !USE_WEAK_SEEDING */
/*
 * Compute x = (7^5 * x) mod (2^31 - 1)
 * without overflowing 31 bits:
 *      (2^31 - 1) = 127773 * (7^5) + 2836
 * From "Random number generators: good ones are hard to find",
 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
 * October 1988, p. 1195.
 */
	long hi, lo, x;

	/* Must be in [1, 0x7ffffffe] range at this point. */
	hi = ctx / 127773;
	lo = ctx % 127773;
	x = 16807 * lo - 2836 * hi;
	if (x < 0)
		x += 0x7fffffff;
	ctx = x;
	/* Transform to [0, 0x7ffffffd] range. */
	return (x - 1);
#endif  /* !USE_WEAK_SEEDING */
}

/*-----------------------------------------------------------------------------------------



------------------------------------------------------------------------------------------- */
void *
cmemmove (void *dest, const void *src, size_t n)
{
  char *d = (char *) dest;
  const char *s = (const char *) src;

  if (d < s)
    while (n--)
      *d++ = *s++;
  else
    {
      d += n;
      s += n;
      
      while (n--)
	*--d = *--s;
    }
  
  return dest;
}

int
cmemcmp (const void *s1, const void *s2, size_t n)
{
  const char *t1 = s1;
  const char *t2 = s2;
  
  while (n--)
    {
      if (*t1 != *t2)
	return (int) *t1 - (int) *t2;

      t1++;
      t2++;
    }

  return 0;
}
void *cmemcpy (void *dest, const void *src, size_t count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--){
        //if(*tmp==0){
        //	//printf("fatal : segment failed %s=%d\n", __FUNCTION__, __LINE__);
        //	break;
		//}	
 		*tmp++ = *s++;
   }
	return dest;
}
void *
cmemset (void *s, int c, size_t n)
{
  unsigned char *p = (unsigned char *) s;

  while (n--)
    *p++ = (unsigned char) c;

  return s;
}

char *
cstrcpy (char *dest, const char *src)
{
  char *p = dest;

  while ((*p++ = *src++) != '\0')
    ;

  return dest;
}
char *
cstrcat (char *dest, const char *src)
{
  char *p = dest;

  while (*p)
    p++;

  while ((*p++ = *src++) != '\0')
    ;

  return dest;
}

char *
cstrncat (char *dest, const char *src, int c)
{
  char *p = dest;

  while (*p)
    p++;

  while ((*p++ = *src++) != '\0' && --c)
    ;
  *(--p) = '\0';
  
  return dest;
}

int
cstrcmp (const char *s1, const char *s2)
{
  while (*s1 && *s2)
    {
      if (*s1 != *s2)
	return (int) *s1 - (int) *s2;
      
      s1++;
      s2++;
    }

  return (int) *s1 - (int) *s2;
}

int
cstrncmp (const char *s1, const char *s2, size_t n)
{
  if (n == 0)
    return 0;
  
  while (*s1 && *s2 && --n)
    {
      if (*s1 != *s2)
	return (int) *s1 - (int) *s2;
      
      s1++;
      s2++;
    }

  return (int) *s1 - (int) *s2;
}

size_t
cstrlen (const char *s)
{
  const char *p = s;

  while (*p)
    p++;

  return p - s;
}




