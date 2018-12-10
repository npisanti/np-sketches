
#include "PSystem.h"

void np::PSystem::setup( int w, int h, int numAttractors ){
   
#ifdef __ARM_ARCH
    fbo.allocate( w, h, GL_RGBA );
#else
    fbo.allocate( w, h, GL_RGBA, 2  );
#endif
    
    fbo.begin();
        ofClear(0, 0, 0, 0);
    fbo.end();
    
    emitters.resize(4);
    
    emitters[0].setPosition(ofVec3f( 5, fbo.getHeight()-5));
    emitters[0].setVelocity(ofVec3f(0.0,0.0));
    emitters[0].posSpread = ofVec3f(10,10.0);
    emitters[0].velSpread = ofVec3f(10.0,10);
    emitters[0].life = 250;
    emitters[0].lifeSpread = 5.0;
    emitters[0].numPars = 3;
    emitters[0].size = 32;

    emitters[1] = emitters[0];
    emitters[1].setPosition(ofVec3f(fbo.getWidth()-5, 5));
    //emitters[1].setVelocity(ofVec3f(-10.0,0.0));

    emitters[2] = emitters[0];
    emitters[2].setPosition( ofVec3f( fbo.getWidth()-5, fbo.getHeight()-5));
    //emitters[2].setVelocity(ofVec3f(0.0,10.0));

    emitters[3] = emitters[0];
    emitters[3].setPosition( ofVec3f( 5, 5 ) );
    //emitters[3].setVelocity(ofVec3f(0.0,-10.0));

    vectorField.allocate(128, 128, 3);
    
    attractors.resize( numAttractors );
    
    ui.setName("particle system");
    ui.add(rotAcc.set("rot. acceleration", 37, 2, 300) );
    ui.add(gravAcc.set("gravity", 55, 2, 500) );
    ui.add(fieldMult.set("vect field", 0.35f, 0.1f, 0.99f) );
    ui.add(drag.set("drag const", 0.96, 0.8f, 0.99f) );
    ui.add(trail.set("particle trail", 0.5f, 0.05f, 1.0f) );
    ui.add(lineW.set("part line width", 1.0f, 1.0f, 3.0f) );
    ui.add(speed.set("update speed", 1.5f, 0.125f, 1.95f) );
    ui.add(life.set("particle life", 250, 100, 800) );
    ui.add(bFade.set("fade", false ) );
    ui.add(fade.set("fade amount", 255, 0, 255) );
    ui.add(bShowField.set("show vector field", false));
    ui.add(bShowInfo.set("show infos", true));
    
    for( size_t i=0; i<attractors.size(); ++i ){
        ui.add( attractors[i].x.set("attractor " + ofToString(i) + " x", fbo.getWidth()*0.5f, 0, fbo.getWidth() ) );
        ui.add( attractors[i].y.set("attractor " + ofToString(i) + " y", fbo.getHeight()*0.5f, 0, fbo.getHeight() ) );
    }
    
    cout<< "[PSystem] ofEnableAlphaBlending()\n";
    ofEnableAlphaBlending();

}

void np::PSystem::update( const ofColor & color ){

	for(size_t i=0; i<emitters.size(); ++i){
		emitters[i].life = life;
	}
    
    for(int y = 0; y < vectorField.getHeight(); y++) {
        for(int x=0; x< vectorField.getWidth(); x++) {
            float angle = ofNoise(x/(float)vectorField.getWidth()*4.0, y/(float)vectorField.getHeight()*4.0,ofGetElapsedTimef()*0.05)*TWO_PI*2.0;
            ofVec2f dir(cos(angle), sin(angle));
            dir.normalize().scale(ofNoise(x/(float)vectorField.getWidth()*4.0, y/(float)vectorField.getHeight()*4.0,ofGetElapsedTimef()*0.05+10.0));
            vectorField.setColor(x, y, ofColor_<float>(dir.x,dir.y, 0));
        }
    }

    for(size_t i=0; i<attractors.size(); ++i){
        particleSystem.gravitateTo( ofPoint(attractors[i].x, attractors[i].y), gravAcc, 1, 10.0, false);
        particleSystem.rotateAround(ofPoint(attractors[i].x, attractors[i].y), rotAcc, 10.0, false);                
    }

    particleSystem.applyVectorField(vectorField.getData(), vectorField.getWidth(), vectorField.getHeight(), vectorField.getNumChannels(), ofGetWindowRect(), fieldMult);

    //float dt = (ofGetLastFrameTime() * 60.0f * speed );
    
    // todo : use std::chrono high resolution clock for accurate time step 
    float dt = speed;

    particleSystem.update(dt, drag);

	for(size_t i=0; i<emitters.size(); ++i){
		particleSystem.addParticles(emitters[i]);
	}

    fbo.begin();
        if( bFade ){
            ofFill();
            ofSetColor(0, fade );
            ofDrawRectangle( -1, -1, fbo.getWidth()+2, fbo.getHeight()+2 );
        }else{
            ofClear(0, 0, 0, 0);
        }        
        ofSetColor( color );

        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofPushStyle();
        ofSetLineWidth( lineW );
        particleSystem.draw( trail );
        ofPopStyle();
        ofDisableBlendMode();
    fbo.end();

}

void np::PSystem::draw() {
    draw( 0, 0 );
}

void np::PSystem::draw( int x, int y, bool diagnostics ){
   
    ofSetColor( 255 );
    fbo.draw( x, y );

    if( bShowField && diagnostics ){
        ofMesh fieldMesh;
        fieldMesh.disableIndices();
        fieldMesh.setMode(OF_PRIMITIVE_LINES);
        for(int y = 0; y < vectorField.getHeight(); y++) {
            for(int x=0; x< vectorField.getWidth(); x++) {
                ofColor_<float> c = vectorField.getColor(x, y);
                ofVec3f pos(x,y);
                ofVec3f dir(c.r, c.g);
                fieldMesh.addVertex(pos);
                fieldMesh.addColor(c);
                fieldMesh.addVertex(pos + dir);
                fieldMesh.addColor(c);
            }
        }

        ofSetLineWidth(1.0);
        ofSetColor(80, 80, 80);
        ofPushMatrix();
        ofScale(fbo.getWidth()/(float)vectorField.getWidth(), fbo.getHeight()/(float)vectorField.getHeight());
        fieldMesh.draw();
        ofPopMatrix();
    }
    
    if(bShowInfo && diagnostics ){   
        for(size_t i=0; i<attractors.size(); ++i) {
            ofNoFill();
            ofSetCircleResolution(180);
            ofSetColor(255, 0, 0, 125);
            ofDrawCircle( attractors[i].x, attractors[i].y, gravAcc);
            ofSetColor( 100, 100, 100, 1 );
            ofDrawCircle(attractors[i].x, attractors[i].y, rotAcc);        
        }
        
        float dt = (ofGetLastFrameTime() * ofGetFrameRate() * speed);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("particles: "+ ofToString(particleSystem.getNumParticles()) + "\nfps: " + ofToString(ofGetFrameRate()) + "\ntimestep: " + ofToString(dt) , 20, fbo.getHeight()-40);        
    }
}

void np::PSystem::moveAttractor( int x, int y, int index ) {
    attractors[index].x = x;
    attractors[index].y = y;
}
