#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        ofxPanel gui;
        ofParameter<float> rInner;
        ofParameter<float> rOuter;
        
        ofParameter<int> dPoints;
        ofParameter<int> dNum;
        ofParameter<bool> bDrawGuides;

        ofParameter<bool> bFade;
        ofParameter<int> fade;

        void draw_polyline( const ofPolyline & polyline );        
        ofPolyline inner;
        ofPolyline outer;
        ofFbo fbo;
        
        ofParameter<ofColor> color;
        
};
