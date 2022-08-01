//
//  main.cpp
//  threadexample
//
//  Created by Vincent Moscaritolo on 8/1/22.
//

#include <stdio.h>
#include <stdlib.h>   // exit()

#include "FooMgr.hpp"

int main(int argc, const char * argv[]) {
	
	FooMgr* foo;
	
	foo = new FooMgr;
	
	if(!foo->begin()) {
		return 0;
	}

	foo->setShouldRead(true);
	
	while(true) {
 		sleep(10);
	}
	
 
 	return 0;
}
