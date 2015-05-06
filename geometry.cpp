#include "geometry.h"

cQuadFace::cQuadFace(cVertex v1, cVertex v2, cVertex v3, cVertex v4, cUVCoord uv1, cUVCoord uv2, cUVCoord uv3, cUVCoord uv4, cNormal n) {
    this->v[0] = v1;
    this->v[1] = v2;
    this->v[2] = v3;
    this->v[3] = v4;
    this->uv[0] = uv1;
    this->uv[1] = uv2;
    this->uv[2] = uv3;
    this->uv[3] = uv4;
    this->n = n;
}


cFace::cFace(vector<cVertex> &vertices, vector<cUVCoord> &UVCoords, cNormal &normal, GLuint texId)
: v(vertices), uv(UVCoords), n(normal), texId(texId)
{
    this->vCount = vertices.size();
}

cRect::cRect(float x1, float y1, float x2, float y2)
: x1(x1), y1(y1), x2(x2), y2(y2)
{}
