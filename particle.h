#ifndef __PARTICLE_H
#define __PARTICLE_H


#include <string>
#include <deque>

namespace ptl {

#include "geometry.h"
#include "gl/gl.h"

using std::string;
using std::deque;

class cParticle
{
    public:
        cParticle() {}
        cParticle(float fade, float r, float g, float b, cVertex position, cVertex velocity, cVertex acceleration)
            : life(1.0), fade(fade), r(r), g(g), b(b), position(position), velocity(velocity), acceleration(acceleration)
            {}

        float life;
        float fade;

        float r;
        float g;
        float b;

        cVertex position;
        cVertex velocity;
        cVertex acceleration;

        bool active;
};

void initParticles();
void createParticles(deque<cParticle> &newParticles);
void tickParticles();
void renderParticles();
GLuint loadTexture(string filename);
int getLiveParticles();

}
#endif /* ifndef __PARTICLE_H */
