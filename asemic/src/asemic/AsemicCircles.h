
#pragma once

#include "ofMain.h"
#include "AsemicTextBlock.h"

namespace np {

class AsemicCircles : public AsemicTextBlock {

public: 
	AsemicCircles() { direction = VerticalLeftToRight; };

private:    
    void setupGlyph( int gh ) override;
    int getGlyphWidth( int gh ) override;
    
    int glyph( float gw, float gh, int previous ) override ;
    
    int cx;
    int cy;
    int dim;

    
    float r0;
    float r1;
    float r2;
    float cSize;
    
    float tJump;
    int   points;
};

}
