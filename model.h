#ifndef __MODEL_H
#define __MODEL_H

#include <string>
#include <vector>
#include "geometry.h"
#include "gl/gl.h"

using std::string;
using std::vector;


class cModel {
    public:
        cModel();

        bool loadFile(string filename);
        void draw();

        GLuint loadTexture(string filename);

    private:
        cNormal calcNormal(vector<cVertex> &vertices);

        static GLuint nextTexId;
        GLuint displayList;
        bool displayListMade;
        vector<cFace> faces;
};

#endif /* ifndef __MODEL_H */
