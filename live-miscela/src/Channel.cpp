
#include "Channel.h"
#include "Constants.h"

//--------------------------------------------------------------

np::miscela::Channel::Channel(){

    parameters.add( sketchControl.set("sketch control", 0.5f, 0.0f, 1.0f) );
    parameters.add( sketchMod.set("sketch mod", 0.0f, 0.0f, 1.0f) );
    
    parameters.add( overlayControl.set("overlay control", 0.5f, 0.0f, 1.0f) );
    parameters.add( overlayMod.set("overlay mod", 0.0f, 0.0f, 1.0f) );

}

void np::miscela::Channel::setup( np::PaletteTable & palette, int w, int h ){
    this->palette = &palette;
    
    fbo.allocate( w, h, GL_RGBA, OVERSAMPLING );
    
    fbo.begin();
        ofClear( 0, 0, 0, 0 );
    fbo.end();
    
    sketch.resize( w, h );
    overlay.resize( w, h );
}


void np::miscela::Channel::update( const glm::vec3 & position, float mod, float speed, int colA, int colB ){

    float ctrl = sketchControl + mod*sketchMod;
    if( ctrl>1.0f){ ctrl = 1.0f; }
    sketch.update( fbo, ctrl, speed, position, palette->color(colA), palette->color(colB) );
    
    ctrl = overlayControl + mod*overlayMod;
    if( ctrl>1.0f){ ctrl = 1.0f; }
    overlay.update( fbo, ctrl, speed, position, palette->color(colA), palette->color(colB) );
}
