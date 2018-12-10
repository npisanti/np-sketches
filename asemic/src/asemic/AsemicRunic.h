
#pragma once

#include "ofMain.h"
#include "AsemicTextBlock.h"
#include "drawArc.h"

namespace np {

class AsemicRunic : public AsemicTextBlock {

private:    
    void setupGlyph( int gh ) override;
    int getGlyphWidth( int gh ) override;
    int glyph( float gw, float gh, int previous ) override ;
    
    float cx;
    float cy;

    float separation;
    float curvature;
    float dotSize;
        
};

}
