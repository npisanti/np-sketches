
#pragma once

#include "ofMain.h"

namespace miscela {
    
class Folder{

public:
    void setup( std::string folder, int layers );
    
    void resize( int w, int h );
    
    void next( int layer );
    void prev( int layer );
    
    size_t size(){ return files.size(); }
    
    std::string get( int layer );
    
    void draw( int x, int y );

    std::vector<ofParameter<int>> indices;
    
private:

    void indexToCoord( const size_t & index, glm::vec2 & coord );

    ofDirectory files;

    int maxrows;
    
    ofFbo fbo;
};  
    
}
