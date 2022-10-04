#ifndef ASSIGNMENT3_VERTEX_H
#define ASSIGNMENT3_VERTEX_H

#include <iostream>
#include <cmath>
#include "ValueProperties.h"

namespace rasterPipeline {

    class Vertex {
    public:

        /*constructor to initialize all fields equal zero*/
        Vertex() {
            x = y = z = normalX = normalY = normalZ = screenX = screenY = screenZ = u = v = 0;
        }

        /*ridefinition of the operator * between two vertex to calculate the inner product*/
        friend double operator*(Vertex va, Vertex vb) {
            return va.getScreenCoordinateX() * vb.getScreenCoordinateX() + va.getScreenCoordinateY() * vb.getScreenCoordinateY();
        }

        /*funzion used to calculate the euclidean norm*/
        double euclideanNorm() {
            return std::sqrt(std::pow(this->screenX, 2) + std::pow(this->screenY, 2));
        }

        /*function to set initial triangle coordinates*/
        void setCoordinates(double x, double y, double z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        /*function to set normalized coordinates*/
        void setNormalizedCoordinates(double normalX, double normalY, double normalZ) {
            this->normalX = normalX;
            this->normalY = normalY;
            this->normalZ = normalZ;
        }

        /*function to set screen coordinates*/
        void setScreenCoordinates(double screenX, double screenY, double screenZ) {
            this->screenX = screenX;
            this->screenY = screenY;
            this->screenZ = screenZ;
        }

        /*function to set texture coordinates*/
        void setTextureCoordinates(double u, double v) {
            this->u = u;
            this->v = v;
        }

        /*functions to set all screen coordinates*/
        void setScreenCoordinateX(double x) {
            this->screenX = x;
        }

        void setScreenCoordinateY(double y) {
            this->screenY = y;
        }

        void setScreenCoordinateZ(double z) {
            this->screenZ = z;
        }

        /*functions to get all initial coordinates*/
        double getCoordinateX() {
            return this->x;
        }

        double getCoordinateY() {
            return this->y;
        }

        double getCoordinateZ() {
            return this->z;
        }

        /*functions to get all normalized coordinates*/
        double getNormalCoordinateX() {
            return this->normalX;
        }

        double getNormalCoordinateY() {
            return this->normalY;
        }

        double getNormalCoordinateZ() {
            return this->normalZ;
        }

        /*functions to get all screen coordinates*/
        double getScreenCoordinateX() {
            return this->screenX;
        }

        double getScreenCoordinateY() {
            return this->screenY;
        }

        double getScreenCoordinateZ() {
            return this->screenZ;
        }

        /*functions to get all texture coordinates*/
        double getTextureCoordinateU() {
            return this->u;
        }

        double getTextureCoordinateV() {
            return this->v;
        }

        /*function to print triangle characteristics*/
        void print() {
            std::cout << SPACE_OUTPUT << SPACE_OUTPUT << "Input: X: " << this->x << " Y: " << this->y << " Z: " << this->z << std::endl
                      << SPACE_OUTPUT << SPACE_OUTPUT << "Normal X: " << this->normalX << " Y: " << this->normalY << " Z: " << this->normalZ << std::endl
                      << SPACE_OUTPUT << SPACE_OUTPUT << "Screen X: " << this->screenX << " Y: " << this->screenY << " Z: " << this->screenZ << std::endl
                      << SPACE_OUTPUT << SPACE_OUTPUT << "Textur U: " << this->u << " V: " << this->v << std::endl;
        }

    private:

        /*i added also the screen coordinates variables to maintain a complete schema for the vertex, in case i can eliminate them in the future assignments*/
        double x, y, z, normalX, normalY, normalZ, screenX, screenY, screenZ, u, v;

    };

}

#endif