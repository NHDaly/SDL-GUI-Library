
#include "Compatibility.h"

void initGameWindows(){
	
	freopen("output.txt", "w", stdout);
	CHDIR(MY_RESOURCES_FOLDER);	// This moves the working directory inside the .app	
	
	
}

void initGameMac(){
	
	CHDIR(MY_RESOURCES_FOLDER);	// This moves the working directory inside the .app	
	
}
