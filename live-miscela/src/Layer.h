
#pragma once 

#include "ofMain.h"
#include "ofxDotFrag.h"
#include "np-extra.h"
#include "miscela/DotFrag.h"
#include "miscela/Lua.h"

namespace np{ namespace miscela {

class Layer{
public:
    Layer();
    
    void resize( int width, int height );
    void load( std::string path );

    void update( ofFbo & fbo, float control, float speed, const glm::vec3 & position, const ofColor & colorA, const ofColor & colorB );
    
    bool isFragment(){ return mode==0; }
    bool requiresClear(){ return mode<2; }
    
    int mode;
    np::miscela::DotFrag  frag;
    np::miscela::Lua      lua;
   
    bool loaded;
    
    int width;
    int height;
};  

}}
