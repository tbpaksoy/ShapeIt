#ifndef XML
#define XML
#include "../include/tinyxml2.h"
#include "../include/tinyxml2.cpp"
#endif
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <any>
#include <string>
#include <map>
#include <functional>
#include <variant>

#ifndef ANALYZE_H
#define ANALYZE_H

typedef std::map<std::string, std::variant<std::vector<glm::vec3>, std::vector<int>, std::vector<glm::vec2>>> MeshData;

void AnalyzeTag(tinyxml2::XMLElement *element, MeshData &data);

glm::vec3 *BoxPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *CirclePositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *CylinderPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *ConePositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *ArrayPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *PrismPositionalData(tinyxml2::XMLElement *element, int &size);

glm::vec3 *AddColorData(int size, glm::vec3 color);
glm::vec3 *AddColorData(int size, tinyxml2::XMLElement *element);
glm::vec3 *AddParametricColorData(int size, std::function<glm::vec3(glm::vec3)> colorFunction, glm::vec3 *positonalData = nullptr);

int *BoxIndexData(tinyxml2::XMLElement *element, int &size);
int *CircleIndexData(tinyxml2::XMLElement *element, int &size);
int *CylinderIndexData(tinyxml2::XMLElement *element, int &size);
int *ConeIndexData(tinyxml2::XMLElement *element, int &size);
int *ArrayIndexData(tinyxml2::XMLElement *element, int &size);
int *PrismIndexData(tinyxml2::XMLElement *element, int &size);

glm::vec3 *Translate(glm::vec3 *input, int size, glm::vec3 offset);
glm::vec3 *Rotate(glm::vec3 *input, int size, glm::vec3 rotation);
glm::vec3 *Rotate(glm::vec3 *input, int size, glm::quat rotaion);
glm::vec3 *Scale(glm::vec3 *input, int size, glm::vec3 scale);

glm::vec3 *ApplyTranforms(tinyxml2::XMLElement *element, glm::vec3 *input, int size);

#endif