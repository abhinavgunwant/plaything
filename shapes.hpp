#ifndef PLAYTHING_SHAPES
#define PLAYTHING_SHAPES

#include <string>
#include "raylib.h"
#include "common.hpp"

using namespace std;

enum Shape3DType {
	SHAPE_3D_CUBE,
	SHAPE_3D_LINE,
};

enum Shape2DType {
    SHAPE_2D_TEXT,
};

struct CubeData {
	Vector3 position;
	float width;
	float height;
	float length;
	Color color;
};

struct LineData {
	Vector3 startPos;
	Vector3 endPos;
	Color color;
};

union Shape3DData {
	CubeData cubeData;
	LineData lineData;
};

struct Shape3D {
	Shape3DType type;
	Shape3DData shapeData;
};

struct TextData {
	char* text;
	int fontSize;
	Color color;
};

union Shape2DData {
	TextData textData;
};

struct Shape2D {
    Shape2DType type;
    // TODO: see if float can be replaced by int here
    float x;
    // TODO: see if float can be replaced by int here
    float y;
    Shape2DData shapeData;
};

Shape2D Text(char* text, int x, int y);
Shape2D Text(char* text, int x, int y, int fontSize);
Shape2D Text(char* text, int x, int y, int fontSize, Color color);
Shape2D Text(char * text, int x, int y, Color color);

Shape3D Cube(Vector3 position, float width, float height, float length, Color color);
Shape3D Line(Vector3 startPos, Vector3 endPos, Color color);
Shape3D DefaultCube();
Shape3D XAxis();
Shape3D YAxis();
Shape3D ZAxis();

#endif

