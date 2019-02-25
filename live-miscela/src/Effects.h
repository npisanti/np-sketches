
#pragma once

#include "ofMain.h"
#include "ofxDotFrag.h"

namespace miscela{

class Effects{
public:
    void setup( ofFbo & fbo  );
    
    void process( float mod );


    
    void draw( int x, int y );   
 
    std::vector<ofx::dotfrag::Base*> effects;
    
    ofx::dotfrag::Live live;
    ofx::dotfrag::Twist twist;
    ofx::dotfrag::Turbolence turbolence;

    ofParameterGroup parameters;
        ofParameter<bool> active;
        ofParameter<int> index;
        ofParameter<float> controlA;
        ofParameter<float> modA;
        ofParameter<float> controlB;
        ofParameter<float> modB;
        
        ofParameter<bool> nextButton;
    
    ofParameter<float> u_control_a;
    ofParameter<float> u_control_b;
    
private:
    void onNextButton( bool & value );
    void onIndexChange( int & value );
    void updateParameters( float a, float b );
    
    float oldA;
    float oldB;
    float mod;
    
    ofFbo * pFbo;
    
    ofFbo names;
};
    
}
