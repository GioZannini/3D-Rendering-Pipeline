#ifndef ASSIGNMENT3_VALUEPROPERTIES_H
#define ASSIGNMENT3_VALUEPROPERTIES_H

#include <string>

namespace rasterPipeline {

    /*number of threads*/
    const unsigned int N_THREAD = 5;

    /*screen details*/
    const int WIDTH = 150;
    const int HEIGHT = 50;

    /*other details*/
    const int NUMBER_PROJECTION_ARRAY = 6;
    const int NUMBER_VERTICES_TRIANGLE = 3;
    const int NUMBER_DIFFERENT_MEASURES = 3;
    const int NUMBER_PROJECTION_MATRIX = 4;
    const int NUMBER_NORMALIZED_MEASURES = 4;
    const char PRINTED_CHARACTER_INVALID_CELL = '.';
    const std::string SPACE_OUTPUT = "          ";
    const int ASCII_CODE_0 = 48;
    const int NUMBER_CHARACTERISTICS_VERTEX_TRIANGLE = 3;

    /*file parameter*/
    const std::string FILE_SOURCE = "cubeMod.obj";

    /*parameter far and near parametric*/
    const int FAR = 2;
    const int NEAR = 1;

    /*define the projection array as left, top, right, bottom, near,  far*/
    const double projectionArray[NUMBER_PROJECTION_ARRAY] = {-1, -1, 1, 1, 1, 2};
    /*define the view matrix*/
    const double viewMatrix[NUMBER_PROJECTION_MATRIX * NUMBER_PROJECTION_MATRIX] = {0.5, 0.0, 0.0, 0.7,
                                                                                    0.0, 0.5, 0.0, 0.7,
                                                                                    0.0, 0.0, 0.5, 0.9,
                                                                                    0.0, 0.0, 0.0, 1.0};

    /*order triangles*/
    const int FLAT_CORNER = 180;
    const double PI = 3.14159;
    const int FULL_CIRCLE = 360;

}

#endif
