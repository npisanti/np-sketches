#include "ofApp.h"

// particle system test
// actually the rPi is still too slow for using also a shader
// maybe a solution (if needed) could be to make the ofxParticle.h multithreaded

//--------------------------------------------------------------
void ofApp::setup(){
    
    //np::linux_no_cursor_fix();    
    ofBackground(0, 0, 0);
    
    bAA = true;
    ofEnableAntiAliasing();

    ps.setup( ofGetWidth(), ofGetHeight() );

    gui.setup("", "settings.xml", 20, 20);
    gui.add( ps.ui );
	gui.add(color.set("color",ofColor(255, 20, 20),ofColor(0,0),ofColor(255,255)));

#ifndef __ARM_ARCH
    ofSetWindowTitle("particle system test");
    gui.loadFromFile("settings.xml");
    bDrawGui = true;
#else
    ofSetFrameRate(24);
    ps.speed = 1.0f;
    ps.trail = 0.5f;
    bDrawGui = false;
#endif

	//color = ofColor(255, 20, 20);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ps.update( color );

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ps.draw(0, 0);

    if(bDrawGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ps.attractors[0].x = x;
    ps.attractors[0].y = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ps.attractors[0].x = x;
    ps.attractors[0].y = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ps.attractors[0].x = x;
    ps.attractors[0].y = y;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'v' ) ps.bShowField = true;
    if( key == 'g' ) bDrawGui = bDrawGui ? false : true;
    if( key == 'a'){
		bAA = !bAA;
		if(bAA){
			ofEnableAntiAliasing();
			cout << "activating AA\n";
		} else {
			ofDisableAntiAliasing();
			cout << "deactivating AA\n";
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'v' ) ps.bShowField = false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
