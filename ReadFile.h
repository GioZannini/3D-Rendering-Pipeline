#ifndef ASSIGNMENT3_READFILE_H
#define ASSIGNMENT3_READFILE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <regex>
#include "ValueProperties.h"
#include "Vertex.h"
#include "Triangle.h"



namespace rasterPipeline {

/*found information about obj file composition in: https://all3dp.com/1/obj-file-format-3d-printing-cad/
 *found information obout the string separator ',' in: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/ */
    class ReadFile {
    public:

        /*function used to transform a string to double and check if there are some invalid characters*/
        double stringToDouble(std::string word) {
            /*create regexp for decimal number with dot*/
            std::regex reg("[-]?[0-9]*[.]?[0-9]*");
            if (!regex_match(word, reg)) {
                std::cout << std::endl << "ERRORE: Caratteri non ammessi nella conversione string to double" << std::endl;
                exit(1);
            }
            return std::stod(word);
        }

        /*function used to create a vertex object from input file*/
        Vertex vInitilization(std::stringstream &ss) {
            /*create the parameters to create a vertex object and a temp double*/
            double x, y, z, temp;
            /*initialize cont equal zero to verify the correct number of parameters*/
            int cont = 0;
            /*create the variable for the string stream*/
            std::string word;
            /*while exists another word*/
            while (ss >> word) {
                cont++;
                /*convert the actual string in number*/
                temp = stringToDouble(word);
                /*assign vertex coordinates*/
                if (cont == 1) {
                    x = temp;
                } else if (cont == 2) {
                    y = temp;
                } else if (cont == 3) {
                    z = temp;
                }
            }
            /*check that cont must equal 3 otherwise parameter number is wrong*/
            if (cont != 3) {
                std::cout << std::endl << "ERRORE: Numero parametri errato nelle coordinate del vertice" << std::endl;
                exit(1);
            }
            /*print all in output to verify if data are correct*/
            std::cout << "v)  " << x << " " << y << " " << z << std::endl;
            /*now i create the vertex object, set it the input values and return it*/
            Vertex vertex;
            vertex.setCoordinates(x, y, z);
            return vertex;
        }

        /*function used to create a vertex texture object from input file*/
        Vertex vtInitilization(std::stringstream &ss) {
            /*create the parameters to create a vertex texture object and a temp double*/
            double u, v, temp;
            /*initialize cont equal zero to verify the correct number of parameters*/
            int cont = 0;
            /*create the variable for the string stream*/
            std::string word;
            /*while exists another word*/
            while (ss >> word) {
                cont++;
                /*convert the actual string in number*/
                temp = stringToDouble(word);
                /*assign vertex texture coordinates*/
                if (cont == 1) {
                    u = temp;
                } else if (cont == 2) {
                    v = temp;
                }
            }
            /*check that cont must equal 2 otherwise parameter number is wrong*/
            if (cont != 2) {
                std::cout << std::endl << "ERRORE: Numbero parametri errato nelle texture del vertice" << std::endl;
                exit(1);
            }
            /*print all in output to verify if data are correct*/
            std::cout << "vt) " << u << " " << v << std::endl;
            /*now i create the vertex object, set it the input values and return it*/
            Vertex vertex;
            vertex.setTextureCoordinates(u, v);
            return vertex;
        }

        /*function used to create a vertex normalized object from input file*/
        Vertex vnInitilization(std::stringstream &ss) {
            /*create the parameters to create a vertex normalized object and a temp double*/
            double nx, ny, nz, temp;
            /*initialize cont equal zero to verify the correct number of parameters*/
            int cont = 0;
            /*create the variable for the string stream*/
            std::string word;
            /*while exists another word*/
            while (ss >> word) {
                cont++;
                /*convert the actual string in number*/
                temp = stringToDouble(word);
                /*assign vertex normalized coordinates*/
                if (cont == 1) {
                    nx = temp;
                } else if (cont == 2) {
                    ny = temp;
                } else if (cont == 3) {
                    nz = temp;
                }
            }
            /*check that cont must equal 3 otherwise parameter number is wrong*/
            if (cont != 3) {
                std::cout << std::endl << "ERRORE: Numbero parametri errato nelle coordinate normalizzate del vertice" << std::endl;
                exit(1);
            }
            /*print all in output to verify if data are correct*/
            std::cout << "vn) " << nx << " " << ny << " " << nz << std::endl;
            /*now i create the vertex object, set it the input values and return it*/
            Vertex vertex;
			/* We could create for each vertex other fields to store the orthogonal vector to compute the illumination, but in this assigment this isn't necessary*/
            return vertex;
        }

        /*function used to create an association between all vertex characteristics from input file*/
        void fInitilization(std::stringstream &ss, int numberVertices, int numberTextures, int numberNormalizedCoordinates, std::vector<Triangle> &triangles,
                            std::vector<Vertex> coordinates, std::vector<Vertex> textureCoordinates, std::vector<Vertex> normalizedCoordinates) {
            /*create the parameters to create a triangle object and a temp object*/
            double coordinatesNumber[NUMBER_CHARACTERISTICS_VERTEX_TRIANGLE] = {0};
            double textureCoordinatesNumber[NUMBER_CHARACTERISTICS_VERTEX_TRIANGLE] = {0};
            double normalizedCooordinatesNumber[NUMBER_CHARACTERISTICS_VERTEX_TRIANGLE] = {0};
            /*create the variable for the string stream*/
            std::string word;
            /*create array to store the splitted values*/
            std::string splittedValues[NUMBER_DIFFERENT_MEASURES];
            /*initialize cont equal zero to save in correct way the number in the many array*/
            int cont = 0;
            /*variable used to store the number of splitted value*/
            int contSplittedValues;
            /*create a triangle object to insert its fields using the f function*/
            Triangle triangle;
            /*while exists another word in the f row and until they are in the correct position*/
            while (ss >> word && cont < NUMBER_CHARACTERISTICS_VERTEX_TRIANGLE) {
                /*split the actual word to extract the three info from the f function parameters and save the count number*/
                contSplittedValues = split(word, '/', splittedValues);
                /*if count of splitted value are different from 3 error*/
                if (contSplittedValues != 3) {
                    std::cout << std::endl << "ERRORE: Numbero parametri errato nella funzione f" << std::endl;
                    exit(1);
                }/*if the input key in f function are wrong*/
                else if (!(stringToDouble(splittedValues[0]) >= 1 && stringToDouble(splittedValues[0]) <= numberVertices &&
                           stringToDouble(splittedValues[1]) >= 1 && stringToDouble(splittedValues[1]) <= numberTextures &&
                           stringToDouble(splittedValues[2]) >= 1 && stringToDouble(splittedValues[2]) <= numberNormalizedCoordinates)) {
                    std::cout << std::endl << "ERRORE: Non esistono gli oggetti riferiti nella funzione f" << std::endl;
                    exit(1);
                }
                coordinatesNumber[cont] = stringToDouble(splittedValues[0]);
                textureCoordinatesNumber[cont] = stringToDouble(splittedValues[1]);
                normalizedCooordinatesNumber[cont] = stringToDouble(splittedValues[2]);
                cont++;
            }
            /*check that cont must equal 3 otherwise parameter number is wrong*/
            if (cont != 3) {
                std::cout << std::endl << "ERRORE: Numbero parametri errato nella funzione f" << std::endl;
                exit(1);
            }
            /*mapping triangles coordinates, texture coordinates and normalized coordinates using the parameters of f function*/
            triangle.setVertexCoordinates(coordinates[static_cast<int>(coordinatesNumber[0] - 1.0)], coordinates[static_cast<int>(coordinatesNumber[1] - 1.0)], coordinates[static_cast<int>(coordinatesNumber[2] - 1.0)]);
            /*set normalized coordinates in a for loop because a single vertices can have already from the input file and in another we must calculate*/
            for (int i = 0; i < NUMBER_CHARACTERISTICS_VERTEX_TRIANGLE; ++i) {
                /*only if is set the normalized coordinates for actual vertex then set it*/
                if (coordinatesNumber[i] > 0) {
                    triangle.setVertexNormalizedCoordinates(normalizedCoordinates[static_cast<int>(normalizedCooordinatesNumber[i] - 1.f)], i);
                }
            }
            triangle.setVertexTextureCoordinates(textureCoordinates[static_cast<int>(textureCoordinatesNumber[0] - 1.0)], textureCoordinates[static_cast<int>(textureCoordinatesNumber[1] - 1.0)], textureCoordinates[static_cast<int>(textureCoordinatesNumber[2] - 1.0)]);
            /*insert the triangle object in the vector*/
            triangles.push_back(triangle);
            /*print all in output to verify if data are correct*/
            std::cout << "f) ";
            for (int i = 0; i < NUMBER_CHARACTERISTICS_VERTEX_TRIANGLE; ++i) {
                std::cout << " " << coordinatesNumber[i] << "/" << textureCoordinatesNumber[i] << "/" << normalizedCooordinatesNumber[i];
            }
            std::cout << std::endl;
        }

        /*function used to create vertex and triangle objects from an input file, but in this function are only developed: 'v', 'vt', 'vn', 'f'*/
        void readFile(std::ifstream &file, std::vector<Triangle> &triangles) {
            /*vector used to store the vertices, the textures, the normalized coordinates*/
            std::vector<Vertex> coordinates;
            std::vector<Vertex> textures;
            std::vector<Vertex> normalizedCoordinates;
            /*variables used to store the number of vertices, textures and normalized coordinates that they are in the input file*/
            int numberVertices = 0, numberTextures = 0, numberNormalizedCoordinates = 0;
            /*string to store the current line*/
            std::string line;
            /*use the debug mode to see if all data are correct*/
            std::cout << std::endl << "INPUT FILE:" << std::endl;
            /*while the file is not finished*/
            while (file.good()) {
                /*take the current line and create the string stream object*/
                std::getline(file, line);
                std::stringstream ss(line);
                std::string word;
                /*split actual row using the space character and get the first word*/
                ss >> word;
                /*if this line represents a vertex*/
                if (!word.compare("v")) {
                    coordinates.push_back(vInitilization(ss));
                    numberVertices++;
                } else if (!word.compare("vt")) {
                    textures.push_back(vtInitilization(ss));
                    numberTextures++;
                } else if (!word.compare("vn")) {
                    normalizedCoordinates.push_back(vnInitilization(ss));
                    numberNormalizedCoordinates++;
                } else if (!word.compare("f")) {
                    /*also pass this variables to check if the input data are correct*/
                    fInitilization(ss, numberVertices, numberTextures, numberNormalizedCoordinates, triangles, coordinates, textures, normalizedCoordinates);
                }
            }
            std::cout << std::endl;
        }

        void openFile(std::ifstream &file) {
            file.open(FILE_SOURCE);
            /*if file is open*/
            if (!file.is_open()) {
                std::cout << std::endl << "ERRORE: Impossibile aprire il file" << std::endl;
                exit(1);
            }
        }

    private:

        /*function used to split a string using a determined character in input*/
        int split(std::string s, char delimiter, std::string splittedValues[NUMBER_DIFFERENT_MEASURES]) {
            /*count number of divider character in case they are wrong error*/
            int cont = 0;
            for (int i = 0; s[i] != '\0'; ++i) {
                if (s[i] == delimiter) {
                    cont++;
                }
            }
            /*check if cont equal or 1 or 2 -> 1/2 or 1/2/3 because the normalized coordinates i can calculate after*/
            if (cont != 1 && cont != 2) {
                std::cout << std::endl << "ERRORE: Numero parametri errato nei riferimenti all'interno della funzione f" << std::endl;
                exit(1);
            }
            /*create the string stream and the temporary token*/
            std::istringstream tokenStream(s);
            std::string token;
            /*initialize cont equal zero to save in the right order the input indexes*/
            cont = 0;
            /*store each splitted value in the array*/
            while (std::getline(tokenStream, token, delimiter)) {
                splittedValues[cont] = token;
                cont++;
            }
            /*return number of splitted value*/
            return cont;
        }

    };

}

#endif
