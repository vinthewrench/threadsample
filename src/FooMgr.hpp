//
//  FooMgr.hpp
//  threadexample
//
//  Created by Vincent Moscaritolo on 8/1/22.
//
#pragma once
 

#include <mutex>
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
 
using namespace std;

class FooMgr {
	
public:

	FooMgr();
	~FooMgr();
 
	bool begin(uint64_t queryDelay = 5 /* seconds*/ );
	bool begin(uint64_t queryDelay,  int &error);
	 
	void setQueryDelay(uint64_t);

	bool setShouldRead(bool shouldRead);
	bool shouldRead() {return _shouldRead;};
 
	void stop();
	bool isConnected() ;
 
	
private:
	bool 				_isSetup = false;
	bool				_shouldRead = false;
 
	struct timespec	_lastQueryTime;
	uint64_t     		_queryDelay;			// how long to wait before next query
 
	void FooReader();		// C++ version of thread
	// C wrappers for W1Reader;
	static void* FooReaderThread(void *context);
	static void FooReaderThreadCleanup(void *context);
	bool 			_isRunning = false;
 
  pthread_cond_t 		_cond = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t 	_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_t				_TID;
};
