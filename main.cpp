#include "Vertex.h"
#include "Triangle.h"
#include "OperationRasterizer.h"
#include "ValueProperties.h"
#include "ReadFile.h"
#include "Thread.h"

using namespace rasterPipeline;

struct Shader {
    char operator() (Vertex& v) {
        double aux = v.getScreenCoordinateZ() - static_cast<int>(v.getScreenCoordinateZ());  // I take only decimal part
        return static_cast<int>(aux * 10) + '0';
    }
};



int main() {

    /*define a mutex matrix to manage the concurrency in zbuffer*/
    std::mutex mutex[HEIGHT * WIDTH];

    /*define screen as char type as in Moodle example to make the same example*/
    char screen[HEIGHT * WIDTH];
    /*initialize screen with all '.' symbols*/
    std::fill_n(screen, HEIGHT * WIDTH, PRINTED_CHARACTER_INVALID_CELL);

    /*define zbuffer for managing the different depth between all triangles, double to make more precision */
    double zbuffer[HEIGHT * WIDTH];
    /*initialize all positions of zbuffer to +infinity*/
    std::fill_n(zbuffer, HEIGHT * WIDTH, DBL_MAX);

    /*define the projection matrix*/
    double projectionMatrix[NUMBER_PROJECTION_MATRIX * NUMBER_PROJECTION_MATRIX];

    /*define the operation class object used to manipulate triangles*/
    OperationRasterizer<char, Shader> operation;
    /*create shader object*/
    Shader shader;
    /*include shader object*/
    operation.setShader(shader);
    /*import mutex into operation class object*/
    operation.setMutex(mutex);
    /*import screen into operation class object*/
    operation.setScreen(screen);
    /*import zbuffer into operation class object*/
    operation.setZbuffer(zbuffer);
    /*import projection matrix into operation class object*/
    operation.setProjectionMatrix(projectionMatrix);
    /*calculate the perspectiveProjection*/
    operation.perspectiveProjection();

    /*create a vector of triangles to store the triangles in the input file*/
    std::vector<Triangle> trianglesFile;
    /*create the file object to import the input file*/
    std::ifstream file;
    /*create the read file object*/
    ReadFile ro;
    /*try to open the file*/
    ro.openFile(file);
    /*read the input file*/
    ro.readFile(file, trianglesFile);

    /*create the object for using threads*/
    Parallelization<char, Shader> parall(operation, trianglesFile);
    /*do rendering*/
    parall.rendering();

    return 0;
}