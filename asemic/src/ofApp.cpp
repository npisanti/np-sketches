#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle( "---" );
    
    ofDisableAntiAliasing();
    
    int w = 460;
    int h = 560;
    int mx = 20;
    int my = 20;
    int multisample = 1;
    
    tb.setup( w, h, 12, 10, mx, my, multisample );
    tb.setLineWidth( 1.2f );
     
    circles.setup( w, h, 20, 4, mx, my, multisample, false );
    n4.setup( w, h, 12, 4, mx, my, multisample, true );
    squares.setup( w, h, 8, 4, mx, my );

    runic.setup( w, h, 12, 6, mx, my, multisample, true );
    
    asemic = &circles;
    division = 1;
    
    ofBackground(0);

    bAutomatic = true;
    bSpace = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if( bAutomatic ){
        if( ofGetFrameNum()%division == 0){
            float chance = ofRandomuf();
            
            if ( chance <0.8f || bSpace || asemic->cursorOnFirstSpaceOfLine() ) {
                asemic->write(1);
                bSpace = false;
            }else if (chance < 0.99f ){
                asemic->whitespace();
                if( asemic==&tb) asemic->whitespace();
                bSpace = true;
            }else{
                if( ! asemic->cursorOnFirstSpaceOfFirstLine() ) asemic->enter();
                if( ! asemic->cursorOnFirstSpaceOfFirstLine() ) asemic->enter();
                bSpace = true;
            }
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor( 255, 120, 0 );
    asemic->draw( 20, 20 );
    
    ofNoFill();
    ofDrawRectangle( 21, 21, asemic->getWidth()-1, asemic->getHeight()-1);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch( key ) {
        
        case '1': asemic = &squares; division=1; break;
        case '2': asemic = &n4; division=1; break;
        case '3': asemic = &circles; division=2; break;
        case '4': asemic = &tb; division=1; break;
        case '5': asemic = &runic; division=1; break;
        
        case OF_KEY_F1 : bAutomatic = !bAutomatic; break;
        
        case ' ':               asemic->whitespace();    break;
        case OF_KEY_RETURN:     asemic->enter();         break;
        case OF_KEY_BACKSPACE:  asemic->clear();         break;
        default:                asemic->write(1);        break;
    }
}

//--------------------------------------------------------------
void ofApp::exit(){}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
