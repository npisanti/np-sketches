#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxPDSPPatches.h"
#include "ofxLaserManager.h"
#include "ofxLaserDacEtherdream.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void patch();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        ofxPanel gui;
        
        // graphics ------------------------------
        void sigilcraft( int glyph, std::vector<ofPolyline> & vessel, float x, float y, float w );
        


        std::vector<std::vector<ofPolyline>> sigil;
        ofParameterGroup graphics;
        ofParameter<float> padding;
        ofParameter<int> gw;
        ofParameter<float> cp0x;
        ofParameter<float> cp0y;
        ofParameter<float> cp1x;
        ofParameter<float> cp1y;
        
        ofParameter<float> tilde;
        ofParameter<float> gh;
        ofParameter<float> hlSpacing;
        ofParameter<float> offx;
        ofParameter<float> offy;
        ofParameter<ofColor> color;

        std::vector<ofPolyline> randoms;
        std::vector<float> xvalues;
        
        ofParameter<bool> bGraphics;
        
        int laststep;
        // pdsp modules --------------------------
        pdsp::Engine   engine;
        
        ofx::patch::synth::SinePercussion zap;
        ofx::patch::synth::NoiseDevice noise;
        ofx::patch::synth::NoiseDevice click;

        ofx::patch::sequence::MarkovChain mc;
    
    
        // laser -----------------------------------            
        ofxLaser::Manager laser;
        ofxLaser::DacEtherdream dac;
        bool drawingShape = false;
        int laserWidth;
        int laserHeight;

    

    
};
