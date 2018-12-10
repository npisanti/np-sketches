//
//  ofxParticles.h
//
//  Created by Timothy Scaffidi on 6/14/12.
//  Modified Nicola Pisanti 18 Mar 2017
//

#pragma once
#include <list>

#include "ofMain.h"


class ofxParticle {
public:
    ofVec3f position;
    ofVec3f velocity; // pixels/sec
    ofVec3f acceleration; // pixels/sec/sec

    ofVec3f rotation;
    ofVec3f rotationalVelocity;

    float mass;
    float size;
    float life;
    float lifeStart;
    float dt;
    int particleID;


    bool operator < (const ofxParticle &b);

    ofxParticle();
    
    ofxParticle(ofVec3f pos, ofVec3f vel, float size_ = 1.0, float life_ = 1.0);

    ofxParticle(const ofxParticle &mom);
    ~ofxParticle();

    ofxParticle & operator=(const ofxParticle &mom);

    void constrainToRect(ofRectangle bounds, const float k, const float dist, const float drag);

    void applyForce(ofVec3f force);
    
    // void spring(ofPoint p, const float k, const float springDist, const float drag) { spring(p, k, springDist, drag); }
    float springBoth(ofxParticle * p, const float k, const float springDist, const float drag = 1.0f, const float springSnap = 1.0f);

    void attractTo(ofxParticle p, const float accel, const float minDist, const bool consumeParticle = false);
    void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle = false);

    void gravitateTo(ofxParticle p, const float gravity, const float minDist, const bool consumeParticle = false);
    void gravitateTo(ofPoint p, const float gravity, const float mass2, const float minDist, const bool consumeParticle = false);

    void gravitateBoth(ofxParticle * p, const float gravity, const float minDist, bool consumeParticle = false);
    
    void rotateAround(ofxParticle p, const float accel, const float minDist, const bool consumeParticle = false);
    void rotateAround(ofPoint p, const float accel, const float minDist, const bool consumeParticle = false);

    
    void update(const float timeStep, const float drag);
    
    void setVboLine( vector<ofVec2f> & points, const int & i, float trail=1.0f);
    
    bool isAlive();

};

class ofxParticleEmitter {
public:
    ofxParticleEmitter() : positionStart(), positionEnd(),posSpread(),velocityStart(),velocityEnd(),velSpread(),
    rotation(),rotSpread(),rotVel(),rotVelSpread(),size(1.0),sizeSpread(0.0),
    life(1.0),lifeSpread(0.0),numPars(1)
    {}
    ~ofxParticleEmitter() {}
    
    ofVec3f positionStart;
    ofVec3f positionEnd;
    ofVec3f posSpread;
    ofVec3f velocityStart;
    ofVec3f velocityEnd;
    ofVec3f velSpread;
    ofVec3f rotation;
    ofVec3f rotSpread;
    ofVec3f rotVel;
    ofVec3f rotVelSpread;
    float size;
    float sizeSpread;
    float life;
    float lifeSpread;
    int numPars;
    
    ofxParticleEmitter & setPosition(ofVec3f pos);
    ofxParticleEmitter & setPosition(ofVec3f posStart, ofVec3f posEnd);
    ofxParticleEmitter & setVelocity(ofVec3f vel);
    ofxParticleEmitter & setVelocity(ofVec3f velStart, ofVec3f velEnd);
    
};

class ofxParticleSystem {
public:
    ofxParticleSystem();
    ~ofxParticleSystem();

    void addParticles(ofxParticleEmitter & src);
    void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle = false);

    void gravitateTo(ofPoint p, const float gravity, const float mass, const float minDist, const float consumeParticle);

    void rotateAround(ofPoint p, const float accel, const float minDist, const float consumeParticle);

    void applyVectorField(float * field, int fieldWidth, int fieldHeight, int numComponents, ofRectangle areaOfInfluence, float force);
    
    int update(float timeStep, float drag);
    
    void draw( float trail );
    
    void draw();
    
    int getNumParticles();

    void binParticles(vector<ofxParticle *> particles, const int binPower, const ofRectangle bounds);

    int getNeighbors(vector<ofxParticle *> &neighbors, ofxParticle &p, const float radius);
    
    int getNeighbors(vector<ofxParticle *> &neighbors, float targetX, float targetY, const float radius);


private:
    list<ofxParticle*> particles;
    int numParticles;
    int totalParticlesEmitted;

    vector<vector<ofxParticle *>> bins;
    ofRectangle binBounds;
    int binPower;
    int xbins, ybins;

    // FAST RENDERING
    // a simple vector of points
    vector <ofVec2f> points;
    vector <ofVec2f> speeds;
    // in super fast mode we use a vbo
    ofVbo vbo;
};
