
#pragma once

#include "ofMain.h"

inline void drawArc(  const float & cx, const float cy, const float & radius, const float & thetaStart, const float & arcDeg, const float & hx, const float & hy,  const int & resolution = 20 ){

    float tStep = arcDeg  / (float) resolution;
    float theta = thetaStart;
    
    ofBeginShape();
        for( int i=0; i<resolution; ++i){
            ofVertex( cx + cos(theta) * radius * hx, cy + sin(theta) * radius * hy ); 
            theta += tStep;
        }
        ofVertex( cx + cos(thetaStart+arcDeg) * radius * hx, cy + sin(thetaStart+arcDeg) * radius * hy ); 
    ofEndShape();
    
}
