
#include "ofApp.h"
#include "Constants.h"

#define FILECOLS 3

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetCircleResolution( 90 );
    ofEnableAntiAliasing();
    ofSetLineWidth( 2.0f );
    
    position = glm::vec3( 0.5, 0.5, 0.0 );
    mod = 0.0f;
    
    int pw = PROJECTOR_WIDTH;
    int ph = PROJECTOR_HEIGHT;
    
    logo.setUseTexture( true );
    logo.load( ofToDataPath("logo.png") );
    logo.resize( pw, ph ); 

    bResize = false;
    fbo.allocate( pw, ph, GL_RGBA, OVERSAMPLING );
    
    int numPalettes = 2;
    palette.setup( numPalettes, 2 );
    channels.resize(2);
    for( auto & channel : channels ){
        channel.setup( palette, pw, ph );
    }
    
    effects.setup( fbo );

	bDrawGui = full;
	gui.setName("GUI");
	gui.setup("miscela", "settings.xml" );
    
    gui.add( master.set("master fader", 1.0f, 0.0f, 1.0f) );
    gui.add( speed.set( "speed control", 1.0f, 0.0f, 2.0f) );
    gui.add( palette.parameters );
    controls.setName("controls");
        controls.add( mix.set("mix", 0.0f, 0.0f, 1.0f) );
        controls.add( invertPalette.set("invert palette", false) );
        controls.add( drawBorder.set("draw border", false) );
        controls.add( useCamTexture.set("use cam texture", false) );

    // -------------- positions -------------------------
    
    float padding = 10;
    fboRect.width = fbo.getWidth();
    fboRect.height = fbo.getHeight();
    fxRect.width = LABELW;
    fxRect.height = LABELH * effects.effects.size();
    filesRect.width = FILECOLS * LABELW;
    filesRect.height = LABELH * padding;
    
    channelRect.width = fboRect.width;
    channelRect.height = fboRect.height;
    
    fboRect.x = padding;
    fboRect.y = padding;
    
    filesRect.alignTo( fboRect, OF_ALIGN_HORZ_LEFT, OF_ALIGN_VERT_TOP );
    filesRect.y += padding + fboRect.height;
    
    fxRect.alignTo( filesRect, OF_ALIGN_HORZ_LEFT, OF_ALIGN_VERT_TOP );
    fxRect.x += filesRect.width + padding;
    
    guiPos.alignTo( fboRect, OF_ALIGN_HORZ_LEFT, OF_ALIGN_VERT_TOP );
    guiPos.x += padding + fboRect.width;
    
    gui.setPosition( guiPos.x, guiPos.y );
    
    ofSetWindowShape(   padding*4 + filesRect.width + fxRect.width + 200 , 
                        padding*3 + fboRect.height + filesRect.height );
    
    if( !full ){ gui.setPosition( 20, 20 ); }
    
    // ---------------------------------------------
    controls.add( channelOffX.set("channel off x", (fboRect.width-channelRect.width)/2, 0, fboRect.width ) );
    controls.add( channelOffY.set("channel off y", (fboRect.height-channelRect.height)/2, 0, fboRect.height ) );
   
    if(!full){
        channels[0].sketch.load( cmdpath );
        ofSetWindowTitle( "miscela : " + ofFilePath::getFileName( cmdpath ) );
        ofSetWindowShape( fbo.getWidth(), fbo.getHeight() );
    } else {
        ofSetWindowTitle( "miscela" );
    }
    
    channels[0].parameters.setName( "channel 0" );
    channels[1].parameters.setName( "channel 1" );
    
    folder.setup( "/home/nicola/oF/addons/np-miscela/sketches", 4 );
    folder.resize( filesRect.width, filesRect.height );

    for( size_t i=0; i<channels.size(); ++i ){
        gui.add( channels[i].parameters );
        controls.add( folder.indices[i] );
    }

    gui.add( effects.parameters );
        
    gui.add( controls );

    // ------------------- audio modulations ----------------
    analyzer.band( "audio in", 0 );
    analyzer.setup( 2 ); // audio device id

    audioMap.setName("audio mod");
    
    audioMap.add( audioMeter.set("audio in", 0.0f, 0.0f, 1.0f) );
    audioMap.add( lowThreshold.set("low threshold", 0.0f, 0.0f, 1.0f) );
    audioMap.add( highThreshold.set("high threshold", 1.0f, 0.0f, 1.0f) );
    audioMap.add( modMeter.set("mod level", 0.0f, 0.0f, 1.0f) );
    gui.add( audioMap );
    gui.add( analyzer.parameters );
    // -------------------------------------------------------
  
	gui.minimizeAll(); 

    useCamTexture.addListener( this, &ofApp::onUseCamTexture );
    
    if( full ){
        folder.indices[0] = 0;
        folder.indices[1] = 0;        
    }else{
        channels[0].fbo.allocate( ofGetWidth(), ofGetHeight() );
    }
    
    // ---------- launch control bindings ----------------------
    
    lc.setup();
    
    lc.knob( 0,     channels[0].sketchMod );
    lc.knob( 0 + 8, channels[0].sketchControl );
    lc.knob( 1,     channels[0].overlayMod );
    lc.knob( 1 + 8, channels[0].overlayControl );
    
    lc.knob( 2, channels[0].sketch.frag.speed );
    lc.knob( 2, channels[1].sketch.frag.speed );
    lc.knob( 2 + 8 , mix );
    lc.toggle( 2, useCamTexture );

    lc.knob( 3,     channels[1].sketchMod );
    lc.knob( 3 + 8, channels[1].sketchControl );
    lc.knob( 4,     channels[1].overlayMod );
    lc.knob( 4 + 8, channels[1].overlayControl );
    
    lc.knob( 5,     effects.modA );
    lc.knob( 5 + 8, effects.controlA );
    lc.knob( 6,     effects.modB );
    lc.knob( 6 + 8, effects.controlB );
    lc.momentary( 5, effects.nextButton );
    lc.toggle( 6, effects.active );
    
    lc.knob( 7, master );
    lc.knob( 7+8, palette.cursor );
    lc.toggle( 7, invertPalette );

}

//--------------------------------------------------------------
void ofApp::update(){
    
    if( bResize ){
        channels[0].fbo.allocate( ofGetWidth(), ofGetHeight() );
        channels[0].sketch.resize( ofGetWidth(), ofGetHeight() );
        bResize = false;
    }
    
    if( cam.isInitialized() ){ cam.update(); }

    mod = ofMap(analyzer.meter(), lowThreshold, highThreshold, 0.0f, 1.0f, true );
    
    for( auto & channel : channels ){ 
        if( channel.sketch.requiresClear() ){
            channel.fbo.begin();
                ofClear( 0, 0, 0, 0 );
            channel.fbo.end();  
        }
        
        if(channel.sketch.isFragment()){
            if( useCamTexture ){
                channel.fbo.begin();
                    ofSetColor( 255 );
                    cam.draw( channel.fbo.getWidth(), 0, -channel.fbo.getWidth(), channel.fbo.getHeight() );
                channel.fbo.end();        
            }else{
                channel.fbo.begin();
                    ofSetColor( 255 );
                    ofRectangle r ( 0, 0, logo.getWidth(), logo.getHeight() );
                    ofRectangle fbr ( 0, 0, channel.fbo.getWidth(), channel.fbo.getHeight() );
                    r.alignTo( fbr );
                    logo.draw( fbr.x, fbr.y, fbr.width, fbr.height );
                channel.fbo.end();
            }
        }
    }
    
    int ca = invertPalette ? 1 : 0; 
    int cb = invertPalette ? 0 : 1;
    
    for( auto & channel : channels ){ 
        channel.update( position, mod, speed, ca, cb ); 
    }

    /*
    if( full ){
        fbo.begin();
            ofClear(0, 0, 0, 0);
            ofSetColor( 255, (1.0f-mix) * 255 );
            channels[0].fbo.draw( channelOffX, channelOffY );
            ofSetColor( 255,  mix * 255 );
            channels[1].fbo.draw( channelOffX, channelOffY );
            if(drawBorder){
                ofNoFill();
                ofSetColor( palette.color(ca) );
                ofDrawRectangle(  channelOffX+1, channelOffY+1, channelRect.width-2, channelRect.height -2 );
            }
        fbo.end();
        
        effects.process( mod );
    }
    */
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    if( full ){
  
        ofPushMatrix();
        ofTranslate(fboRect.x, fboRect.y );
        //ofClear(0, 0, 0, 0);
        ofSetColor( 255, (1.0f-mix) * 255 );
        channels[0].fbo.draw( channelOffX, channelOffY );
        ofSetColor( 255,  mix * 255 );
        channels[1].fbo.draw( channelOffX, channelOffY );
        if(drawBorder){
            ofNoFill();
            ofSetColor( 255 );
            ofDrawRectangle(  channelOffX+1, channelOffY+1, channelRect.width-2, channelRect.height -2 );
        }  
        ofPopMatrix();
  
        //ofSetColor( 255, master*255.0f );
        //fbo.draw( fboRect.x, fboRect.y);
        
        ofSetColor( 255 );
        
        folder.draw( filesRect.x,  filesRect.y );
        effects.draw( fxRect.x, fxRect.y );
    
    }else{
        ofSetColor( 255 );
        channels[0].fbo.draw( 0, 0 );     
    }
    
    if( bDrawGui ){ 
        audioMeter = analyzer.meter();
        modMeter = mod;
        gui.draw(); 
        ofDrawBitmapString( "fps = " + ofToString(ofGetFrameRate()), ofGetWidth()-120, ofGetHeight()-20 );
    }      
}

void ofApp::drawSecondWindow(ofEventArgs& args){
    ofBackground(0);
    //ofSetColor( 255, master*255.0f );
    //fbo.draw(0, 0);

    ofSetColor( 255, (1.0f-mix) * 255 );
    channels[0].fbo.draw( channelOffX, channelOffY );
    ofSetColor( 255,  mix * 255 );
    channels[1].fbo.draw( channelOffX, channelOffY );
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------

void ofApp::openCam( int index ){
    cam.setDeviceID( index );
    cam.setDesiredFrameRate(60);
    switch( index ){
        default: cam.initGrabber(640, 480); break;
    }
}
		
void ofApp::onUseCamTexture( bool & value ){
    if( value && !cam.isInitialized() ){
        useCamTexture = false;
    }
}    

void ofApp::keyPressed(int key){
	switch(key){
		case 'g': bDrawGui = !bDrawGui; break;
        //case '0': openCam( 0 ); break;
        //case '1': openCam( 1 ); break;
        //case '2': openCam( 2 ); break;
        case 'c': useCamTexture = !useCamTexture; break;
        //case 'a': channels[0].fbo.allocate( ofGetWidth(), ofGetHeight() ); break;
        case 's': gui.saveToFile( "settings.xml" ); break;
        case 'l': gui.loadFromFile( "settings.xml" ); break;
        
        case ' ':  // reseed
            //for( auto & channel : channels ){ channel.reseed(); }  
        break;
    }
    
    if( full ){
        switch(key){
            case '1': 
                folder.prev(0);
                channels[0].sketch.load( folder.get(0) );
            break;
            
            case 'q': 
                folder.next(0);
                channels[0].sketch.load( folder.get(0) );
            break;    
            
            case '2': 
                folder.prev(1);
                channels[0].overlay.load( folder.get(1) );
            break;
            
            case 'w': 
                folder.next(1);
                channels[0].overlay.load( folder.get(1) );
            break;    
            
            case '3': 
                folder.prev(2);
                channels[1].sketch.load( folder.get(2) );
            break;
            
            case 'e': 
                folder.next(2);
                channels[1].sketch.load( folder.get(2) );
            break;    
            
            case '4': 
                folder.prev(3);
                channels[1].overlay.load( folder.get(3) );
            break;
            
            case 'r': 
                folder.next(3);
                channels[1].overlay.load( folder.get(3) );
            break;    
        }        
    }else{
        if( key == 's' ){
            ofPixels pixels;
            channels[0].fbo.readToPixels( pixels );
            ofSaveImage(pixels, "frame"+ofToString(ofGetFrameNum())+".png" );
        }
    }

}

void ofApp::windowResized(int w, int h){
    if(!full){ bResize = true; } 
}

void ofApp::xyControl( float x, float y, int button ){
    switch( button ){
        default:
            position.x = x / float( ofGetWidth() );
            position.y = y / float( ofGetHeight() );
            position.z = float( ofGetMousePressed() );        
        break;
        
        case 2:
            float control = x / ofGetWidth();
            if( control < 0.0f ){ control = 0.0f; }
            if( control > 1.0f ){ control = 1.0f; }
            channels[0].sketchControl = control;
        break;
    }
}

void ofApp::mouseDragged(int x, int y, int button){
    if(!full){ xyControl( x, y, button ); }
}
void ofApp::mousePressed(int x, int y, int button){
    if(!full){ xyControl( x, y, button ); }
}
void ofApp::mouseReleased(int x, int y, int button){
    if(!full){ xyControl( x, y, button ); }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ }
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}

void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
