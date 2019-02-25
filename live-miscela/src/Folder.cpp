
#include "Constants.h"
#include "Folder.h"

void miscela::Folder::setup( std::string folder, int layers ){    
	files.listDir( folder );
	files.allowExt("frag");
	files.sort(); 
    
    indices.resize( layers );
    for( size_t i=0; i<indices.size(); ++i ){ 
        indices[i].set( "index "+ofToString(i), 0, 0, files.size()-1 );
    }
}
    
void miscela::Folder::next( int layer ){
    int i = indices[layer];
    i++;
    if( i >= int( files.size() ) ){
        i = 0;
    }
    indices[layer] = i;
}

void miscela::Folder::prev( int layer ){
    int i = indices[layer];
    i--;
    if( i < 0 ){
        i = files.size()-1;
    }
    indices[layer] = i;
}

std::string miscela::Folder::get( int layer ){
    return files.getPath(indices[layer]);
}
    
void miscela::Folder::indexToCoord( const size_t & index, glm::vec2 & coord ){
    int x = index / maxrows;
    int y = index % maxrows;
    coord.x = x * LABELW;
    coord.y = y * LABELH;
}
    
void miscela::Folder::resize( int w, int h ){
    
    maxrows = h / LABELH;
    glm::vec2 coord;
    
    fbo.allocate( w, h );
    fbo.begin();
        ofClear( 0, 0, 0, 0 );
        ofSetColor( 255 );
        for( size_t i=0; i<files.size(); ++i ){
            indexToCoord( i, coord );
            ofDrawBitmapString( files.getName(i), coord.x + TEXTOFF_X, coord.y + TEXTOFF_Y );
        }
    fbo.end();
    
}

void miscela::Folder::draw( int x, int y ){
    ofPushMatrix();
    ofPushStyle();
        ofTranslate( x, y );
        ofFill();
        
        glm::vec2 coord;
        
        ofSetColor( 60 );
        for( size_t i=0 ; i<indices.size(); ++i ){
            indexToCoord( indices[i], coord );
            ofDrawRectangle( coord.x, coord.y, LABELW, LABELH );
        }
        
        ofSetColor( 255 );        
        for( size_t i=0 ; i<indices.size(); ++i ){
            std::string id;
            switch( i ){
                case 0: id = "(A0)"; break;
                case 1: id = "(A1)"; break;
                case 2: id = "(B0)"; break;
                case 3: id = "(B1)"; break;
                default: id = "("+ofToString(i)+")"; break;
            }
            
            indexToCoord( indices[i], coord );
            int off = 0;
            for( size_t k=i; k<indices.size(); ++k ){
                if( indices[k] == indices[i] ){ off -= 31; }
            }
            ofDrawBitmapString(id, coord.x+LABELW+off, coord.y + 14 );
        }
        fbo.draw( 0, 0 );
        
        ofSetColor( 60 );
        ofNoFill();
        ofDrawRectangle( 0, 0, fbo.getWidth(), fbo.getHeight() );
    ofPopStyle();
    ofPopMatrix();
}
