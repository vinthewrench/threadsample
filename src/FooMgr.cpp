//
//  FooMgr.cpp
//  threadexample
//
//  Created by Vincent Moscaritolo on 8/1/22.
//
 

#include "FooMgr.hpp"

#include <sys/time.h>

#define NSEC_PER_SEC 1000000000
/* Subtract timespecs
 *
 * \param r[out] result: a - b
 * \param a[in] operand
 * \param b[in] operand
 */
static inline void
timespec_sub(struct timespec *r,
		  const struct timespec *a, const struct timespec *b)
{
	r->tv_sec = a->tv_sec - b->tv_sec;
	r->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (r->tv_nsec < 0) {
		r->tv_sec--;
		r->tv_nsec += NSEC_PER_SEC;
	}
}


typedef void * (*THREADFUNCPTR)(void *);

FooMgr::FooMgr() {
	_isSetup = false;
	_queryDelay 		= 5;	// seconds
	_lastQueryTime 	= {0,0};
		_isRunning = true;

	pthread_create(&_TID, NULL,
										  (THREADFUNCPTR) &FooMgr::FooReaderThread, (void*)this);

	
}


FooMgr::~FooMgr(){
	stop();
	
	pthread_mutex_lock (&_mutex);
	_isRunning = false;

	pthread_cond_signal(&_cond);
	pthread_mutex_unlock (&_mutex);

	pthread_join(_TID, NULL);
 
}


bool FooMgr::begin(uint64_t queryDelay){
	int error = 0;
	
	return begin(queryDelay, error);
}


bool FooMgr::begin(uint64_t queryDelay,  int &error){

	if(isConnected())
		return true;
 
	_queryDelay = queryDelay;
	_lastQueryTime 	= {0,0};
	_shouldRead = true;
	_isSetup = true;
	

	return _isSetup;
}

void FooMgr::stop(){
	
	if(_isSetup) {
		_shouldRead = false;
		_isSetup = false;
	}
}

bool  FooMgr::isConnected() {
	return _isSetup;
};

void FooMgr::setQueryDelay(uint64_t delay){
	_queryDelay = delay;
	_lastQueryTime = {0,0};
};


bool FooMgr::setShouldRead(bool shouldRead){
	if(_isSetup && _isRunning){
		_shouldRead = shouldRead;
		return true;
	}
	return false;
}


void FooMgr::FooReader(){
	
	while(_isRunning){
		// if not setup // check back later
		if(!_isSetup ||  !_shouldRead){
			sleep(1);
			continue;
		}
		
	 
		bool shouldQuery = false;
		
		if(_lastQueryTime.tv_sec == 0 &&  _lastQueryTime.tv_nsec == 0 ){
			shouldQuery = true;
		} else {
			
			struct timespec now, diff;
			clock_gettime(CLOCK_MONOTONIC, &now);
			timespec_sub( &diff, &now, &_lastQueryTime);
			
			if(diff.tv_sec >=  _queryDelay  ) {
				shouldQuery = true;
			}
		}
		
		if(shouldQuery){
 
	 // do whatever you need to do here.
			 
			printf("Do foo\n");
			
			clock_gettime(CLOCK_MONOTONIC, &_lastQueryTime);
		}
		else
		{
			sleep(1);
		}
	}
}


void* FooMgr::FooReaderThread(void *context){
	FooMgr* d = (FooMgr*)context;

	//   the pthread_cleanup_push needs to be balanced with pthread_cleanup_pop
	pthread_cleanup_push(   &FooMgr::FooReaderThreadCleanup ,context);
 
	d->FooReader();
	
	pthread_exit(NULL);
	
	pthread_cleanup_pop(0);
	return((void *)1);
}

 
void FooMgr::FooReaderThreadCleanup(void *context){
//	printf("cleanup W1Mgr\n");
}
 
