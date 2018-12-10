
#pragma once

#include "ofMain.h"
#include "AsemicTextBlock.h"
#include "drawArc.h"

namespace np {

class Asemic2B : public AsemicTextBlock {

private:    
    void setupGlyph( int gh ) override;
    int getGlyphWidth( int gh ) override;
    int glyph( float gw, float gh, int previous ) override ;
    
    int cx;
    vector<float> cy;
    vector<float> by;

    float separation;
    float curvature;
    float dotSize;
        
};

}
