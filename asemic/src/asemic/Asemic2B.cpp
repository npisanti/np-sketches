
#include "Asemic2B.h"

void np::Asemic2B::setupGlyph( int gh ) {
    
    cx = getGlyphWidth(gh) * 0.5f ;
    cy.resize(2);
    cy[0] = gh * 0.25f; 
    cy[1] = gh * 0.75f; 
    by.resize(3);
    by[0] = 0.0f; 
    by[1] = gh * 0.5f; 
    by[2] = gh; 

    curvature = 0.8f;
    separation = 0.25f;
    dotSize = gh * 0.05f;
    if(dotSize<1.0f) dotSize = 1.0f;
    
}

int np::Asemic2B::getGlyphWidth( int gh ) { 
    return gh*0.65f; 
}
    
int np::Asemic2B::glyph( float gw, float gh, int previous ) {
    
    bool drawn = false;
    
    while( !drawn ){    
        
        for( int i=0; i<2; ++i ){
            
            float choice = ofRandomuf();
            
            if(choice < 0.5f ){
                static const float borderChance = 0.2f;
                
                if(ofRandomuf()<borderChance) { 
                    ofDrawLine( 0.0f, by[i], 0.0f, by[i+1] ); 
                    drawn = true; 
                }
                if(ofRandomuf()<borderChance) { 
                    ofDrawLine( 0.0f, by[i], gw, by[i] ); 
                    drawn = true; 
                }
                if(ofRandomuf()<borderChance) { 
                    ofDrawLine( 0.0f, by[i+1], gw, by[i+1] ); 
                    drawn = true; 
                }
                
                int inner = rand()%9;
                
                switch( inner ){
                    case 0: 
                        ofFill();
                        ofDrawCircle( cx, cy[i], dotSize );
                        drawn = true;
                    break;
                        
                    case 1: case 2: 
                        ofDrawLine( 0.0f, by[i], gw, by[i+1] ); 
                        drawn = true; 
                    break;
                    
                    case 3: case 4: 
                        ofDrawLine( 0.0f, by[i+1], gw, by[i] ); 
                        drawn = true; 
                    break;
                    
                    default: break;
                }
                
            } 
            
            else if ( choice <0.6f ) {
                int mode = rand()%3;
                ofNoFill();
                
                switch( mode ){
                    case 0:
                        drawArc( 0.0f, cy[i], gh*0.25f, TWO_PI*0.75f, PI, curvature, 1.0f, 15);
                    break;

                    case 1:
                        ofDrawLine( 0.0f, by[i], 0.0f, by[i+1] );
                        drawArc( cx, by[i], gw*0.5f, 0.0f,  PI, 1.0f, curvature, 15);
                    break;

                    case 2:
                        ofDrawLine( 0.0f, by[i], 0.0f, by[i+1] );
                        drawArc( cx, by[i+1], gw*0.5f, TWO_PI*0.5f, PI, 1.0f, curvature, 15);
                    break;
                } 
                drawn = true;
            }
                    
            /*
            else if ( choice <0.8f ){
                int mode = rand()%2;
                static const float lineChance = 0.2f;
                switch( mode ){
                    case 0:
                        ofDrawCircle( 0.0f, by[i], dotSize );
                        if(ofRandomuf()<lineChance) ofDrawLine( gw*separation, by[i], gw, by[i] );
                        if(ofRandomuf()<lineChance) ofDrawLine( gw*separation, by[i]+gh*separation*0.5f, gw, by[i+1] );
                        if(ofRandomuf()<lineChance) ofDrawLine( 0.0f, by[i]+gh*separation*0.5f, 0.0f, by[i+1] );
                    break;
                    
                    case 1:
                        ofDrawCircle( 0.0f, by[i+1], dotSize );
                        if(ofRandomuf()<lineChance) ofDrawLine( gw*separation, by[i+1], gw, by[i+1] );
                        if(ofRandomuf()<lineChance) ofDrawLine( gw*separation, by[i+1]-gh*separation*0.5f, gw, by[i] );
                        if(ofRandomuf()<lineChance) ofDrawLine( 0.0f, by[i+1]-gh*separation*0.5f, 0.0f, by[i] );
                    break;
                }
            }
            */
        }
    }

    return 0;
}
