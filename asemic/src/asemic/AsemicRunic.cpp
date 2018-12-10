
#include "AsemicRunic.h"

void np::AsemicRunic::setupGlyph( int gh ) {
    
    cx = getGlyphWidth(gh) * 0.5f ;
    cy = gh * 0.5f; 


    separation = 0.2f;
    curvature = (1.0f-separation)*0.5f;

    dotSize = gh * 0.05f;
    if(dotSize<1.0f) dotSize = 1.0f;
    
}

int np::AsemicRunic::getGlyphWidth( int gh ) { 
    return gh; 
}
    
int np::AsemicRunic::glyph( float gw, float gh, int previous ) {
        
    float x0 = separation*0.5f*gw;
    float x1 = gw * (1.0f-separation*0.5f);

    bool drawn = false;
    
    while( !drawn ){    
    
        static const float borderChance = 0.3f;
        
        int horizontals=0;
        int curved = 0;    

        if(ofRandomuf()<borderChance) { 
            if( ofRandomuf() < 0.5f && curved==0 ){
                ofNoFill();
                drawArc( cx, gh, (gw*(1.0f-separation))*0.5f, PI, PI, 1.0f, 1.0f, 20);
                curved = 1; //top
            } else {
                ofDrawLine( x0, gh, x1, gh ); 
            }
            
            horizontals++;
        }
        
        if(ofRandomuf()<borderChance) { 
            
            if( ofRandomuf() < 0.5f && curved==0 ){
                ofNoFill();
                drawArc( cx, 0.0f, (gw*(1.0f-separation))*0.5f, 0.0f, PI, 1.0f, 1.0f, 20);
                curved=2; // bottom                
            } else {
                ofDrawLine( x0, 0.0f, x1, 0.0f ); 
            }

            horizontals++;
        }
        
        if( horizontals>0 ) { 
            drawn = true; 
            
            if( ofRandomuf() < 0.5f && curved==0 ){
                ofNoFill();
                drawArc( x0, cy, gh*0.5f, TWO_PI*0.75f, PI, curvature, 1.0f, 20);
                curved = 3; // vertical
            } else {
                ofDrawLine( x0, 0.0f, x0, gh ); 
                
            }
        }        
        
        if( drawn ){
            int inner = rand()%9;
            switch( inner ){
                case 0: 
                    if( curved==0 ){
                        ofFill();
                        ofDrawCircle( cx, cy, dotSize );
                    }
                break;
                    
                case 1:  
                    if( curved==0 ) ofDrawLine( x0, cy, cx, cy ); 
                break;
                
                case 2: 
                    if(curved!=3) ofDrawLine( cx, cy, cx, 0.0f ); 
                break;
                
                case 3:  
                    if( curved==0) ofDrawLine( cx, cy, x1, cy ); 
                break;
                
                case 4: 
                    if(curved!=3) ofDrawLine( cx, cy, cx, gh ); 
                break;
                
                default: break;
            }
        
        }   

    }

    return 0;
}
