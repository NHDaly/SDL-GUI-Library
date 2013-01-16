
#include "Compatibility.h"

#include "GUIUtility.h"

#ifdef _MSC_VER // other possibilities are WIN32 _WIN32 or _WIN64

#define MY_RESOURCES_FOLDER "\code"

void initGUI(){

    initSDL(SDL_INIT_EVERYTHING);

// Open the stdout as an output file. (By default, SDL on windows supresses stdout)
	freopen("output.txt", "w", stdout);
// Move the working directory to the resources folder.
	_chdir(MY_RESOURCES_FOLDER);
    
}

#else // Mac OSX

#include <CoreFoundation/CFBundle.h>
#include <cassert>
#include <string>

#define MY_RESOURCES_FOLDER "/Contents/Resources"

void initGUI(){
	
    initSDL(SDL_INIT_EVERYTHING);

// Move the working directory to resources folder.
	// Get the name of the application bundle
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef url = CFBundleCopyBundleURL(mainBundle);
    UInt8 bundlePath[PATH_MAX];
    int chdir_success = (CFURLGetFileSystemRepresentation(url, true, bundlePath, sizeof(bundlePath)));
    assert(chdir_success != 0);
    
    // Create the path to the resources folder
    std::string path((const char*)bundlePath);
    path.append(MY_RESOURCES_FOLDER);
    
    // chdir to the resources folder
    chdir_success = chdir(path.c_str());
    assert(chdir_success == 0);
}

#endif
