#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char *argv[] ){
    
    ofGLFWWindowSettings settings;
    
    settings.setSize( PROJECTOR_WIDTH + 240, PROJECTOR_HEIGHT + 400 );
    settings.setPosition(ofVec2f(100, 100));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    shared_ptr<ofApp> mainApp(new ofApp);
    
    if(argc>1){		
        std::string path = std::string( argv[1] );	
        mainApp->cmdpath = path;
        mainApp->full = false;
	}else{
        settings.setSize( PROJECTOR_WIDTH, PROJECTOR_HEIGHT );
        settings.setPosition(ofVec2f(ofGetScreenWidth(), 0));
        settings.resizable = false;
        settings.decorated = false;
        settings.shareContextWith = mainWindow;
        shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(settings);
        secondWindow->setVerticalSync(false);
        
        mainApp->cmdpath = "";
        mainApp->full = true;
        ofAddListener(secondWindow->events().draw, mainApp.get(), &ofApp::drawSecondWindow);
    }
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
}
