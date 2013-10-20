//
//  main.c
//  TestObjC
//
//  Created by Resident Evil on 23.07.13.
//  Copyright (c) 2013 Resident Evil. All rights reserved.
//

#include <stdio.h>
#include "REApplication.h"

int main(int argc, const char * argv[])
{
	REApplication * app = new REApplication();
	
	
	// insert code here...
	printf("Hello, World!\n");
	
	delete app;
	
    return 0;
}

