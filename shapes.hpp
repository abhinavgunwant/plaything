#ifndef PLAYTHING_SHAPES
#define PLAYTHING_SHAPES

#include <string>

#include "raylib.h"

using namespace std;

enum Shape3DType {
    SHAPE_3D_NONE,
	SHAPE_3D_CUBE,
	SHAPE_3D_LINE,
    SHAPE_3D_MESH,
};

enum Shape2DType {
    SHAPE_2D_NONE,
    SHAPE_2D_TEXT,
    SHAPE_2D_LINE,
    SHAPE_2D_RECTANGLE,
    SHAPE_2D_IMAGE,
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
	char * text;
	int fontSize;
	Color color;
    bool heapAllocated;
};

/**
 * Represents bot rectangle and line.
 *
 * The only difference is how it's interpreted in EntityManager::draw2D.
 */
struct Dimension2DData {
    int width;
    int height;
    Color color;
};

struct ImageData {
    int width;
    int height;
    Texture2D texture;
};

union Shape2DData {
	TextData textData;
    Dimension2DData dimensionData;
    ImageData imageData;
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
Shape2D Text(string text, int x, int y, int fontSize, Color color);
Shape2D Text(char* text, int x, int y, Color color);
Shape2D Line2D(int startX, int startY, int endX, int endY, Color color);
Shape2D Rect(int x, int y, int width, int height, Color color);
Shape2D Img(int x, int y, int width, int height, Texture2D imageTexture);
Shape2D Img(int x, int y, int width, int height, Image image);
Shape2D Img(int x, int y, int width, int height, string imagePath);

Shape3D Cube(Vector3 position, float width, float height, float length, Color color);
Shape3D Line(Vector3 startPos, Vector3 endPos, Color color);
Shape3D DefaultCube();
Shape3D XAxis();
Shape3D YAxis();
Shape3D ZAxis();

/**
 * Prints the debug info for the shape.
 */
void shape2DInfo(Shape2D shape);

#endif

