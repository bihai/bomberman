#include <deque>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <windows.h>
#include "geometry.h"
#include "particle.h"
#include "model.h"
#include "settings.h"
#include "debugout.h"

namespace ptl {

    using std::deque;

    deque<cParticle> particles;
    GLuint texId;
    int liveParticles;

    void initParticles()
    {
        particles.resize(OPT_NUM_INITIAL_PTLS);
        deque<cParticle>::iterator ptlIter;
        for( ptlIter = particles.begin(); ptlIter != particles.end(); ptlIter ++ )
        {
            ptlIter->active = false;
        }

        cModel dummyModel;
        texId = dummyModel.loadTexture(OPT_PATH_TEXTURES + (string)"particle" + OPT_PATHEXT_TEXTURES);
    }

    void createParticles(deque<cParticle> &newParticles)
    {
        deque<cParticle>::iterator newPtlIter = newParticles.begin();

        // Check for unused spots
        deque<cParticle>::iterator ptlIter;
        for( ptlIter = particles.begin(); ptlIter != particles.end(); ptlIter ++ )
        {
            if(! ptlIter->active)
            {
                *ptlIter = *newPtlIter;
                ptlIter->active = true;
                newPtlIter ++;
                if(newPtlIter == newParticles.end()) return;
            }
        }

        if((newPtlIter != newParticles.end()) && (liveParticles < OPT_MAX_PTLS))
        {
            // Didn't find unused spot, so create them
            for( newPtlIter = newPtlIter; newPtlIter != newParticles.end(); newPtlIter ++ )
            {
                particles.push_back(*newPtlIter);
                particles.back().active = true;
            }
        }
    }

    void tickParticles()
    {
        deque<cParticle>::iterator ptlIter;
        for( ptlIter = particles.begin(); ptlIter != particles.end(); ptlIter ++ )
        {
            if(ptlIter->life > 0)
            {
                ptlIter->position.x += ptlIter->velocity.x / 100;
                ptlIter->position.y += ptlIter->velocity.y / 100;
                ptlIter->position.z += ptlIter->velocity.z / 100;

                ptlIter->velocity.x += ptlIter->acceleration.x / 100;
                ptlIter->velocity.y += ptlIter->acceleration.y / 100;
                ptlIter->velocity.z += ptlIter->acceleration.z / 100;

                ptlIter->life -= ptlIter->fade / 100;
            }
            else
            {
                ptlIter->active = false;
            }
        }
    }

    void renderParticles()
    {
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, texId);

        liveParticles = 0;

        deque<cParticle>::iterator ptlIter;
        for( ptlIter = particles.begin(); ptlIter != particles.end(); ptlIter++ )
        {
            if(ptlIter->active)
            {
                glBegin(GL_TRIANGLE_STRIP);
                    glColor4f(ptlIter->r, ptlIter->g, ptlIter->b, ptlIter->life);
                    glTexCoord2d(1,1); glVertex3f(ptlIter->position.x + 0.1f, ptlIter->position.y, ptlIter->position.z + 0.1f); // Top Right
                    glTexCoord2d(0,1); glVertex3f(ptlIter->position.x - 0.1f, ptlIter->position.y, ptlIter->position.z + 0.1f); // Top Left
                    glTexCoord2d(1,0); glVertex3f(ptlIter->position.x + 0.1f, ptlIter->position.y, ptlIter->position.z - 0.1f); // Bottom Right
                    glTexCoord2d(0,0); glVertex3f(ptlIter->position.x - 0.1f, ptlIter->position.y, ptlIter->position.z - 0.1f); // Bottom Left
                glEnd();
                liveParticles ++;
            }
        }

        glEnable(GL_LIGHTING);
    }

    int getLiveParticles()
    {
        return liveParticles;
    }

} /* namespace ptl */
