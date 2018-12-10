
#pragma once

#include "ofMain.h"
#include "ofxParticles.h"

namespace np {

class PSystem {

public:
    struct Attractor {
        ofParameter<int> x;
        ofParameter<int> y;
    };

    void setup( int w, int h, int numAttractors=1 );
    void update( const ofColor & color );
    void draw( int x, int y, bool diagnostics=true );
    void draw();

    void moveAttractor( int x, int y, int index=0 );

    ofParameterGroup ui;
        ofParameter<float>  trail;
        ofParameter<float>  lineW;
        ofParameter<float>  speed;        
        ofParameter<int>    rotAcc;
        ofParameter<int>    gravAcc;
        ofParameter<float>  fieldMult;
        ofParameter<float>  drag;
        ofParameter<int>    life;
        ofParameter<int>    fade;
        ofParameter<bool>   bFade;
        ofParameter<bool>   bShowField;
        ofParameter<bool>   bShowInfo;
        
        
        
    vector<Attractor> attractors;
 
    ofFbo fbo;

	vector<ofxParticleEmitter> emitters;

    ofxParticleEmitter topEmitter, botEmitter, leftEmitter, rightEmitter;

private:  
    ofxParticleSystem particleSystem;    
    
    ofFloatPixels vectorField;

};

}
