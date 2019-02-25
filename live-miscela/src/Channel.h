
#pragma once 

#include "ofMain.h"
#include "ofxDotFrag.h"
#include "np-extra.h"
#include "miscela/DotFrag.h"
#include "Layer.h"

namespace np{ namespace miscela {

class Channel{
public:
    Channel();

    void setup( np::PaletteTable & palette, int w, int h );

    void update( const glm::vec3 & position,float mod=0.0f, float speed=1.0f, int colA=0, int colB=1 );

    Layer sketch;
    Layer overlay;
    
    np::PaletteTable * palette;
    
    ofParameterGroup parameters;
        ofParameter<float> sketchControl;
        ofParameter<float> sketchMod;
        ofParameter<float> overlayControl;
        ofParameter<float> overlayMod;

    ofFbo fbo;
};  

}}
