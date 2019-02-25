
#include "Layer.h"
#include "Constants.h"

//--------------------------------------------------------------

np::miscela::Layer::Layer(){
    loaded = false;
    mode = -1;
    width = 0;
    height = 0;
}

void np::miscela::Layer::resize( int width, int height ){
    this->width = width;
    this->height = height;
    lua.allocate( width, height );
}

void np::miscela::Layer::load( std::string path ){
    std::string ext = ofFilePath::getFileExt( path );
    if( ext == "frag" ){
        loaded = true;
        frag.load( path );
        mode = 0;
    }else if( ext == "lua" ){
        loaded = true;
        lua.load( path );
        mode = 1;
    }else{
        loaded = false;
        mode = -1;
    }
}

void np::miscela::Layer::update( ofFbo & fbo, float control, float speed, const glm::vec3 & position, const ofColor & colorA, const ofColor & colorB ){
    if( loaded ){
        switch( mode ){
            case 0: // shader
                if( frag.control != control ){ frag.control = control; }
                frag.position.set( position );
                frag.speed = speed;
                frag.colorA = colorA;
                frag.colorB = colorB;

                frag.apply( fbo );            
            break;
            
            case 1: // lua
                lua.setBoundaries( ofRectangle( 0, 0, fbo.getWidth(), fbo.getHeight()) );
                lua.position.set( position );
                lua.control = control;
                lua.speed = speed;
                lua.update();
                lua.colorA = colorA;
                lua.colorB = colorB;
                
                fbo.begin();
                    lua.draw();
                fbo.end();
            break;
            
            default: break;
        }
    }
}


