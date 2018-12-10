
#include "AsemicCircles.h"


void np::AsemicCircles::setupGlyph( int gh ) {
    cx = gh*0.5f;
    cy = gh*0.5f;
    dim = gh * 0.48f;
    
    points = 12;
    tJump = TWO_PI / (float) points;
    
    r0 = dim * 0.3f;
    r2 = dim;
    r1 = r0*0.35f + r2*0.65f;
    
    cSize = r0*2.0f; 
}

int np::AsemicCircles::getGlyphWidth( int gh ) { 
    return gh; 
}
    
int np::AsemicCircles::glyph( float gw, float gh, int previous ) {

    if( ofRandomuf()<0.5f ) {
        ofFill();
    }else{
        ofNoFill();
    }

    const int resolution = 30;
    float tStep = TWO_PI  / (float) resolution;
    float theta = 0.0f;

    ofBeginShape();
    for( int i=0; i<resolution; ++i){
        ofVertex( cx + cos(theta)*r0, cy + sin(theta)*r0 ); 
        theta += tStep;
    }
    ofEndShape( true );    
    
    theta = M_PI_2; // 0.5pi

    for (int i=0; i<points; ++i ){
        
        int t = rand()%5;
        
        switch( t ) {
            case 0: case 1: case 2: break;
            
            case 3:
            {
                float st = sin(theta);
                float ct = cos(theta);
                ofDrawLine( cx + ct*r1, cy + st*r1, cx + ct*r2, cy + st*r2 );
            }
            break;
                        
            case 4:
            {
                float st = sin(theta);
                float ct = cos(theta);
                ofDrawLine( cx + ct*r0, cy + st*r0, cx + ct*r2, cy + st*r2 );
            }
            break;
        }
        
        theta += tJump;
    } 
    

    return 0;
}


