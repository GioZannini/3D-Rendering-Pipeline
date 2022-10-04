#ifndef ASSIGNMENT3_TRIANGLE_H
#define ASSIGNMENT3_TRIANGLE_H

#include <iostream>
#include "ValueProperties.h"
#include "Vertex.h"



namespace rasterPipeline {

    class Triangle {
    public:

        /*function used to set the vertex coordinates of this triangle*/
        void setVertexCoordinates(Vertex v1, Vertex v2, Vertex v3) {
            this->vertices[0].setCoordinates(v1.getCoordinateX(), v1.getCoordinateY(), v1.getCoordinateZ());
            this->vertices[1].setCoordinates(v2.getCoordinateX(), v2.getCoordinateY(), v2.getCoordinateZ());
            this->vertices[2].setCoordinates(v3.getCoordinateX(), v3.getCoordinateY(), v3.getCoordinateZ());
        }

        /*function used to set the vertex normalized coordinates in a single vertex*/
        void setVertexNormalizedCoordinates(Vertex v, int i) {
            this->vertices[i].setNormalizedCoordinates(v.getNormalCoordinateX(), v.getNormalCoordinateY(), v.getNormalCoordinateZ());
        }

        /*function used to set the vertex texture coordinates of this triangle*/
        void setVertexTextureCoordinates(Vertex v1, Vertex v2, Vertex v3) {
            this->vertices[0].setTextureCoordinates(v1.getTextureCoordinateU(), v1.getTextureCoordinateV());
            this->vertices[1].setTextureCoordinates(v2.getTextureCoordinateU(), v2.getTextureCoordinateV());
            this->vertices[2].setTextureCoordinates(v3.getTextureCoordinateU(), v3.getTextureCoordinateV());
        }

        /*function used to get all vertices of this triangle*/
        Vertex *getVertices() {
            return this->vertices;
        }

        /*function used to print the triangle properties and vertices to get a good vision of this triangle characteristics*/
        void print(int triangleNumber) {
            std::cout << "Triangolo " << triangleNumber << std::endl;
            for (int i = 0; i < NUMBER_VERTICES_TRIANGLE; ++i) {
                std::cout << SPACE_OUTPUT << "Vertice " << i + 1 << std::endl;
                this->vertices[i].print();
            }
        }

    private:

        /*definition of 3 vertices for each triangle*/
        Vertex vertices[NUMBER_VERTICES_TRIANGLE];

    };

}

#endif
