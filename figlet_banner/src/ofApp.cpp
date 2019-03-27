#include "ofApp.h"

int counter;
int mode = 3;

//--------------------------------------------------------------
void ofApp::setup(){
    
    fontSize = 9;
    font.load("terminus.ttf", 9, false, true );
    
    counter = 0;
    
    ofSetWindowTitle("   ");
    
    // To generate text file:
    // toilet -t -f slant "sample text" > sample.txt

    ofSetFrameRate( 15 );
    
    textIndex = 0;
    baseTexts.resize(7);
    ofBuffer buffer = ofBufferFromFile("502.txt");
    baseTexts[0] = buffer.getText();
    buffer = ofBufferFromFile("bad_gateway.txt");
    baseTexts[1] = buffer.getText();
    buffer = ofBufferFromFile("music.txt");
    baseTexts[2] = buffer.getText();
    buffer = ofBufferFromFile("mpia3.txt");
    baseTexts[3] = buffer.getText();
    buffer = ofBufferFromFile("mboys.txt");
    baseTexts[4] = buffer.getText();
    buffer = ofBufferFromFile("paula.txt");
    baseTexts[5] = buffer.getText();    
    buffer = ofBufferFromFile("perc.txt");
    baseTexts[6] = buffer.getText();

}

//--------------------------------------------------------------
void ofApp::update(){

    mode = rand()%16;

    if( mode<6 ) splitText = ofSplitString( baseTexts[textIndex], "\n" );

    switch( mode ) {
        
        case 0: // moving bar
            finalText = "";
            for ( size_t i=0; i<splitText.size(); ++i ) {
                if (counter == i) {
                    for(size_t k=0; k<splitText[i].size(); k++) finalText += "X";
                } else {
                    finalText += splitText[i];
                }
                finalText += "\n";
            }
            
            counter ++;
            if(counter == (int)splitText.size()) counter = 0;
        break;
        
        case 1: // random vert bars
        
            finalText = "";
            for ( size_t i=0; i<splitText.size(); ++i ) {
                finalText += splitText[i];
                finalText += "\n";
            }
            for ( int i=0; i<48; ++i ) {
                int pos = rand()%finalText.size();
                if( finalText[pos]!='\n' ){
                    finalText[pos] = '|';
                }
            }
        break;
        
        case 2: case 3: case 4: // offset glitch
        {
            finalText = "";
            
            int glitch = rand()%splitText.size();
            for ( size_t i=0; i < splitText.size(); ++i ) {
                if ( i == glitch) {
                    int off = rand()%8 + 2;
                    for( int k = 0; k<off; ++k ) finalText += " ";
                } 
                finalText += splitText[i];
                finalText += "\n";
            }
            
            counter ++;
            if(counter == (int)splitText.size()) counter = 0;
        }
        break;
        
        case 5: // noise stains
        {
            float time = ofGetElapsedTimef();
            float freq = 25.0f;
            for ( size_t y=0; y<splitText.size(); ++y ) {
                for( int x = 0; x < splitText[y].size(); ++x ) {
                    float noise = ofNoise(   static_cast<float>(x) / freq, 
                                             static_cast<float>(y) / freq, time );
                    
                    if( noise>0.9f ) {
                        splitText[y][x] = '.';
                    }else if( noise > 0.8f ){
                        splitText[y][x] = ':';
                    }
                }
            }
            
            finalText = "";
            for ( size_t i=0; i<splitText.size(); ++i ) {
                finalText += splitText[i];
                finalText += "\n";
            }
        }
        break;
        
        default:
            finalText = baseTexts[textIndex];
        break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int flash = rand()%26;
    
    switch( flash ) {
        case 0: 
            ofBackground( 255, 0, 0 );
            ofSetColor( 255 );
            counter = 0;
        break;
        
        case 1: case 2: case 3:
            ofBackground(0);
            ofSetColor( 255 );
            counter = 0;            
        break;
        
        default:
            ofBackground(0);
            ofSetColor( 255 );  
        break;
    }
    
    ofRectangle b = font.getStringBoundingBox( baseTexts[textIndex], 0, 0 ); 
    int offX = ( ofGetWidth() - b.width)  * 0.5f;
    int offY = (ofGetHeight() - b.height) * 0.5f;

    font.drawString( finalText, offX, offY + fontSize );
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch( key ){
        case '1' : textIndex = 0; break;
        case '2' : textIndex = 1; break;
        case '3' : textIndex = 2; break;
        case '4' : textIndex = 3; break;
        case '5' : textIndex = 4; break;
        case '6' : textIndex = 5; break;
        case '7' : textIndex = 6; break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
