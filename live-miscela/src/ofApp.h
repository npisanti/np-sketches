#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//#include "ofxLua.h"
#include "ofxDotFrag.h"
#include "ofxLaunchControllers.h"
#include "np-extra.h"

#include "Channel.h"
#include "Folder.h"
#include "Effects.h"

#include "components/Analyzer.h"

// this should match the projector resolution
#define PROJECTOR_WIDTH 720
#define PROJECTOR_HEIGHT 480

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void drawSecondWindow(ofEventArgs& args);
    
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

        void xyControl( float x, float y, int button );
        template <int la> void onPrevButton( bool & value );
        template <int la> void onNextButton( bool & value );
        template <int la> void onIndexChange( int & value );
        
        ofFbo 		fbo;
        
        bool		bDrawGui;
        
		ofxPanel 	gui;
        
        ofParameterGroup controls;
        ofParameter<bool> drawBorder;
        ofParameter<float> mix;
        ofParameter<float> master;

        std::vector<np::miscela::Channel> channels;
        np::PaletteTable palette;
        
        ofParameter<bool> invertPalette;
        
        miscela::Folder folder;
        miscela::Effects effects;
        void next( int i );
        void prev( int i );
        
        
        ofRectangle fxRect;
        ofRectangle filesRect;
        ofRectangle fboRect;
        ofRectangle guiPos;
        ofRectangle channelRect;
        ofParameter<int> channelOffX;
        ofParameter<int> channelOffY;
        
        bool full;
        std::string cmdpath;
        
        ofParameter<bool> useCamTexture;
        void onUseCamTexture( bool & value );
        
        void openCam( int index );
        ofImage logo;
        ofVideoGrabber cam;        
        
        np::Analyzer analyzer;
        ofParameterGroup audioMap;
        ofParameter<float> lowThreshold;
        ofParameter<float> highThreshold;
        ofParameter<float> audioMeter;
        ofParameter<float> modMeter;
        float mod;
        
        ofxLaunchControl lc;
        
        glm::vec3 position;
        
        bool bResize;
        
        ofParameter<float> speed;
};
