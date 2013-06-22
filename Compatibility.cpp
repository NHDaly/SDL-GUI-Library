
#include "Compatibility.h"
#include <cassert>
#include <iostream>

#include "GUIUtility.h"

using namespace std;

void nav_to_resources();


#ifdef _WIN32 // other possibilities are WIN32 _WIN32 or _WIN64

const char * MY_RESOURCES_FOLDER = "Resources";

void initGUI(){

    initSDL(SDL_INIT_EVERYTHING);

// Open the stdout as an output file. (By default, SDL on windows supresses stdout)
	FILE* fileout;
	freopen_s(&fileout, "GUILog.txt", "w", stdout);
    
    // Move the working directory to the resources folder.
    nav_to_resources();
}

void nav_to_resources() {
	char outbuf[100];
	_getcwd(outbuf, 100);
	cout << "current dir: " << outbuf << endl;

    int chdir_success = _chdir(MY_RESOURCES_FOLDER);
    assert(chdir_success == 0);
}

const char* getResourcePath() {
	const size_t size = strlen(MY_RESOURCES_FOLDER);
    char *str = new char[size];
    strcpy_s(str, size, MY_RESOURCES_FOLDER);//(str, MY_RESOURCES_FOLDER);
    return (const char*)str;
}


#elif __APPLE__ // Mac OSX

# include <CoreFoundation/CFBundle.h>
# include <string>

# include <iostream>
using namespace std;

// For App bundle created in Xcode
const char * BUNDLE_RESOURCES_FOLDER = "Contents/Resources";
// For Standalone executable compiled with Makefile.
const char * STANDALONE_RESOURCES_FOLDER = "Resources";

void initGUI(){
	
    GUI::initSDL(SDL_INIT_EVERYTHING);
    
    // Move the working directory to resources folder.
    nav_to_resources();
}

const char* getResourcePath() {
    // Get the name of the application bundle
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef url = CFBundleCopyBundleURL(mainBundle);
    UInt8 bundlePath[PATH_MAX];
    int chdir_success = (CFURLGetFileSystemRepresentation(url, true, bundlePath, sizeof(bundlePath)));
    assert(chdir_success != 0);

    cout << "BUNDLE PATH: " << (const char*)bundlePath << endl;
    // Create the path to the resources folder
    std::string path((const char*)bundlePath);
    path.append("/");
    path.append(BUNDLE_RESOURCES_FOLDER);

    char *str = new char[path.length()+1];
    strcpy(str, path.c_str());
    return (const char*)str;
}

void nav_to_resources() {

	// Get the name of the application bundle
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef url = CFBundleCopyBundleURL(mainBundle);
    UInt8 bundlePath[PATH_MAX];
    int chdir_success = (CFURLGetFileSystemRepresentation(url, true, bundlePath, sizeof(bundlePath)));
    assert(chdir_success != 0);
    
    cout << "BUNDLE PATH: " << (const char*)bundlePath << endl;
    // Create the path to the resources folder
    std::string path((const char*)bundlePath);
    path.append("/");
    path.append(BUNDLE_RESOURCES_FOLDER);
    
    // chdir to the resources folder
    chdir_success = chdir(path.c_str());
    // If failed, try with standalone directory
    if (!(chdir_success == 0)) {
        std::string path((const char*)bundlePath);
        path.append("/");
        path.append(STANDALONE_RESOURCES_FOLDER);
        
        // chdir to the resources folder
        chdir_success = chdir(path.c_str());
        assert(chdir_success == 0);
    }
}

#else // Linux?

#endif
