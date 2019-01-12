#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("the eye");
    ofSetWindowShape( width, height );    
    
    fbo.allocate( width, height );
    
    monochrome.allocate( fbo );
    monochrome.active = false;
    
    webcam.setDeviceID( id );
    webcam.setDesiredFrameRate(60);
    webcam.initGrabber(width, height);

    ofBackground(0);

    bDrawGui = false;

    gui.setup( "", "setting.xml", 20, 20 );
    gui.setName( "GUI" );
    
    gui.add( monochrome.parameters );
}

//--------------------------------------------------------------
void ofApp::update(){

    webcam.update();
    if(webcam.isFrameNew()){
        fbo.begin();
            ofClear(0,0,0,0);
            webcam.draw( 0, 0 );
        fbo.end();
        monochrome.apply( fbo );
    }					

}

//--------------------------------------------------------------
void ofApp::draw(){
	fbo.draw( 0, 0 );
	//frag.draw( 0, 0 );
	if(bDrawGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch( key ){
		case 'g': bDrawGui = !bDrawGui;	break;
        case 'm': monochrome.active = ! monochrome.active; break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
