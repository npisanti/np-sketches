
#include "Effects.h"
#include "Constants.h"

void miscela::Effects::setup( ofFbo & fbo ){
    pFbo = &fbo;
    mod = 0.0f;
    
    //effects.push_back( &live );
    effects.push_back( &twist );
    effects.push_back( &turbolence );
    
    parameters.setName("effects");
    parameters.add( active.set("active", false) );
    parameters.add( index.set("index", 1, 0, effects.size()-1) );
    parameters.add( controlA.set("control A", 0.0f, 0.0f, 1.0f) );
    parameters.add( modA.set("mod A", 0.0f, 0.0f, 1.0f) );
    parameters.add( controlB.set("control B", 0.0f, 0.0f, 1.0f) );
    parameters.add( modB.set("mod B", 0.0f, 0.0f, 1.0f) );
    
    nextButton.set( "next", false );
    nextButton.addListener( this, &miscela::Effects::onNextButton );
    
    //live.label( "live" );
    //live.load( ofToDataPath("livefx.frag") );
    //live.uniform( u_control_a, "u_control_a" );
    //live.uniform( u_control_b, "u_control_a" );
    
    index.addListener( this, &miscela::Effects::onIndexChange );
    
    names.allocate( LABELW, LABELH*effects.size() );
    names.begin();
        ofClear( 0, 0, 0, 0 );
        for( size_t i=0; i<effects.size(); ++i ){
            ofDrawBitmapString( effects[i]->parameters.getName(), TEXTOFF_X, i*LABELH + TEXTOFF_Y);
        }
    names.end();
}

void miscela::Effects::process( float mod ){
    this->mod = mod;
    float a = controlA + mod*modA;
    a = (a<1.0f) ? a : 1.0f;
    float b = controlB + mod*modB;
    b = (b<1.0f) ? b : 1.0f;
    if( a!= oldA || b!=oldB ){
        updateParameters( a, b );
    }
    if( active ){
        if( index==0 ){
            if( u_control_a != a ){ u_control_a = a; }
            if( u_control_b != b ){ u_control_b = b; }
        }
        effects[index]->apply( *pFbo );
    } 
}

void miscela::Effects::onNextButton( bool & value ){
    if( value ){
        int i= index;
        i++;
        if( i >= int( effects.size() ) ){
            i=0;
        }
        index = i;        
    }
}

void miscela::Effects::draw( int x, int y ){
    ofPushMatrix();
    ofPushStyle();
        ofTranslate( x, y );
        ofSetColor( 60 );
        ofFill();
        ofDrawRectangle(0, index*LABELH, LABELW, LABELH );
     
        ofSetColor( 255 );
        names.draw( 0, 0 );
        
        ofSetColor( 60 );
        ofNoFill();
        ofDrawRectangle( 0, 0, names.getWidth(), names.getHeight() );
    ofPopMatrix();
    ofPopStyle();
}

void miscela::Effects::onIndexChange( int & value ){
    float a = controlA + mod*modA;
    a = (a<1.0f) ? a : 1.0f;
    float b = controlB + mod*modB;
    b = (b<1.0f) ? b : 1.0f;
    updateParameters( a, b );
}

void miscela::Effects::updateParameters( float a, float b ){
    switch( index ){
        case 0: break;
    
        case 1: // twist
            twist.amount = a * twist.amount.getMax();
            twist.variation = b * twist.variation.getMax();
        break;
        
        case 2: // turbolence
            turbolence.x = a*turbolence.x.getMax();
            turbolence.y = b*turbolence.y.getMax();
        break;
    }

    oldA = a;
    oldB = b;
}
