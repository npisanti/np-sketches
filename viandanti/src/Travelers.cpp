
#include "Travelers.h"


np::Travelers::Travelers(){
    parameters.setName("travelers");
    parameters.add( dimension.set("dimension", 26, 3, 64) );
    parameters.add( segments.set("segments", 24, 8, 64) );
    parameters.add( spacing.set("spacing", 1, 1, 6) );
    parameters.add( angle.set("angle", 0.9f, 0.0f, 1.0f) );
    parameters.add( anglemod.set("angle mod", 0.0f, 0.0f, 0.01f) );
    
    travelers.resize(1);
    entered.resize(1);
    
    for(size_t i=0; i<entered.size(); ++i){
        entered[i] = 0;
    }
}

ofParameterGroup & np::Travelers::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

void np::Travelers::update(){
    for( size_t i=0; i<travelers.size(); ++i ){
        if( entered[i] ){
            while( static_cast<int>(travelers[i].getVertices().size()) > segments*spacing ){
                travelers[i].getVertices().erase( travelers[i].getVertices().begin() );
            }      
        }else{
            if( travelers[i].getVertices().size() > 0 ){
                travelers[i].getVertices().erase( travelers[i].getVertices().begin() );
            }  
        }
    }
}

void np::Travelers::draw(){
    
    float rmult =  static_cast<float>(dimension) / (float) (spacing * segments);
    
    for( size_t i=0; i<travelers.size(); ++i ){
        
        for( int k=travelers[i].size()-1; k>1; k -= spacing ){
                        
            float pct1 = static_cast<float>(k) / static_cast<float>(travelers[i].size()-1);
            float pct0 = static_cast<float>(k-1) / static_cast<float>(travelers[i].size()-1);
            
            float r = rmult*k;
            float radians =  PI*( angle - anglemod*k );

            glm::vec2 p1 = travelers[i].getPointAtPercent(pct1);
            glm::vec2 p0 = travelers[i].getPointAtPercent(pct0);

            glm::vec2 spine = glm::vec2( p1.x - p0.x, p1.y - p0.y );

            glm::vec2 bone0 = spine;
            bone0 = glm::rotate( bone0, radians );
            bone0 = glm::normalize( bone0 );
            bone0 = bone0*r + p1;

            glm::vec2 bone2 = spine;
            bone2 = glm::rotate( bone2, - radians );
            bone2 = glm::normalize( bone2 );
            bone2 = bone2*r + p1;            

            ofDrawLine( p1, bone0 );
            ofDrawLine( p1, bone2 );
        }            
    }
    
}

void np::Travelers::resize( int size ){
    int oldsize = travelers.size();
    travelers.resize(size);
    entered.resize(size);
    
    for(size_t i=oldsize; i<entered.size(); ++i){
        entered[i] = 0;
    }
}

void np::Travelers::move( float x, float y, int i ){
    if( entered[i] ) travelers[i].addVertex( x, y );
}

void np::Travelers::enter( int i ){
    entered[i] = 1;
}

void np::Travelers::exit( int i ){
    entered[i] = 0;
}
