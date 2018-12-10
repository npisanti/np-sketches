
#pragma once

#include "ofMain.h"

namespace np {

class AsemicTextBlock {

public:    

    enum Direction { LeftToRight, RightToLeft, VerticalLeftToRight, VerticalRightToLeft };

	AsemicTextBlock();
	
    void setup( int w, int h, int gsize, int interline=4, int marginX=0, int marginY=0, int multisample=1, bool autoClearPage=false ); 
    
    void enter(); 
    void write ( int num ); 
    void whitespace ( int num=1 ); 
    void writeLine( int num ); 
    void draw( int x, int y ); 
    
    void clear(); // clears page

    int getLineWrap() const; 
    
    int getWidth() const;
    int getHeight() const;
    
    void setLineWidth( float width ){ linewidth = width; }

    bool cursorOnFirstSpaceOfLine() const;
    bool cursorOnFirstSpaceOfFirstLine() const;
    
    ofParameter<bool> bClearPage;

    void glyph( float gh ) {
        lastGlyph = glyph( gh, getGlyphWidth(gh), lastGlyph );
    };

    Direction direction;
    
protected:
    virtual int glyph( float gh, float gw, int previous )=0;
    virtual void setupGlyph( int gh )=0;
    virtual int getGlyphWidth( int gh )=0;

    
private:
    void advance();
    void clearLine();
    
    int interline;
    float mx;
    float my;
    
    float gh;
    float gw;
    
    float linewidth;
    
    int numLines;
    int lineWrap;
    int lineStep;
    
    ofFbo fbo;
    
    int cursor;
    int line;
    
    int lastGlyph;

};

};
