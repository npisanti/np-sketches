#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    gui.setup("panel", "settings.xml", ofGetWidth()-220, 20 );
    gui.add( travelers.parameters );
    gui.add( color.set("color", ofColor(255, 80, 80), ofColor(0), ofColor(255)) );
    ofBackground(0);
    
    ofSetWindowTitle( "mouse controlled traveler" );
}

//--------------------------------------------------------------
void ofApp::update(){
    travelers.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor( color );
    travelers.draw();
    gui.draw();    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    travelers.move( x, y );
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    travelers.enter();
    travelers.move( x, y );
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    travelers.move( x, y );
    travelers.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
