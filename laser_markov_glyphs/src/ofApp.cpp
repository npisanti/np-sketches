
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::patch(){
    
    mc.load( ofToDataPath("mc.chain") );

    engine.score.init( 1, 1, 172.0f); // sections, sequences, tempo
    engine.score.sections[0].setCell( 0, &mc, pdsp::Behavior::Loop );
    engine.score.sections[0].launchCell(0);

    engine.score.sections[0].out_trig(0) >> zap.in_trig();
    engine.score.sections[0].out_value(1) >> zap.in_pitch();
    engine.score.sections[0].out_trig(2) >> noise.in_trig();
    engine.score.sections[0].out_trig(3) >> click.in_trig();
    engine.score.sections[0].out_value(4) >> click.in_pitch();
    zap >> engine.audio_out(0);
    zap >> engine.audio_out(1);        
    noise >> engine.audio_out(0);
    noise >> engine.audio_out(1);        
    click.out_L() >> engine.audio_out(0);
    click.out_R() >> engine.audio_out(1);       
    
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("markov chain sequencer");    
    patch();
        
    // -------------------GRAPHIC SETUP--------------
    ofBackground(0);
    
    sigil.resize(32);
    for( auto & vessel : sigil ){ vessel.reserve( 4 ); }
    
    graphics.setName( "graphics" );
    graphics.add( bGraphics.set("active", true) );
    graphics.add( padding.set("padding", 0.1f, 0.0f, 1.0f));
    graphics.add( gw.set("gw", 22, 8, 60));
    graphics.add( gh.set("gh", 1.5f, 1.0f, 2.0f));
    graphics.add( offx.set("offx", 50, 0, ofGetWidth()));
    graphics.add( offy.set("offy", 50, 0, ofGetHeight()));
    graphics.add( cp0x.set("cp 0 x", 0.5f, 0.0f, 1.0f));
    graphics.add( cp0y.set("cp 0 y", 0.5f, 0.0f, 1.0f));
    graphics.add( cp1x.set("cp 1 x", 0.5f, 0.0f, 1.0f));
    graphics.add( cp1y.set("cp 1 y", 0.5f, 0.0f, 1.0f));
    graphics.add( tilde.set("tilde", 0.25f, 0.0f, 1.0f));
    graphics.add( hlSpacing.set("hlspacing", 0.25f, 0.0f, 1.0f));
    graphics.add( color.set("color", ofColor(255, 0, 0), ofColor(0), ofColor(255)));

    // -------------------GUI------------------------
    
    gui.setup("panel", "settings.xml", ofGetWidth()-480, 300 );
    gui.add( graphics );
    gui.add( mc.parameters );
    gui.add( zap.parameters );
    gui.add( noise.label("noise") );
    gui.add( click.label("click") );
    gui.loadFromFile(ofToDataPath("settings.xml"));
    gui.minimizeAll();

    // ------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

    
    // ----------------- LASER -----------------------

    laserWidth = 800;
    laserHeight = 800;
    laser.setup(laserWidth, laserHeight);
    laser.addProjector(dac);
    dac.setup("169.254.103.84");
    laser.initGui( true );

    // --------------- variables ------------------

    for(size_t i=0; i<sigil.size(); ++i){
        float width = gw * (1.0f-padding);
        sigilcraft( i%6, sigil[i], offx + gw*i, offy, width );
    }
    
    laststep = -1;

    randoms.resize(64);
    xvalues.resize( randoms.size() );
}


//--------------------------------------------------------------
void ofApp::update(){

    int step = mc.getStep();
    int state = mc.getStates()[step];
    
    if( laststep != step  ){
        float width = gw * (1.0f-padding);
        sigilcraft( state, sigil[step], offx + gw*step, offy, width );
    }
    laststep = step;


    // prepares laser manager to receive new points
    laser.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
	ofNoFill();
	ofSetLineWidth(1);
	ofDrawRectangle(0,0,laserWidth, laserHeight);
	
        
    ofNoFill();
    ofSetColor(255, 85, 85);   



	// laser --------------------------
    
    if( bGraphics ){
        if( mc.getState() == 3){
            
            float xmin = offx;
            float xmax = offx + 32 * gw; 
            float h = gw * (1.0f-padding) * gh;

            for( size_t i=0; i<xvalues.size(); ++i ){
                xvalues[i] = ofRandom( xmin, xmax );
            }
            std::sort(xvalues.begin(), xvalues.end());
            
            for( size_t i=0; i<randoms.size(); ++i ){
                randoms[i].clear();
                float x = xvalues[i];
                if(i%2==0){
                    randoms[i].addVertex( x, offy);
                    randoms[i].addVertex( x, offy+h);
                }else{
                    randoms[i].addVertex( x, offy+h);
                    randoms[i].addVertex( x, offy);                
                }
            }

            for(size_t i=0; i<randoms.size(); ++i){
                auto & vertices = randoms[i].getVertices();
                ofBeginShape();
                for( size_t k=0; k<vertices.size(); ++k ){
                    ofVertex( vertices[k] );
                }
                ofEndShape(false);
            }            
            
            for(size_t i=0; i<randoms.size(); ++i){
                laser.drawPoly(randoms[i], color, OFXLASER_PROFILE_DETAIL );
            }
            
        }else{
     
            for ( auto & vessel : sigil ){
                for(size_t i=0; i<vessel.size(); ++i){
                    auto & vertices = vessel[i].getVertices();
                    ofBeginShape();
                    for( size_t k=0; k<vertices.size(); ++k ){
                        ofVertex( vertices[k] );
                    }
                    ofEndShape(false);
                }            
            }
            
            for ( auto & vessel : sigil ){
                for(size_t i=0; i<vessel.size(); ++i){
                    laser.drawPoly(vessel[i], color, OFXLASER_PROFILE_DETAIL );
                }
            }        
        }
                
    }

    laser.send();
    // --------------------------------

    laser.drawUI();
    
    //mc.draw( 20, ofGetHeight()-80);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::sigilcraft( int glyph, std::vector<ofPolyline> & vessel, float x, float y, float w ){
    
    vessel.clear();
    
    float h = w * gh;
    
        
    const auto & click = [&]( ofPolyline & polyline ){
        int kin = rand()%3;
        switch( kin ){
            case 0:
                polyline.addVertex(x,   y + h*0.5f);
                polyline.addVertex(x+w, y + h*0.5f);            
            break;
            
            case 1:
                polyline.curveTo(x,   y + h*0.5);
                
                polyline.curveTo(x,           y + h*0.5f);
                polyline.curveTo(x + w*.25f,  y + h*(0.5f-tilde));
                polyline.curveTo(x + w*.5f ,  y + h*0.5f);
                polyline.curveTo(x + w*.75f , y + h*(0.5f+tilde));
                polyline.curveTo(x + w,       y + h*0.5f);
                
                polyline.curveTo(x + w,       y + h*0.5f);
                       
            break;
            
            case 2:
                polyline.addVertex(x+w*0.3f,   y + h*0.5f);
                polyline.addVertex(x+w*0.7f,   y + h*0.5f);                 
            break;
        }

    };
    
    

    switch( glyph ){
        case 0: case 1: break;
        
        case 2:
        {
            ofPolyline dum;
            
            dum.addVertex(x, y+h);
            dum.addVertex(x, y);
            dum.addVertex(x+w, y);
            
            dum.bezierTo( x + w*cp0x*0.5f, y + h*cp0y*0.5f,
                          x + w*cp1x*0.5f, y + h*cp1y*0.5f, 
                          x+w, y + h*0.5f, 30 );
            
            vessel.push_back( dum );      
        }
        break;
        
        case 3:
        {
            for(int i=0; i<3; ++i){
                ofPolyline hline;
                float xx = ofMap( i, 0, 2, w*(0.5f-hlSpacing), w*(0.5f+hlSpacing) );
                hline.addVertex( x+xx, y);
                hline.addVertex( x+xx, y+h);
                vessel.push_back( hline );
            }   
        }
        break;
        
        case 4:
        {
            ofPolyline cl;
            click( cl );
            vessel.push_back( cl );  
        }
        break;
        
        case 5:
        {
            ofPolyline cl;
            click( cl );
            vessel.push_back( cl );
            
            ofPolyline curve;
            curve.addVertex(x+w, y + h*0.5);
            curve.bezierTo( x + w*cp0x*0.5f, y + h*(cp0y*0.5f + 0.5),
                            x + w*cp1x*0.5f, y + h*(cp1y*0.5f + 0.5), 
                            x+w, y + h, 30 );
            vessel.push_back( curve );
            
        }
        break;
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    laser.saveSettings();
    dac.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key==OF_KEY_TAB) {
        laser.nextProjector();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
