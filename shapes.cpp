#include <iostream>
#include <cstring>

#include "raylib.h"

#include "common.hpp"
#include "shapes.hpp"

using namespace std;

Shape2D Text(char* text, int x, int y) {
	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { text, 20, GRAY } };
}
Shape2D Text(char* text, int x, int y, int fontSize) {
	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { text, fontSize, GRAY } };
}

Shape2D Text(char* text, int x, int y, int fontSize, Color color) {
	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { text, fontSize, color } };
}

Shape2D Text(string text, int x, int y, int fontSize, Color color) {
    char _charStr[20];
    strcpy_s(_charStr, (char *) text.c_str());

    char * cstr = new char[text.length() + 1];

    for (int i=0; i<text.length()+1; ++i) {
        cstr[i] = _charStr[i];
    }

	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { cstr, fontSize, color, true } };
}

Shape2D Text(char* text, int x, int y, Color color) {
	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { text, 20, color } };
}

Shape2DData dimensionShape(int width, int height, Color color) {
    Dimension2DData d = { width, height, color };
    Shape2DData s;
    s.dimensionData = d;
    return s;
}

Shape2D Line2D(int x, int y, int width, int height, Color color) {
    // Dimension2DData l2d = Dimension2DData { width, height, color };
    // Shape2DData s2d;
    // s2d.dimensionData = l2d;

    // return { SHAPE_2D_LINE, (float)x, (float)y, s2d };
    return { SHAPE_2D_LINE, (float)x, (float)y, dimensionShape(width, height, color) };
}

Shape2D Rect(int x, int y, int width, int height, Color color) {
    // Dimension2DData l2d = Dimension2DData { width, height, color };
    // Shape2DData s2d;
    // s2d.dimensionData = l2d;

    // return { SHAPE_2D_LINE, (float)x, (float)y, s2d };
    return { SHAPE_2D_RECTANGLE, (float)x, (float)y, dimensionShape(width, height, color) };
}

Shape2D Img(int x, int y, int width, int height, Texture2D imageTexture) {
    ImageData i = { width, height, imageTexture };
    Shape2DData s;
    s.imageData = i;

    return { SHAPE_2D_IMAGE, (float)x, (float)y, s };
}

Shape2D Img(int x, int y, int width, int height, Image image) {
    return Img(x, y, width, height, imageToTexture(width, height, image) );
}

Shape2D Img(int x, int y, int width, int height, string imagePath) {
    if (imagePath.empty()) { return { SHAPE_2D_NONE }; }

    Image img = LoadImage(imagePath.c_str());

    return Img(x, y, width, height, img);
}

Shape3D Cube(Vector3 position, float width, float height, float length, Color color) {
	return { SHAPE_3D_CUBE, CubeData { position, height, width, length, color } };
}

Shape3D Line(Vector3 startPos, Vector3 endPos, Color color) {
	LineData ld = LineData{ startPos, endPos, color };
	Shape3DData sd;
	sd.lineData = ld;

	return { SHAPE_3D_LINE, sd };
}

Shape3D DefaultCube() {
	return { SHAPE_3D_CUBE, CubeData { VEC_ZERO, 0.0f, 0.0f, 0.0f, RED } };
}

Shape3D XAxis() {
	LineData ld = LineData { VEC_ZERO, { 50.0f, 0.0f, 0.0f }, RED };
	Shape3DData sd;
	sd.lineData = ld;

	return { SHAPE_3D_LINE, sd };
}

Shape3D YAxis() {
	LineData ld = LineData{ VEC_ZERO, { 0.0f, 50.0f, 0.0f }, GREEN };
	Shape3DData sd;
	sd.lineData = ld;

	return { SHAPE_3D_LINE, sd };
}

Shape3D ZAxis() {
	LineData ld = LineData{ VEC_ZERO, { 0.0f, 0.0f, 50.0f }, BLUE };
	Shape3DData sd;
	sd.lineData = ld;

	return { SHAPE_3D_LINE, sd };
}

void shape2DInfo(Shape2D shape) {
    cout << "\nShape: { type: ";

    switch (shape.type) {
        case SHAPE_2D_TEXT: {
            cout << "SHAPE_2D_TEXT, x: " << shape.x << ", y: " << shape.y
                << ", shapeData: TextData { text: " << shape.shapeData.textData.text
                << ", fontSize: " << shape.shapeData.textData.fontSize
                << ", color: TODO: print color"
                << " } }";
            break;
        }

        case SHAPE_2D_LINE: {
            cout << "SHAPE_2D_LINE, x:" << shape.x << ", y: " << shape.y
                << ", shapeData: Dimension2DData { width: " << shape.shapeData.dimensionData.width
                << ", height: " << shape.shapeData.dimensionData.height
                << ", color: TODO: print color"
                << " } }";
            break;
        }

        case SHAPE_2D_RECTANGLE: {
            cout << "SHAPE_2D_RECTANGLE, x:" << shape.x << ", y: " << shape.y
                << ", shapeData: Dimension2DData { width: " << shape.shapeData.dimensionData.width
                << ", height: " << shape.shapeData.dimensionData.height
                << ", color: TODO: print color"
                << " } }";
            break;
        }

        case SHAPE_2D_IMAGE: {
            cout << "SHAPE_2D_IMAGE";
            break;
        }
    }
}

