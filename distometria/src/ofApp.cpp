#include "ofApp.h"

// sketch init to make graphics of jitter square figures or horiz/vert lines 
// no antialiasing
// no curve or oblique lines

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableAntiAliasing();

    gui.setup( "gui", "settings.xml", ofGetWidth()-220, 20 );
    gui.add( rInner.set("inner radius", 180, 20, ofGetWidth()*0.5f));
    gui.add( rOuter.set("outer radius", 250, 20, ofGetWidth()*0.5f));
    gui.add( bDrawGuides.set("draw guides", false) );
    
    gui.add( dPoints.set("d points", 32, 2, 64) );
    gui.add( dNum.set("d num", 4, 1, 10) );
    
    gui.add( bFade.set("fade", false));
    gui.add( fade.set("fade value", 20, 1, 45) );
    
    gui.add( color.set("color", ofColor(255), ofColor(0), ofColor(255) ) );
    ofBackground( 0 );
    
    int pw = ofGetWidth();
    int ph = ofGetHeight();
    fbo.allocate( pw, ph );
    fbo.begin();
        ofClear(0, 0, 0, 0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // -------------- guides --------------
    inner.clear();
    outer.clear();
    
    float step;
    float theta;
    
    float cx = fbo.getWidth() * 0.5f;
    float cy = fbo.getHeight() * 0.5f;
    float tStart = fmod( ofGetElapsedTimef() * 0.1, TWO_PI );
    
    int vInner = 4;
    step = TWO_PI / vInner;
    theta = tStart;
    for( int i=0; i<vInner; ++i ){
        inner.addVertex( cx + cos(theta)*rInner, cy + sin(theta)*rInner );
        theta+=step;
    }
    inner.close();

    int vOuter = vInner;
    step = TWO_PI / vOuter;
    theta = tStart;
    for( int i=0; i<vOuter; ++i ){
        outer.addVertex( cx + cos(theta)*rOuter, cy + sin(theta)*rOuter );
        theta+=step;
    }
    outer.close();
    


    // -------------- distometries -------------- 
    fbo.begin();
        if(bFade){
            ofFill();
            ofSetColor(0, 0, 0, fade);
            ofDrawRectangle( 0, 0, fbo.getWidth(), fbo.getHeight() );
        }else{
            ofClear(0, 0, 0, 0);
        }
       
        ofSetColor( 255 );
        ofNoFill();
        
        for( int i=0; i<dPoints; ++i ){
            float pct = static_cast<float>(i) / static_cast<float>(dPoints);
            const auto & p0 = inner.getPointAtPercent( pct );
            const auto & p1 = outer.getPointAtPercent( pct );
            
            for( int n=0; n<dNum; ++n ){
                //float x0 = ofRandom( p0.x, p1.x );
                //float y0 = ofRandom( p0.y, p1.y );            
                float x0 = p0.x;
                float y0 = p0.y;        

                float x1 = ofRandom( p0.x, p1.x );
                float y1 = ofRandom( p0.y, p1.y );        

                //float w = x1 - x0;
                //float h = y1 - y0;
                //ofDrawRectangle( x0, y0, w, h );

                if(n%2==0){ // also try i%2==0
                    ofDrawLine( x0, y0, x0, y1);
                    ofDrawLine( x0, y1, x1, y1);
                }else{
                    ofDrawLine( x0, y0, x1, y0);
                    ofDrawLine( x1, y0, x1, y1);
                }
            }
        }        
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if( bDrawGuides ){
        ofSetColor( 255 );
        draw_polyline( inner );
        draw_polyline( outer );        
    }
    
    ofSetColor( color );
    fbo.draw( 0, 0 );
    
    gui.draw();
}


//--------------------------------------------------------------
void ofApp::draw_polyline( const ofPolyline & polyline ){
        auto & vertices = polyline.getVertices();    
        ofBeginShape();
        for( size_t i=0; i<vertices.size(); ++i ){
            ofVertex( vertices[i] );
        }
        ofEndShape( true );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
