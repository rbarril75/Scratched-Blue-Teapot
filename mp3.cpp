// Written on Mac OS X 10.6

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
#include "SOIL/SOIL.h"
//#include "glm.h"

#define PI 3.14159

char * texFH = "blue.jpg";
char * envFH = "rnl_probe2.bmp";

void loadTextures() {
	GLuint textures[2];
	glGenTextures(2, textures);
	
	int w, h;
	unsigned char * image;
	
	// Texture mapping
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	image = SOIL_load_image(texFH, &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	
	// Environment mapping
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	image = SOIL_load_image(envFH, &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
}

void loadLights() {
	// LIGHT0
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 3.0, 0.0, 0.0 };
	
	glShadeModel (GL_SMOOTH);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	// END LIGHT0
	
	// LIGHT1
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position1[] = { -0.2, 1.0, 0.2, 0.0 };
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	// END LIGHT1
	
	// LIGHT2
	GLfloat light_position2[] = { 0.0, -1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	// END LIGHT2
	
	// LIGHT3
	GLfloat light_position3[] = { -2.0, 0.5, 0.0, 0.0 };
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	// END LIGHT3
	
	// LIGHT4
	GLfloat light_position4[] = { 2.0, -0.5, 0.0, 0.0 };
	glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
	// END LIGHT4
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
}

void init() {
	loadTextures();
	loadLights();
}

// Vertex class encapsulating vertex coordinates and its associated normal and texture coordinates.
class Vertex {
public:
	float x, y, z;
	float normal[3];
	float texcoords[2];
	
	Vertex(float x, float y, float z) {
		this->x = x; this->y = y; this->z = z;
		normal[0] = 0.0; normal[1] = 0.0; normal[2] = 0.0;
	}
};

// Face/Triangle class encapsulating three vertex corners and associated normal.
class Face {
public:
	Vertex * v1, * v2, * v3;
	float * normal;

	Face(Vertex * v1, Vertex * v2, Vertex * v3) {
		this->v1 = v1; this->v2 = v2; this->v3 = v3;
	}
};

class Model {
public:
	Vertex ** vertexList; // array of Vertex pointers
	Face ** faceList; // array of Face pointers
	int numVertices;
	int numFaces;
	
	// Populate Model fields using data from .obj file.
	void parseOBJ(char * filename) {
		FILE * file = fopen(filename, "r");
		
		char buf[128];
		float vBuf[3];
		int fBuf[3];
		while (fscanf(file, "%s", buf) != EOF) {
			if (buf[0] == '#') { // comment
				fgets(buf, sizeof(buf), file); 
				continue;
			}
			else if (buf[0] == 'v') { // vertex
				fscanf(file, "%f %f %f", &vBuf[0], &vBuf[1], &vBuf[2]);
				vertexList[numVertices] = new Vertex(vBuf[0], vBuf[1], vBuf[2]);
				numVertices++;
				continue;
			}
			else if (buf[0] == 'f') { // face
				fscanf(file, "%d %d %d", &fBuf[0], &fBuf[1], &fBuf[2]);
				faceList[numFaces] = 
				new Face(vertexList[fBuf[0]], vertexList[fBuf[1]], vertexList[fBuf[2]]);
				numFaces++;
				continue;
			}
		}
	}
	
	// Returns cross product of 3-element vectors u and v.
	float * cross(float * u, float * v) {
		float * n = new float[3];
		n[0] = u[1]*v[2] - u[2]*v[1];
		n[1] = u[2]*v[0] - u[0]*v[2];
		n[2] = u[0]*v[1] - u[1]*v[0];
		return n;
	}
	
	// Normalizes 3-element vector.
	void normalize(float * v) {
		float mag = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		v[0] /= mag; v[1] /= mag; v[2] /= mag;
	}
	
	// Find face normal by crossing two edges of face, then normalizing.
	void calcFaceNormal(Face * f) {
		float u[3];
		float v[3];
		
		u[0] = f->v2->x - f->v1->x;
		u[1] = f->v2->y - f->v1->y;
		u[2] = f->v2->z - f->v1->z;
		v[0] = f->v3->x - f->v1->x;
		v[1] = f->v3->y - f->v1->y;
		v[2] = f->v3->z - f->v1->z;
		
		f->normal = cross(u, v);
		normalize(f->normal);
	}
	
	// Add a normal to a vertex.
	void addNormal(Vertex * v, float * normal) {
		v->normal[0] += normal[0];
		v->normal[1] += normal[1];
		v->normal[2] += normal[2];
	}
	
	// Calculate both face normals and vertex normals.
	void calculateNormals() {
		// Calculate face normals and add them to relevant vertex normals.
		int i;
		for (i = 1; i < numFaces; i++) {
			Face * f = faceList[i];
			calcFaceNormal(f);
			addNormal(f->v1, f->normal);
			addNormal(f->v2, f->normal);
			addNormal(f->v3, f->normal);
		}
		
		// Normalize vertex normals.
		for (i = 1; i < numVertices; i++) {
			normalize(vertexList[i]->normal);
		}
	}
	
	// Set texture coordinates. 
	// Sphere texture coordinates formula derived from GLM library function glmSpheremapTexture().
	void calculateTexCoords() {
		float theta, phi, rho, x, y, z, r;
		int i;
		for (i = 1; i < numVertices; i++) {
			z = vertexList[i]->normal[0];
			y = vertexList[i]->normal[1];
			x = vertexList[i]->normal[2];

			r = sqrt((x * x) + (y * y));
			rho = sqrt((r * r) + (z * z));
			
			if(r == 0.0) {
				theta = 0.0;
				phi = 0.0;
			} else {
				if(z == 0.0)
					phi = 3.14159265 / 2.0;
				else
					phi = acos(z / rho);
				
				if(y == 0.0)
					theta = 3.141592365 / 2.0;
				else
					theta = asin(y / r) + (3.14159265 / 2.0);
			}
			
			vertexList[i]->texcoords[0] = theta / 3.14159265;
			vertexList[i]->texcoords[1] = phi / 3.14159265;
		}
	}
	
	float abs(float f) {
		return (f > 0) ? f : -f;
	}
	
	float max(float a, float b) {
		return (a > b) ? a : b;
	}
	
	// Unitization process derived from GLM library function glmUnitize()
	void unitize() {
		float maxX, minX, maxY, minY, maxZ, minZ;
		
		maxX = minX = vertexList[1]->x;
		maxY = minY = vertexList[1]->y;
		maxZ = minZ = vertexList[1]->z;

		int i;
		for (i = 1; i < numVertices; i++) {
			maxX = (maxX > vertexList[i]->x) ? maxX : vertexList[i]->x;
			minX = (minX < vertexList[i]->x) ? minX : vertexList[i]->x;
			maxY = (maxY > vertexList[i]->y) ? maxY : vertexList[i]->y;
			minY = (minY < vertexList[i]->y) ? minY : vertexList[i]->y;
			maxZ = (maxZ > vertexList[i]->z) ? maxZ : vertexList[i]->z;
			minZ = (minZ < vertexList[i]->z) ? minZ : vertexList[i]->z;
		}
		
		int w = abs(maxX) + abs(minX);
		int h = abs(maxY) + abs(minY);
		int d = abs(maxZ) + abs(minZ);
		
		float cx = (maxX + minX) / 2.0;
		float cy = (maxY + minY) / 2.0;
		float cz = (maxZ + minZ) / 2.0;
		
		float scale = 2.0 / max(max(w, h), d);
		
		for (i = 1; i < numVertices; i++) {
			vertexList[i]->x -= cx; vertexList[i]->x *= scale;
			vertexList[i]->y -= cy; vertexList[i]->y *= scale;
			vertexList[i]->z -= cz; vertexList[i]->z *= scale;
		}
	}
	
	// Constructor
	Model(char * filename) {
		vertexList = (Vertex **) malloc(sizeof(Vertex *) * 1202);
		faceList = (Face **) malloc(sizeof(Face *) * 2256);
		numVertices = 1;
		numFaces = 1;
		
		parseOBJ(filename);
		calculateNormals();
		unitize();
		calculateTexCoords();
	}
	
	// For a vertex, set its normal and texture coordinates, then draw it.
	void drawVertex(Vertex * v) {
		float nArray[3] = {v->normal[0], v->normal[1], v->normal[2]};
		glNormal3fv(nArray);
		float tArray[2] = {v->texcoords[0], v->texcoords[1]};
		glMultiTexCoord2f(GL_TEXTURE0, tArray[0], tArray[1]);
		glMultiTexCoord2f(GL_TEXTURE1, tArray[0], tArray[1]);
		//glTexCoord2fv(tArray);
		float vArray[3] = {v->x, v->y, v->z};
		glVertex3fv(vArray);
	}
	
	// Draw the model.
	void draw() {
		glBegin(GL_TRIANGLES);
		for (int i = 1; i < 2257; i++) { // for each face, draw a triangle with its three vertices
			drawVertex(faceList[i]->v1);
			drawVertex(faceList[i]->v2);
			drawVertex(faceList[i]->v3);
		}
		glEnd();
	}
};

void display(void) {	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	// Load model
	Model * mod = new Model("teapot.obj");
	
	/* GLM model, for reference
	GLMmodel* model = glmReadOBJ("teapot.obj");
	glmFacetNormals(model); 
	glmVertexNormals(model, 90.0);
	glmUnitize(model);
	glmSpheremapTexture(model);
	*/
	
	// Projection transformation	
	glMatrixMode(GL_PROJECTION);
	glFrustum(1, 0, 0, 1, 0, 10);
	glRotatef(55, 0, 1, 0);
	glRotatef(-35, 0, 0, 1);
	glRotatef(35, 1, 0, 0);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST); // Remove hidden surfaces
	
	// Draw the teapot
	mod->draw();
	//glmDraw(model, GLM_SMOOTH | GLM_TEXTURE); 
	
	glFlush();
	glutSwapBuffers();	// swap front/back framebuffer to avoid flickering 
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 27) 
		exit(0);
}

int main(int argc, char ** argv) {
	glutInit(&argc, (char**)argv);
	
	// set up for double-buffering & RGB color buffer & depth test
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	// set up window
	glutInitWindowSize (700, 700); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ((const char*) "Teapot");
	init();
	
	glClearColor(0, 0, 0, 1.0); // set background color
		
	// set up the call-back functions 
	glutDisplayFunc(display);  // called when drawing 
	//glutReshapeFunc(reshape);  // called when change window size
	glutKeyboardFunc(keyboard); // called when received keyboard interaction
	//glutTimerFunc(100, timer, FPS); // a periodic timer. Usually used for updating animation
	
	glutMainLoop(); // start the main callback loop
	
	return 0;
}