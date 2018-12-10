
#include "AsemicN4.h"


void np::AsemicN4::setupGlyph( int gh ) {
    
    cx = gh*0.5f;
    cy = gh*0.5f;
    dim = gh * 0.4f;
    
    tilde.allocate(gh, gh);
    
    tilde.begin();
        ofClear( 0, 0, 0, 0 );

        static const float h = 0.8;
            
        float c0x = cx - dim*0.5f;
        float c1x = cx + dim*0.5f;
        int resolution = 30;

        ofSetColor(255);
        ofNoFill();
        
        drawArc( c0x, cy, dim * 0.5f, 0.0f, PI, 1.0f, h, resolution );
        drawArc( c1x, cy, dim * 0.5f, PI, PI, 1.0f, h, resolution );
    tilde.end();
    
}

int np::AsemicN4::getGlyphWidth( int gh ) { 
    return gh; 
}
    
int np::AsemicN4::glyph( float gw, float gh, int previous ) {
    
    int g = rand()%4;

    
    switch( g ){
        case 0:
            ofFill();
            ofDrawCircle(cx, cy, dim*0.3f);
        break;
        
        case 1:
            ofDrawLine( cx, cy-dim, cx, cy+dim);
        break;
        
        case 2:
            ofDrawLine( cx-dim, cy, cx+dim, cy );
        break;
        
        case 3:
            tilde.draw( 0, 0 );
        break;
        
    }
    
    return g;
}
