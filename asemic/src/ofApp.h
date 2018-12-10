#pragma once

#include "ofMain.h"

#include "asemic/AsemicTextBlock.h"
#include "asemic/AsemicCircles.h"
#include "asemic/AsemicN4.h"
#include "asemic/Asemic2B.h"
#include "asemic/AsemicRunic.h"

class AsemicSquares : public np::AsemicTextBlock {

public:    
    AsemicSquares(){ direction = VerticalLeftToRight; };

private:    
    void setupGlyph( int gh ) override { }
    int getGlyphWidth( int gh ) override { return gh; }
        
    int glyph( float gw, float gh, int previous ) override {
        if( ofRandomuf() < 0.5f ){
            ofNoFill();
            ofDrawRectangle( 1, 1, gw-1, gh-1 );
        }else{
            ofFill();
            ofDrawRectangle( 0, 0, gw, gh );
        }
        return 0;
    }
  
};


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        
        AsemicSquares squares;
        np::AsemicCircles circles;
        np::AsemicN4 n4;
        np::Asemic2B tb;
        np::AsemicRunic runic;
        
        np::AsemicTextBlock* asemic;
        
        

        bool bAutomatic;
        bool bSpace;
        int division;

};
