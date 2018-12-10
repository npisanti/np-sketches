
#include "AsemicTextBlock.h"

np::AsemicTextBlock::AsemicTextBlock() { 
    direction = LeftToRight; 
    linewidth = 1.0f;
};

void np::AsemicTextBlock::setup( int w, int h, int gsize, int interline, int marginX, int marginY, int multisample, bool autoClearPage ) {
    
    this->interline = interline;
    
    setupGlyph( gsize );
    gh = gsize;
    gw = getGlyphWidth( gh );

    switch( direction ){
        case LeftToRight: case RightToLeft:
            lineWrap = (w-(marginX*2)) / gw;
            lineWrap--;
            mx = ( w - lineWrap*gw - marginX*2 ) * 0.5f;
            mx += marginX;
            
            lineStep = gh+interline;
            numLines = (h-(marginY*2)) / lineStep;    
            my = ( h - numLines*lineStep - marginY*2 )*0.5f;
            my += marginY;        
        break;
        
        case VerticalLeftToRight: case VerticalRightToLeft:
            lineStep = gw+interline;
            numLines = (w-(marginY*2)) / lineStep;    
            mx = (w - numLines*lineStep - marginX*2)*0.5f;
            mx += marginY;      

            lineWrap = (h-(marginY*2)) / gh;
            my = (h - lineWrap*gh - marginY*2)*0.5f;
            my += marginY;
        break;
        
    }

    fbo.allocate( w, h, GL_RGBA, multisample );
    fbo.begin();
        ofClear( 0, 0, 0, 255 );
    fbo.end();
    
    cursor = 0;
    line = 0;
    lastGlyph = -1;
    
    bClearPage = autoClearPage;
    
}

void np::AsemicTextBlock::write ( int num ) {
    ofPushStyle();
    ofFill();
    ofSetColor(255);
    ofSetLineWidth( linewidth );
    
    for (int i=0; i<num; ++i){
        fbo.begin();        
            ofPushMatrix();
                switch( direction ){
                    case LeftToRight:
                        ofTranslate( mx + gw*cursor, my + lineStep*line );
                    break;
                    
                    case RightToLeft:
                        ofTranslate( mx + gw*(lineWrap-1-cursor), my + lineStep*line );
                    break;
                    
                    case VerticalLeftToRight:
                        ofTranslate( mx + lineStep*line, my + gh*cursor  );
                    break;
                    
                    case VerticalRightToLeft:
                        ofTranslate( mx + lineStep*(numLines-1-line), my + gh*cursor  );
                    break;
                }
            
                lastGlyph = glyph( gw-1, gh, lastGlyph );
            ofPopMatrix();
        fbo.end();      

        advance(); // advance also advances glyph
    }
    
    ofPopStyle();

}

void np::AsemicTextBlock::whitespace ( int num ) { 
    for (int i=0; i<num; ++i){
        advance(); // advance also advances glyph
    }
}

void np::AsemicTextBlock::advance() {
    cursor++;
    if(cursor==lineWrap){
        enter();
    }
}

void np::AsemicTextBlock::enter() {
    line++;
    if(line >= numLines){
        if(bClearPage) clear();
        line = 0;
    }
    cursor = 0;
    lastGlyph = -1;
    
    if( ! bClearPage ){
        fbo.begin();
            ofFill();
            ofSetColor(0); 
            ofPushMatrix();
                switch( direction ){
                case LeftToRight:
                case RightToLeft:
                    ofTranslate( mx, my + lineStep*line );
                    ofDrawRectangle( -1, -interline*0.5, gw*lineWrap+2, gh+interline );
                break;

                case VerticalLeftToRight:
                    ofTranslate( mx + lineStep*line, my  );
                    ofDrawRectangle( -interline*0.5, -1, gw+interline, gh*lineWrap+2 );
                break;

                case VerticalRightToLeft:
                    ofTranslate( mx + lineStep*(numLines-1-line), my  );
                    ofDrawRectangle( -interline*0.5, -1, gw+interline, gh*lineWrap+2 );
                break;
                }
            ofPopMatrix();
        fbo.end();        
    } 

}

void np::AsemicTextBlock::writeLine( int num ) {
    write(num);
    enter();
}

void np::AsemicTextBlock::draw( int x, int y ) {
    fbo.draw( x, y );
}

int np::AsemicTextBlock::getLineWrap() const { return lineWrap; }

    
int np::AsemicTextBlock::getWidth() const {
    return fbo.getWidth();
}

int np::AsemicTextBlock::getHeight() const {
    return fbo.getHeight();
}
    
void np::AsemicTextBlock::clear() {
    fbo.begin();
        ofClear( 0, 0, 0, 255 );
    fbo.end();
    
    cursor = 0;
    line = 0;
    lastGlyph = -1;
}


bool np::AsemicTextBlock::cursorOnFirstSpaceOfLine() const {
    return (cursor==0);
}

bool np::AsemicTextBlock::cursorOnFirstSpaceOfFirstLine() const {
    return (cursor==0 && line==0);
}
