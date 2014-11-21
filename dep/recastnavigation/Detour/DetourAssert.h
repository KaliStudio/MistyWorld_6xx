

#ifndef DETOURASSERT_H
#define DETOURASSERT_H


#ifdef NDEBUG

#	define dtAssert(x) do { (void)sizeof(x); } while((void)(__LINE__==-1),false)  
#else
#	include <assert.h> 
#	define dtAssert assert
#endif

#endif // DETOURASSERT_H
