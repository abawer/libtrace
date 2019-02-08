#pragma once

#include <list>
#include <cstdio>
#include <cstdlib>

#ifndef MAX_ALLOCS
#define MAX_ALLOCS 10000
#endif

struct AllocInfo
{
	void*  ptr;
	size_t size;
	char bracketAlloc;
	AllocInfo(void* p = NULL, size_t s = 0, char bracket = 0) : ptr(p),size(s),bracketAlloc(bracket){}
}
;

class Trace
{
private:
	AllocInfo info[MAX_ALLOCS];
public:
	Trace()	{fprintf(stdout,"started tracer for MAX_ALLOCS=%d\n", MAX_ALLOCS);}
	~Trace(){ report(); }

	bool addAlloc(void* p, size_t size, char bracketAlloc = 0)
	{
		for (int i = 0 ; i < MAX_ALLOCS; ++i)
		{
			if (info[i].ptr == NULL)		
			{
				info[i] = AllocInfo(p, size, bracketAlloc);
				return true;
			}
		}
		return false;		
	}
	bool removeAlloc(void* p, char bracketDealloc = 0)
	{
		for (int i = 0 ; i < MAX_ALLOCS; ++i)
		{
			if (info[i].ptr == p)		
			{
				if (bracketDealloc != info[i].bracketAlloc)
				{
					fprintf(stderr,"!!!attempted to free allocted address %p with incompatible free (bracket for non-bracket)!!!\n", p);
					return false;
				}
				info[i].ptr = NULL;
				return true;				
			}
		}
		fprintf(stderr,"!!!attempted to free unallocated address %p!!!\n", p);
		return false;		
	}
private:
	void report()
	{
		for (int i = 0 ; i < MAX_ALLOCS; ++i)
		{
			if (info[i].ptr == NULL)
				continue;
			fprintf(stderr,"!!!leak for unfreed address %p size %lu bracket %u!!!\n", info[i].ptr, info[i].size, info[i].bracketAlloc);
		}
	}	
}
trace;

void* trmalloc(size_t size, const char* filename, int line,  const char* funcname, char bracketNew = 0)
{
		void* p = std::malloc(size);
        fprintf(stdout,"%s:%d:%s(...) allocated %p with size %lu\n", filename, line, funcname, p, size);
		if (!trace.addAlloc(p, size, bracketNew))
		{
			fprintf(stderr,"reached MAX_ALLOCS=%d trace limit, compile your program with -DMAX_ALLOCS=some larger number... aborting trace\n",MAX_ALLOCS);
			exit(0);			
		}
        return p;
}

void trfree(void* p, char bracketDel = 0) 
{
 		if (trace.removeAlloc(p, bracketDel))
			std::free(p);
}

void* operator new (size_t size, const char* filename, int line, const char* funcname)
{
	return trmalloc(size, filename, line, funcname);
}

void operator delete (void* p)
{
	trfree(p);
}

void* operator new[] (size_t size, const char* filename, int line, const char* funcname)
{
    return trmalloc(size, filename, line, funcname, true);
}

void operator delete[] (void* p)
{
     trfree(p, true);
}

#define new new(__FILE__,__LINE__,__FUNCTION__)
#define free(x) trfree(x)
#define malloc(x) trmalloc(x,__FILE__,__LINE__,__FUNCTION__)