#ifndef __GEOMETRY_H
#define __GEOMETRY_H

#include <gl\gl.h>
#include <vector>

using std::vector;

class cVertex {
    public:
        cVertex() {};
        cVertex(double x, double y, double z) : x(x), y(y), z(z) {};
        double x;
        double y;
        double z;
};

typedef cVertex cNormal;

class cVertex2d {
    public:
        cVertex2d() {};
        cVertex2d(double x, double y) : x(x), y(y) {};

        union
        {
            double x;
            double w;
            double u;
        };
        union
        {
            double y;
            double h;
            double v;
        };
};

typedef cVertex2d cUVCoord;

class cQuadFace {
    public:
        cQuadFace() {}
        cQuadFace(  cVertex v1, cVertex v2, cVertex v3, cVertex v4,
                    cUVCoord uv1, cUVCoord uv2, cUVCoord uv3, cUVCoord uv4,
                    cNormal n);
        cVertex v[4];
        cUVCoord uv[4];
        cNormal n;
        GLuint texid;
};

class cFace {
    public:
        cFace() {}
        cFace(vector<cVertex> &vertices, vector<cUVCoord> &UVCoords, cNormal &normal, GLuint texId);

        vector<cVertex> v;
        vector<cUVCoord> uv;
        cNormal n;
        GLuint texId;
        unsigned int vCount;
};

class cRect {
    public:
        cRect() {}
        cRect(float x1, float y1, float x2, float y2);

        float x1;
        float y1;
        float x2;
        float y2;
};

#endif /* ifndef __GEOMETRY_H */
