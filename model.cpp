#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include "model.h"
#include "geometry.h"
#include "commons.h"
#include "settings.h"
#include "debugout.h"

using std::ifstream;
using std::string;

GLuint cModel::nextTexId = 0;

cModel::cModel()
{
}

bool cModel::loadFile(string filename)
{
 	ifstream file;

    filename = OPT_PATH_MODELS + filename + OPT_PATHEXT_MODELS;
    file.open(filename.c_str());

    if (file.is_open())
    {
        string inpLine;
        vector<string> tokens;
        vector<cVertex> vertices;
        vector<cUVCoord> uvCoords;
        GLuint curTexId = 0;

    	while(!file.eof())
    	{
			getline(file, inpLine);
            tokens.clear();
            tokenize(inpLine, tokens, " ");

            if(inpLine.length() > 2)
            {
                if(tokens[0] == "v")
                {
                    cVertex newVertex(strToFloat(tokens[1]), strToFloat(tokens[2]), strToFloat(tokens[3]));
                    vertices.push_back(newVertex);
                }
                else if(tokens[0] == "vt")
                {
                    cUVCoord newUVCoord(strToFloat(tokens[1]), strToFloat(tokens[2]));
                    uvCoords.push_back(newUVCoord);
                }
                else if(tokens[0] == "usemtl")
                {
                    curTexId = loadTexture(OPT_PATH_TEXTURES + tokens[1] + OPT_PATHEXT_TEXTURES);
                }
                else if(tokens[0] == "f")
                {
                    vector<cVertex> faceVertices;
                    vector<cUVCoord> faceUVCoords;
                    cNormal faceNormal;
                    vector<string> tokenTokens;

                    for(unsigned int i = 1; i < tokens.size(); i ++)
                    {
                        tokenTokens.clear();
                        tokenize(tokens[i], tokenTokens, "/");
                        faceVertices.push_back( (strToInt(tokenTokens[0]) > 0) ? vertices[strToInt(tokenTokens[0]) - 1] : vertices[uvCoords.size() + strToInt(tokenTokens[0])]);
                        faceUVCoords.push_back( (strToInt(tokenTokens[1]) > 0) ? uvCoords[strToInt(tokenTokens[1]) - 1] : uvCoords[uvCoords.size() + strToInt(tokenTokens[1])]);
                    }

                    faceNormal = calcNormal(faceVertices);

                    cFace newFace(faceVertices, faceUVCoords, faceNormal, curTexId);
                    this->faces.push_back(newFace);
                }
            }
    	}
	}
	else
	{
	    output("Error: cModel::loadFile(): Couldn't access file (" + filename + "). Couldn't load model.");
	    return false;
	}

    file.close();
    this->displayListMade = false;

    debugoutput("Success: cModel::loadFile(): Loaded model from file '" + filename + "'");

    return true;
}

void cModel::draw()
{
    if(this->displayListMade)
    {
        glCallList(this->displayList);
    }
    else
    {
        this->displayList = glGenLists(1);

        glNewList(this->displayList, GL_COMPILE);

        GLuint lastTexid = 0xFFFFFFFF;
        unsigned int lastVCount = 0;

        for(unsigned int f = 0; f < this->faces.size(); f ++)
        {
            if((this->faces[f].texId != lastTexid) || (this->faces[f].vCount != lastVCount))
            {
                if(f > 0)
                {
                    glEnd();
                }

                if(this->faces[f].texId != lastTexid)
                {
                    glBindTexture(GL_TEXTURE_2D, this->faces[f].texId);
                    lastTexid = this->faces[f].texId;
                }

                if(this->faces[f].vCount == 3)
                {
                    glBegin(GL_TRIANGLES);
                }
                else if(this->faces[f].vCount == 4)
                {
                    glBegin(GL_QUADS);
                }

                lastVCount = this->faces[f].vCount;
            }

            glNormal3d(this->faces[f].n.x, this->faces[f].n.y, this->faces[f].n.z);
            for(unsigned int v = 0; v < this->faces[f].vCount; v ++)
            {
                glTexCoord2f(this->faces[f].uv[v].u, this->faces[f].uv[v].v);
                glVertex3f(this->faces[f].v[v].x, this->faces[f].v[v].y, this->faces[f].v[v].z);
            }
        }

        glEnd();
        glEndList();

        this->displayListMade = true;
    }
}

cNormal cModel::calcNormal(vector<cVertex> &vertices)
{
    // Calculate two vectors from three vertices
    cVertex a = cVertex(vertices[0].x - vertices[1].x,
                        vertices[0].y - vertices[1].y,
                        vertices[0].z - vertices[1].z);

    cVertex b = cVertex(vertices[1].x - vertices[2].x,
                        vertices[1].y - vertices[2].y,
                        vertices[1].z - vertices[2].z);

    // Calculate normal from the two vectors (cross product)
    cNormal n = cNormal((a.y * b.z) - (a.z * b.y),
                        (a.z * b.x) - (a.x * b.z),
                        (a.x * b.y) - (a.y * b.x));

    // Normalize to 1
    float len = (float)(sqrt((n.x * n.x) + (n.y * n.y) + (n.z * n.z)));
    if (len == 0.0) len = 1.0;

    n.x /= len;
    n.y /= len;
    n.z /= len;

    // All done
    return n;
}

GLuint cModel::loadTexture(string filename) {
    GLuint texId = cModel::nextTexId;
    cModel::nextTexId ++;

	HBITMAP hBMP;														// Handle Of The Bitmap
	BITMAP	BMP;														// Bitmap Structure

	glGenTextures(1, &texId);											// Create The Texture
	hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), filename.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );

	if (!hBMP)															// Does The Bitmap Exist?
		return FALSE;													// If Not Return False

	GetObject(hBMP, sizeof(BMP), &BMP);									// Get The Object
																		// hBMP:        Handle To Graphics Object
																		// sizeof(BMP): Size Of Buffer For Object Information
																		// &BMP:        Buffer For Object Information

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);								// Pixel Storage Mode (Word Alignment / 4 Bytes)

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texId);								// Bind To The Texture ID
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OPT_TEX_MIN_FILTER);	// Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OPT_TEX_MAG_FILTER);	// Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);													// Delete The Object

	return texId;														// Loading Was Successful
}
