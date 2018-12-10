
#pragma once

#include "ofMain.h"

namespace np{

class Travelers {
 
public:
    Travelers();
    
    void update();
    void draw();
    void resize( int size );
    
    ofParameterGroup & label( std::string name );
    
    void move( float x, float y, int i=0 );
    void enter( int i=0 );
    void exit( int i=0 );

        
    ofParameterGroup parameters;
        ofParameter<int> segments;
        ofParameter<int> spacing;
        ofParameter<float> angle;
        ofParameter<int> dimension;
        ofParameter<float> anglemod;

private:
    std::vector<int> entered;
    std::vector<ofPolyline> travelers;

};

}
