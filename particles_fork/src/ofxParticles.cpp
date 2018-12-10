
#include "ofxParticles.h"


inline ofVec3f ofRandVec3f() {
    return ofVec3f(ofRandomf(),ofRandomf(),ofRandomf()).normalize().scale(ofRandomf());
}

inline float fastSqrt(float x) {
    float xhalf = 0.5f * x;
    int i = *(int*)&x;            // store floating-point bits in integer
    i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
    x = *(float*)&i;              // convert new bits into float
    x = x*(1.5f - xhalf*x*x);     // One round of Newton's method
    return 1.0f / x;
}


bool ofxParticle::operator < (const ofxParticle &b) {
    return position.z < b.position.z;
}

ofxParticle::ofxParticle() {
    position = velocity = acceleration = ofVec3f(0,0,0);
    mass = 1.0f;
    size = 1.0f;
    lifeStart = life = 1.0f;
    particleID = 0;
    dt = 1.0/60;
}

ofxParticle::ofxParticle(ofVec3f pos, ofVec3f vel, float size_, float life_) {
    position = pos;
    velocity = vel;
    acceleration = ofVec3f(0,0,0);
    mass = 1.0f;
    size = size_;
    lifeStart = life = life_;
    particleID = 0;
    dt = 1.0f / 60.0f;
}

ofxParticle::ofxParticle(const ofxParticle &mom) {
    position = mom.position;
    velocity = mom.velocity;
    acceleration = mom.acceleration;
    rotation = mom.rotation;
    rotationalVelocity = mom.rotationalVelocity;
    mass = mom.mass;
    size = mom.size;
    life = mom.life;
    lifeStart = mom.lifeStart;
    particleID = mom.particleID;
    dt = 1.0f / 60.0f;
}

ofxParticle::~ofxParticle() {}

ofxParticle & ofxParticle::operator=(const ofxParticle &mom) {
    if(&mom==this) return *this;
    position = mom.position;
    velocity = mom.velocity;
    acceleration = mom.acceleration;
    rotation = mom.rotation;
    rotationalVelocity = mom.rotationalVelocity;
    mass = mom.mass;
    size = mom.size;
    life = mom.life;
    lifeStart = mom.lifeStart;
    particleID = mom.particleID;
    dt = 1.0f / 60.0f;
    return *this;
}

void ofxParticle::constrainToRect(ofRectangle bounds, const float k, const float dist, const float drag)
{
    ofPoint minPoint = bounds.getTopLeft();
    ofPoint maxPoint = bounds.getBottomRight();
    float inverse_drag = 1.0f / drag;
    float inverse_mass = 1.0f / mass;
    float spring_constant = inverse_mass * inverse_drag;
    float force;
    ofVec3f dir;
    float dis;

    // left side
    if (position.x < minPoint.x) {
        velocity.x = minPoint.x - position.x;
        position.x = minPoint.x+1;
    }
    if (position.x < minPoint.x + dist) {
        dir = ofVec3f(1.0f, 0.0f, 0.0f);
        dis = position.x - minPoint.x;
        dis = dist - dis;
        force = -k * dis * spring_constant;
        acceleration += dir*(-force);
    } else {
        // right side
        if (position.x > maxPoint.x) {
            velocity.x = maxPoint.x - position.x;
            position.x = maxPoint.x-1;
        }
        if (position.x > maxPoint.x - dist) {
            dir = ofVec3f(-1.0f, 0.0f, 0.0f);
            dis = maxPoint.x - position.x;
            dis = dist - dis;
            force =  -k * dis * spring_constant;
            acceleration += dir*(-force);
        }
    }

    // top side
    if (position.y < minPoint.y) {
        velocity.y = minPoint.y - position.y;
        position.y = minPoint.y+1;
    }
    if (position.y < minPoint.y + dist) {
        dir = ofVec3f(0.0f, 1.0f, 0.0f);
        dis = position.y - minPoint.y;
        dis = dist - dis;
        force = -k * dis * spring_constant;
        acceleration += dir*(-force);
    } else {
        // bottom side
        if (position.y > maxPoint.y) {
            velocity.y = maxPoint.y - position.y;
            position.y = maxPoint.y-1;
        }
        if (position.y > maxPoint.y - dist) {
            dir = ofVec3f(0.0f, -1.0f, 0.0f);
            dis = maxPoint.y - position.y;
            dis = dist - dis;
            force =  -k * dis * spring_constant;
            acceleration += dir*(-force);
        }
    }
}

void ofxParticle::applyForce(ofVec3f force) {
    acceleration += force * dt;
}

// void spring(ofPoint p, const float k, const float springDist, const float drag) { spring(p, k, springDist, drag); }
float ofxParticle::springBoth(ofxParticle * p, const float k, const float springDist, const float drag, const float springSnap) {
    if (abs(position.x - p->position.x) > springDist * springSnap || abs(position.y - p->position.y) > springDist * springSnap) return 0.0f;

    float dist = position.squareDistance(p->position);
    if(dist > springDist * springDist * springSnap) return 0.0f;

    dist = fastSqrt(dist);
    dist = springDist - dist;

    float force = (-k / (mass * p->mass)) * (dist / drag);

    ofVec3f dir = ((ofVec3f)(p->position - position));
    dir = (dir / fastSqrt(dir.lengthSquared()));

    acceleration += dir * force * dt;
    p->acceleration -= dir * force * dt;
    // acceleration += dir * (force * dt / mass);
    // p->acceleration -= dir * (force * dt / p->mass);
    return -force;
}

void ofxParticle::attractTo(ofxParticle p, const float accel, const float minDist, const bool consumeParticle) { 
    attractTo(p.position, accel, minDist, consumeParticle); 
}

void ofxParticle::attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle) {
    ofVec3f dir = p-position;
    float dist = dir.length();
    if(dist < minDist) {
        dist = minDist;
        if(consumeParticle)
            life = 0;
    }

    dir.normalize().scale(accel*dt);
    acceleration += dir;
}

void ofxParticle::gravitateTo(ofxParticle p, const float gravity, const float minDist, const bool consumeParticle) { 
    gravitateTo(p.position, gravity, p.mass, minDist, consumeParticle); 
}

void ofxParticle::gravitateTo(ofPoint p, const float gravity, const float mass2, const float minDist, const bool consumeParticle) {
    if(p==position) return;
    ofVec3f dir = p-position;
    float dist = dir.length();
    if(dist < minDist) {
        dist = minDist;
        if(consumeParticle)
            life = 0;
    }
    dir.normalize().scale( gravity * mass * mass2 * dt * (1.0f/(dist)));
    acceleration += dir;
}

void ofxParticle::gravitateBoth(ofxParticle * p, const float gravity, const float minDist, bool consumeParticle) {
   if(p==this) return;
    ofVec3f dir = p->position - position;
    float dist = dir.length();
    if(dist < minDist) {
        dist = minDist;
        if(consumeParticle)
            life = 0;
    }
    dir.normalize().scale( gravity * mass * p->mass * dt * (1.0/(dist)));
    acceleration += dir;
    p->acceleration -= dir;
}

void ofxParticle::rotateAround(ofxParticle p, const float accel, const float minDist, const bool consumeParticle ) { 
    rotateAround(p.position, accel, minDist, consumeParticle);
 }


void ofxParticle::rotateAround(ofPoint p, const float accel, const float minDist, const bool consumeParticle ) {
    ofVec3f toPoint = position - p;
    ofVec3f dir = ofVec3f(-toPoint.y, toPoint.x, toPoint.z);
    float dist = toPoint.length();
    if(dist < minDist) {
        dist = minDist;
        if(consumeParticle)
            life = 0;
    }
    dir.normalize().scale((accel/dist) *dt);

    acceleration += dir;
}

void ofxParticle::update(const float timeStep, const float drag) {
    dt = timeStep;
    ofVec3f a = (acceleration * dt) / mass;
    velocity += a;
    velocity -= (velocity * dt * (1.0f - drag));
    position += velocity * dt;
    acceleration -= a;
    rotation += rotationalVelocity * dt;

    life -= dt;
}

void ofxParticle::setVboLine( vector<ofVec2f> & points, const int & i, float trail){
    points[i*2] = position;
    points[i*2+1] = position-(velocity*dt*trail);
}

bool ofxParticle::isAlive() {return life > 0.0;}


ofxParticleEmitter & ofxParticleEmitter::setPosition(ofVec3f pos) {
    positionStart = positionEnd = pos;
    return *this;
}
ofxParticleEmitter & ofxParticleEmitter::setPosition(ofVec3f posStart, ofVec3f posEnd) {
    positionStart = posStart;
    positionEnd = posEnd;
    return *this;
}
ofxParticleEmitter & ofxParticleEmitter::setVelocity(ofVec3f vel) {
    velocityStart = velocityEnd = vel;
    return *this;
}
ofxParticleEmitter & ofxParticleEmitter::setVelocity(ofVec3f velStart, ofVec3f velEnd) {
    velocityStart = velStart;
    velocityEnd = velEnd;
    return *this;
}



ofxParticleSystem::ofxParticleSystem() {
    numParticles = 0;
    totalParticlesEmitted = 0;
}

ofxParticleSystem::~ofxParticleSystem() {}

void ofxParticleSystem::addParticles(ofxParticleEmitter & src) {
    for(int i=0;i<src.numPars;i++) {
        ofVec3f pos = src.positionStart;
        ofVec3f vel = src.velocityStart;
        if(src.positionEnd != src.positionStart || src.velocityStart != src.velocityEnd) {
            float rf = ofRandomuf();
            pos = src.positionStart.getInterpolated(src.positionEnd, rf);
            vel = src.velocityStart.getInterpolated(src.velocityEnd, rf);
        }
        ofVec3f p = pos+ofRandVec3f()*src.posSpread;
        ofVec3f v = vel+ofRandVec3f()*src.velSpread;
        float s = src.size+ofRandomf()*src.sizeSpread;
        float l = src.life+ofRandomf()*src.lifeSpread;
        ofxParticle * par = new ofxParticle(p,v,s,l);
        par->rotation = src.rotation+ofRandVec3f()*src.rotSpread;
        par->rotationalVelocity = src.rotVel+ofRandVec3f()*src.rotVelSpread;
        par->particleID = totalParticlesEmitted+i;
        particles.push_back(par);
    }
    numParticles+=src.numPars;
    totalParticlesEmitted+=src.numPars;
}

void ofxParticleSystem::attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle) {
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++) {
        (**it).attractTo(p, accel, minDist, consumeParticle);
    }
}

void ofxParticleSystem::gravitateTo(ofPoint p, const float gravity, const float mass, const float minDist, const float consumeParticle) {
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++) {
        (**it).gravitateTo(p, gravity, mass, minDist, consumeParticle);
    }
}

void ofxParticleSystem::rotateAround(ofPoint p, const float accel, const float minDist, const float consumeParticle) {
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++) {
        (**it).rotateAround(p, accel, minDist, consumeParticle);
    }
}

void ofxParticleSystem::applyVectorField(float * field, int fieldWidth, int fieldHeight, int numComponents, ofRectangle areaOfInfluence, float force) {
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++) {
        ofxParticle & p = (**it);
        ofVec2f pos(p.position.x,p.position.y);
        if(areaOfInfluence.inside(pos)) {
            int x = (int)ofMap(pos.x, areaOfInfluence.getMinX(), areaOfInfluence.getMaxX(), 0, fieldWidth-1);
            int y = (int)ofMap(pos.y, areaOfInfluence.getMinY(), areaOfInfluence.getMaxY(), 0, fieldHeight-1);
            int index = (x+y*fieldWidth)*numComponents;
            ofVec2f dir(field[index],field[index+1]);
            dir.scale(force);
            p.applyForce(dir);
        }
    }
}

int ofxParticleSystem::update(float timeStep, float drag) {
    int particlesRemoved = 0;
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++) {
        if((**it).isAlive()) {
            (**it).update(timeStep, drag);
        }
        else{
            ofxParticle * p = (*it);
            it = particles.erase(it);
            delete p;
            particlesRemoved++;
        }
    }
    numParticles-=particlesRemoved;
    return particlesRemoved;
}


void ofxParticleSystem::draw( float trail ){
    points.resize(particles.size()*2);
    int i=0; 
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        (**it).setVboLine( points, i, trail);
        i++;
    }
    vbo.setVertexData(&points[0], (int)points.size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_LINES, 0, (int)points.size());
}

void ofxParticleSystem::draw(){
    points.resize(particles.size());
    int i=0; 
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        points[i] = (**it).position;
        i++;
    }
    vbo.setVertexData(&points[0], (int)points.size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_POINTS, 0, (int)points.size());
}

int ofxParticleSystem::getNumParticles() {return numParticles;}

void ofxParticleSystem::binParticles(vector<ofxParticle *> particles, const int binPower, const ofRectangle bounds) {
    if (this->binPower != binPower || bounds != binBounds) {
        this->binPower = binPower;

        float binSize = (1 << binPower);

        xbins = ceilf(bounds.width / binSize);
        ybins = ceilf(bounds.height / binSize);

        bins.resize(xbins * ybins);
    }

    for (int i = 0; i < bins.size(); ++i) {
        bins[i].clear();
    }

    int xbin, ybin;
    ofxParticle * p;
    for (int i = 0; i < particles.size(); ++i) {
        p = particles[i];
        if (bounds.inside(p->position)) {
            xbin = ((unsigned) p->position.x) >> binPower;
            ybin = ((unsigned) p->position.y) >> binPower;
            bins[xbin + ybin * xbins].push_back(p);
        }
    }
}

int ofxParticleSystem::getNeighbors(vector<ofxParticle *> &neighbors, ofxParticle &p, const float radius) {
    return getNeighbors(neighbors, p.position.x, p.position.y, radius);
}

int ofxParticleSystem::getNeighbors(vector<ofxParticle *> &neighbors, float targetX, float targetY, const float radius) {
    //adapted from kylemcdonalds binned particle system
    unsigned int x, y, p, bindex, minX, minY, maxX, maxY;
    unsigned int minXBin, maxXBin, minYBin, maxYBin;
    minX = (int)(targetX - radius);
    minX = minX < 0 ? 0 : minX;
    maxX = (int)(targetX + radius);
    minY = (int)(targetY - radius);
    minY = minY < 0 ? 0 : minY;
    maxY = (int)(targetY + radius);
    minXBin = minX >> binPower;
    maxXBin = maxX >> binPower;
    minYBin = minY >> binPower;
    maxYBin = maxY >> binPower;
    maxXBin++;
    maxYBin++;

    if(maxXBin > xbins) {
        maxXBin = xbins;
    }
    if(maxYBin > ybins) {
        maxYBin = ybins;
    }

    neighbors.clear();

    for (y = minYBin; y < maxYBin; y++) {
        for (x = minXBin; x < maxXBin; x++) {
            bindex = x + y * xbins;
            for (p = 0; p < bins[bindex].size(); p++) {
                neighbors.push_back(bins[bindex][p]);
            }

        }
    }
    return neighbors.size();
}


