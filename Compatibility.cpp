
#include "Compatibility.h"

#include "GUIUtility.h"


void nav_to_resources();


#ifdef _WIN32 // other possibilities are WIN32 _WIN32 or _WIN64

#define MY_RESOURCES_FOLDER "\code"

void initGUI(){

    initSDL(SDL_INIT_EVERYTHING);

// Open the stdout as an output file. (By default, SDL on windows supresses stdout)
	freopen("output.txt", "w", stdout);
    
    // Move the working directory to the resources folder.
    nav_to_resources();
}

void nav_to_resources() {
    int chdir_success = _chdir(MY_RESOURCES_FOLDER);
    assert(chdir_success == 0);
}

#elif __APPLE__ // Mac OSX

# include <CoreFoundation/CFBundle.h>
# include <cassert>
# include <string>

# include <iostream>
using namespace std;

// For App bundle created in Xcode
#  define BUNDLE_RESOURCES_FOLDER "/Contents/Resources"
// For Standalone executable compiled with Makefile.
#  define STANDALONE_RESOURCES_FOLDER "/Resources"

void initGUI(){
	
    initSDL(SDL_INIT_EVERYTHING);
    
    // Move the working directory to resources folder.
    nav_to_resources();
}

void nav_to_resources() {

	// Get the name of the application bundle
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef url = CFBundleCopyBundleURL(mainBundle);
    UInt8 bundlePath[PATH_MAX];
    int success = (CFURLGetFileSystemRepresentation(url, true, bundlePath, sizeof(bundlePath)));
    assert(success != 0);
    
    cout << "BUNDLE PATH: " << (const char*)bundlePath << endl;
    // Create the path to the resources folder
    std::string path((const char*)bundlePath);
    path.append(BUNDLE_RESOURCES_FOLDER);
    
    // chdir to the resources folder
    success = chdir(path.c_str());
    
    // If failed, try with standalone directory
    if (!(success == 0)) {
        std::string path((const char*)bundlePath);
        path.append(STANDALONE_RESOURCES_FOLDER);
        
        // chdir to the resources folder
        success = chdir(path.c_str());
        assert(success == 0);
    }
    
    CFRelease(url);
}

#else // Linux?

#endif
