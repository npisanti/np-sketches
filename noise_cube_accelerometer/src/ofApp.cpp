#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofDisableAntiAliasing();
    ofSetVerticalSync(true);

    cube.setResolution(1);
    ofBackground(255);
    
    receiver.setup( 4444 ); // port 4444
    
    gui.setup("", "settings.xml", ofGetWidth()-220, 20);
    gui.add( totalSide.set("side", 300.0f, 100.0f, 1000.0f) );
    gui.add( num.set("squares L", 6, 4, 12 ) );
    gui.add( spacing.set("spacing", 1.5f, 1.0f, 4.0f ) );
    gui.add( distance.set("distance", 600.0f, 200.f, 1000.0f) );
    gui.add( fc.set( "control smooth", 0.05f, 0.01f, 0.1f ));
    gui.loadFromFile("settings.xml");
    
    ofSetWindowTitle("android accelerometer control");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);
        
		if(m.getAddress() == "/zero/ax"){
			ax = m.getArgAsFloat(0);
		}
        
		if(m.getAddress() == "/zero/ay"){
			ay = m.getArgAsFloat(0);
		}
        
		if(m.getAddress() == "/zero/az"){
            az = m.getArgAsFloat(0);
		}
    }

    lpf.setFc(fc);
    lpf.update( ofVec3f(ax, ay, az) );

}

//--------------------------------------------------------------
void ofApp::draw(){

    float side = totalSide / (num*spacing);
        
    rotation += 0.15f + ofMap( lpf.value().z, 1.0f, 3.0f, 0, 1.5f, true );
        
    if (rotation>360.0f) rotation-= 360.0f;

    float tiltX = ofMap( lpf.value().x, -4.0f, 4.0f, -0.95f, 0.95f, true );
    float tiltY = ofMap( lpf.value().y, -4.0f, 4.0f, -0.95f, 0.95f, true );

    camera.lookAt( ofPoint( 0, 0, 0 ) );
    ofVec3f front ( 0, 0, distance );
    ofVec3f rotated = front.getRotated( rotation, ofVec3f( 1, 1, 0 ) );
    camera.setPosition( rotated );

    ofNoFill();
    ofSetDepthTest(true);

    camera.begin();
    for( int z=0; z<num; ++z){
        for( int y=0; y<num; ++y){
            for( int x=0; x<num; ++x){             
                
                float tilt = ofMap( x, 0, num-1, -tiltX, tiltX ) + ofMap( y, 0, num-1, -tiltY, tiltY );
                tilt = (tilt <-0.95f) ? -0.95f : tilt;
                tilt = (tilt > 0.95f) ?  0.95f : tilt;
                float reSide = side * (1.0f+tilt);
                cube.set( reSide );

                cube.setPosition( (0.5f+x-num*0.5f)*side*spacing, (0.5f+y-num*0.5f)*side*spacing, (0.5f+z-num*0.5f)*side*spacing);

                ofSetColor(0);     
                cube.draw();
                    
                ofSetColor(255);  
                cube.setScale(1.01f);
                cube.drawWireframe();
                cube.setScale(1.f);        
            }
        }
    }
    camera.end();

    ofSetDepthTest(false);
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

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
