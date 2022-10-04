#ifndef ASSIGNMENT3_OPERATION_H
#define ASSIGNMENT3_OPERATION_H

#include <cmath>
#include <cfloat>
#include <vector>
#include <mutex>
#include "ValueProperties.h"
#include "Triangle.h"
#include "Vertex.h"

namespace rasterPipeline {

    template<class Target_t, class Shader>
    class OperationRasterizer {
    public:

        /*function to set screen*/
        void setScreen(Target_t *screen) {
            this->screen = screen;
        }

        /*function to set zbuffer*/
        void setZbuffer(double *zbuffer) {
            this->zbuffer = zbuffer;
        }

        /*function to set projection matrix*/
        void setProjectionMatrix(double *projectionMatrix) {
            this->projectionMatrix = projectionMatrix;
        }

        /*function to set the strategy pattern*/
        void setShader(Shader shader) {
            this->shader = shader;
        }

        /*function to set mutex*/
        void setMutex(std::mutex *mutex) {
            this->mutex = mutex;
        }

        /*function used to calculate the perspective matrix*/
        void perspectiveProjection() {
            this->projectionMatrix[0] = 2 * projectionArray[4] / (projectionArray[2] - projectionArray[0]);
            this->projectionMatrix[1] = 0;
            this->projectionMatrix[2] = -(projectionArray[2] + projectionArray[0]) / (projectionArray[2] - projectionArray[0]);
            this->projectionMatrix[3] = 0;
            this->projectionMatrix[4] = 0;
            this->projectionMatrix[5] = 2 * projectionArray[4] / (projectionArray[3] - projectionArray[1]);
            this->projectionMatrix[6] = -(projectionArray[3] + projectionArray[1]) / (projectionArray[3] - projectionArray[1]);
            this->projectionMatrix[7] = 0;
            this->projectionMatrix[8] = 0;
            this->projectionMatrix[9] = 0;
            this->projectionMatrix[10] = (projectionArray[5] + projectionArray[4]) / (projectionArray[5] - projectionArray[4]);
            this->projectionMatrix[11] = -2 * projectionArray[4] * projectionArray[5] / (projectionArray[5] - projectionArray[4]);
            this->projectionMatrix[12] = 0;
            this->projectionMatrix[13] = 0;
            this->projectionMatrix[14] = 1;
            this->projectionMatrix[15] = 0;
        }

        /*trasform the coordinates and normalized coordinates of each triangle using the view matrix*/
        void transformCoordinatesUsingViewMatrix(std::vector<Triangle> &triangles) {
            for (unsigned int k = 0; k < triangles.size(); ++k) {
                Vertex *vertices = triangles[k].getVertices();
                double coordinates[NUMBER_NORMALIZED_MEASURES];
                double normalizedCoordinates[NUMBER_NORMALIZED_MEASURES];
                for (int i = 0; i < NUMBER_VERTICES_TRIANGLE; ++i) {
                    for (int j = 0; j < NUMBER_NORMALIZED_MEASURES; ++j) {
                        coordinates[j] =
                                vertices[i].getCoordinateX() * viewMatrix[j * NUMBER_NORMALIZED_MEASURES + 0] +
                                vertices[i].getCoordinateY() * viewMatrix[j * NUMBER_NORMALIZED_MEASURES + 1] +
                                vertices[i].getCoordinateZ() * viewMatrix[j * NUMBER_NORMALIZED_MEASURES + 2] +
                                1 * viewMatrix[j * NUMBER_NORMALIZED_MEASURES + 3];
                        normalizedCoordinates[j] =
                                vertices[i].getNormalCoordinateX() * viewMatrix[j * NUMBER_NORMALIZED_MEASURES + 0] +
                                vertices[i].getNormalCoordinateY() * viewMatrix[j * NUMBER_NORMALIZED_MEASURES + 1] +
                                vertices[i].getNormalCoordinateZ() * viewMatrix[j * NUMBER_NORMALIZED_MEASURES + 2];
                    }
                    for (int j = 0; j < NUMBER_VERTICES_TRIANGLE; ++j) {
                        coordinates[j] = coordinates[j] / coordinates[3];
                    }
                    vertices[i].setCoordinates(coordinates[0], coordinates[1], coordinates[2]);
                    vertices[i].setNormalizedCoordinates(normalizedCoordinates[0], normalizedCoordinates[1], normalizedCoordinates[2]);
                }
            }
        }

        /*function used to calculate the normalized coordinates for each triangle*/
        void calculateNormalizedVertices(std::vector<Triangle> &triangles) {
            for (unsigned int k = 0; k < triangles.size(); ++k) {
                Vertex *vertices = triangles[k].getVertices();
                double normalizedmeasures[NUMBER_NORMALIZED_MEASURES];
                for (int i = 0; i < NUMBER_VERTICES_TRIANGLE; ++i) {
                    for (int j = 0; j < NUMBER_NORMALIZED_MEASURES; ++j) {
                        normalizedmeasures[j] =
                                vertices[i].getCoordinateX() * this->projectionMatrix[j * NUMBER_NORMALIZED_MEASURES + 0] +
                                vertices[i].getCoordinateY() * this->projectionMatrix[j * NUMBER_NORMALIZED_MEASURES + 1] +
                                vertices[i].getCoordinateZ() * this->projectionMatrix[j * NUMBER_NORMALIZED_MEASURES + 2] +
                                1 * this->projectionMatrix[j * NUMBER_NORMALIZED_MEASURES + 3];
                    }
                    for (int j = 0; j < NUMBER_VERTICES_TRIANGLE; ++j) {
                        normalizedmeasures[j] = normalizedmeasures[j] / normalizedmeasures[3];
                    }
                    vertices[i].setNormalizedCoordinates(normalizedmeasures[0], normalizedmeasures[1], normalizedmeasures[2]);
                }
            }
        }

        /*function used to convert the normalized coordinates of input parameter triangles into screen coordinates*/
        void transformNormalizedCoordinatesInScreenCoordinates(std::vector<Triangle> &triangles) {
            for (unsigned int k = 0; k < triangles.size(); ++k) {
                Vertex *vertices = triangles[k].getVertices();
                for (int i = 0; i < NUMBER_VERTICES_TRIANGLE; ++i) {
                    vertices[i].setScreenCoordinateX((vertices[i].getNormalCoordinateX() + 1) * (WIDTH - 1) / 2);
                    vertices[i].setScreenCoordinateY((vertices[i].getNormalCoordinateY() + 1) * (HEIGHT - 1) / 2);
                    vertices[i].setScreenCoordinateZ(((vertices[i].getNormalCoordinateZ() + 1) / 2) * (FAR - NEAR) + NEAR);
                }
            }
        }

        /*function that for each triangle orders its vertexes in clockwise order*/
        void orderTriangleVerticesClockwiseOrder(std::vector<Triangle> &triangles) {
            /*for each triangle*/
            for (unsigned int i = 0; i < triangles.size(); ++i) {
                Vertex *vertices = triangles[i].getVertices();
                // find the first vertex (lower left)
                if (vertices[0].getScreenCoordinateX() <= vertices[1].getScreenCoordinateX() && vertices[0].getScreenCoordinateX() <= vertices[2].getScreenCoordinateX()) {
                    if (vertices[0].getScreenCoordinateX() == vertices[1].getScreenCoordinateX()) {
                        if (vertices[1].getScreenCoordinateY() < vertices[0].getScreenCoordinateY()) {
                            std::swap(vertices[0], vertices[1]);
                        }
                    }
                    if (vertices[0].getScreenCoordinateX() == vertices[2].getScreenCoordinateX()) {
                        if (vertices[2].getScreenCoordinateY() < vertices[0].getScreenCoordinateY()) {
                            std::swap(vertices[0], vertices[2]);
                        }
                    }
                } else {
                    if (vertices[1].getScreenCoordinateX() < vertices[2].getScreenCoordinateX()) {
                        std::swap(vertices[0], vertices[1]);
                    } else if (vertices[2].getScreenCoordinateX() < vertices[1].getScreenCoordinateX()) {
                        std::swap(vertices[0], vertices[2]);
                    } else {
                        if (vertices[2].getScreenCoordinateY() < vertices[1].getScreenCoordinateY()) {
                            std::swap(vertices[0], vertices[2]);
                        } else {
                            std::swap(vertices[0], vertices[1]);
                        }
                    }
                }
                Vertex va_aux = vertices[0];
                Vertex vb_aux = vertices[1];
                Vertex vc_aux = vertices[2];
                double angle1, angle2;
                // Traslation of all vertexes on the origin
                va_aux.setScreenCoordinateX(va_aux.getScreenCoordinateX() - vertices[0].getScreenCoordinateX());
                // in this way I will have a vector parallel to y axis
                va_aux.setScreenCoordinateY(va_aux.getScreenCoordinateY() - vertices[0].getScreenCoordinateY() + HEIGHT / 2);
                vb_aux.setScreenCoordinateX(vb_aux.getScreenCoordinateX() - vertices[0].getScreenCoordinateX());
                vb_aux.setScreenCoordinateY(vb_aux.getScreenCoordinateY() - vertices[0].getScreenCoordinateY());
                vc_aux.setScreenCoordinateX(vc_aux.getScreenCoordinateX() - vertices[0].getScreenCoordinateX());
                vc_aux.setScreenCoordinateY(vc_aux.getScreenCoordinateY() - vertices[0].getScreenCoordinateY());
                angle1 = FULL_CIRCLE - std::acos((va_aux * vb_aux) / (va_aux.euclideanNorm() * vb_aux.euclideanNorm())) * FLAT_CORNER / PI;
                angle2 = FULL_CIRCLE - std::acos((va_aux * vc_aux) / (va_aux.euclideanNorm() * vc_aux.euclideanNorm())) * FLAT_CORNER / PI;
                if (angle1 > angle2) {
                    std::swap(vertices[1], vertices[2]);
                }
            }
        }

        /*function used to calculate the depth of each pixel using the techniques of zbuffer and plan equation*/
        void depthZubberOperation(std::vector<Triangle> &triangles) {
            /*temp variable used to store the depth calculation of each point step by step*/
            double depth;
            int min_x, min_y, max_x, max_y;

            /*for each triangle*/
            for (unsigned int k = 0; k < triangles.size(); ++k) {
                /*obtain the triangle vertices*/
                Vertex* vertices = triangles[k].getVertices();

                min_x = static_cast<unsigned int>(std::max(static_cast<double>(0), std::min(std::min(vertices[0].getScreenCoordinateX(), vertices[1].getScreenCoordinateX()), vertices[2].getScreenCoordinateX())));
                min_y = static_cast<unsigned int>(std::max(static_cast < double>(0), std::min(std::min(vertices[0].getScreenCoordinateY(), vertices[1].getScreenCoordinateY()), vertices[2].getScreenCoordinateY())));
                max_x = static_cast<unsigned int>(std::min(static_cast<double>(WIDTH - 1), std::max(std::max(vertices[0].getScreenCoordinateX(), vertices[1].getScreenCoordinateX()), vertices[2].getScreenCoordinateX())) + .5f);
                max_y = static_cast<unsigned int>(std::min(static_cast<double>(HEIGHT - 1), std::max(std::max(vertices[0].getScreenCoordinateY(), vertices[1].getScreenCoordinateY()), vertices[2].getScreenCoordinateY())) + .5f);

                /*for each point in the matrix*/
                for (int i = min_y; i <= max_y; ++i) {
                    for (int j = min_x; j <= max_x; ++j) {
                            /*if the actual point is contained in the actual triangle using the edge function for each side in fact i order the triangle vertices in
                            clockwise order otherwise the technique of edge function doesn't work*/
                            if (this->edgeFunction(vertices[1], vertices[2], j, i) >= 0 &&
                                this->edgeFunction(vertices[2], vertices[0], j, i) >= 0 &&
                                this->edgeFunction(vertices[0], vertices[1], j, i) >= 0) {
                                /*calculate the depth of the actual point using the strategy of the plan equation between the triangle vertices*/
                                depth = this->planEquationTriangle(vertices[0].getScreenCoordinateX(),
                                                                   vertices[0].getScreenCoordinateY(),
                                                                   vertices[0].getScreenCoordinateZ(),
                                                                   vertices[1].getScreenCoordinateX(),
                                                                   vertices[1].getScreenCoordinateY(),
                                                                   vertices[1].getScreenCoordinateZ(),
                                                                   vertices[2].getScreenCoordinateX(),
                                                                   vertices[2].getScreenCoordinateY(),
                                                                   vertices[2].getScreenCoordinateZ(),
                                                                   j, i);
                                /*in this moment it can be possible store the vertex proprierties but for this assignments is not necessary, in case i will store them in the future assignments*/
                                /*use the zbuffer strategy to show only the parts of objects more near and hide the part of more far objects*/
                                mutex[WIDTH * i + j].lock();
                                if (this->zbuffer[WIDTH * i + j] > depth) {
                                    this->zbuffer[WIDTH * i + j] = depth;
                                    Vertex v;
                                    v.setScreenCoordinates(static_cast<double> (i), static_cast<double> (j), depth);
                                    /*in case here, i can store the number of triangle more near to future features*/
                                    /*update the matrix position with the first decimal of the relative zbuffer position using the strategy pattern*/
                                    this->screen[WIDTH * i + j] = this->shader(v);
                                }
                                mutex[WIDTH * i + j].unlock();
                            }
                        }
                    }
                }
        }

        /*function used to print screen as in the Moodle page with character '.' for empty cell and number in the valid cell and a border composed by symbols: '|' '+' '-'*/
        void print() {
            std::cout << std::endl;
            for (int i = 0; i < HEIGHT + 2; ++i) {
                for (int j = 0; j < WIDTH + 2; ++j) {
                    if ((i == 0 || i == HEIGHT + 1) && j == 0) {
                        std::cout << "+";
                    } else if ((i == 0 || i == HEIGHT + 1) && j == WIDTH + 1) {
                        std::cout << "+" << std::endl;
                    } else if ((i == 0 || i == HEIGHT + 1) && j > 0 && j < WIDTH + 1) {
                        std::cout << "-";
                    } else if ((i > 0 && i < HEIGHT + 1) && j == 0) {
                        std::cout << "|";
                    } else if ((i > 0 && i < HEIGHT + 1) && j == WIDTH + 1) {
                        std::cout << "|" << std::endl;
                    } else {
                        std::cout << this->screen[WIDTH * (i - 1) + (j - 1)];
                    }
                }
            }
        }

    private:
        /*various definitions*/
        double *projectionMatrix;
        Target_t *screen;
        double *zbuffer;
        Shader shader;
        std::mutex *mutex;

        /*function used to create the edge function, a simple method to know if a point (x,y) is below or above of the segment created by the two input parameter vertices */
        inline double edgeFunction(Vertex vertex1, Vertex vertex2, double x, double y) {
            return (vertex2.getScreenCoordinateX() - vertex1.getScreenCoordinateX()) *
                   (y - vertex1.getScreenCoordinateY()) -
                   (vertex2.getScreenCoordinateY() - vertex1.getScreenCoordinateY()) *
                   (x - vertex1.getScreenCoordinateX());
        }

        /*function used to calculate the plan equation between three input point and it returns the z-coordinate of another point(x,y,unknown) , into this plan, that are known only the x and y coordinates */
        inline double planEquationTriangle(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2,
                                           double z2, double x, double y) {
            /*in this function parameters i pass the double coordinates because if i pass the complete vertex (more nice), i must call the getter method of all coordinates
            and the formula becomes illegible*/
            return (((x - x0) * (y1 - y0) * (z2 - z0) + (y - y0) * (z1 - z0) * (x2 - x0) -
                     (y - y0) * (x1 - x0) * (z2 - z0) -
                     (x - x0) * (z1 - z0) * (y2 - y0)) / ((-1) * (x1 - x0) * (y2 - y0) + (y1 - y0) * (x2 - x0))) + z0;
        }
    };

}

#endif
