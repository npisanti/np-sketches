
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ) {

#ifdef __ARM_ARCH
    ofGLESWindowSettings settings;
    settings.glesVersion = 1;
    settings.setSize( 640, 480 );
    ofCreateWindow(settings); 
#else        
    ofSetupOpenGL( 1280,720, OF_WINDOW);      
#endif

    ofRunApp( new ofApp());
}
