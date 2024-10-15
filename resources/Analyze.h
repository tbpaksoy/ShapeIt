#define ARCHITECTURE
#define ADVANCED

// En : This file contains the function prototypes for the functions that analyze the XML file and extract the data from it.
// Tr : Bu dosya, XML dosyasını analiz eden ve içinden verileri çıkaran fonksiyonların prototiplerini içerir.

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
#include <functional>

#ifndef ANALYZE_H
#define ANALYZE_H

// TODO: Consider to add new tags.

// En : Type definion for the data collected from the XML file.
// Tr : XML dosyasından toplanan veriler için tip tanımı.
typedef std::map<std::string, std::variant<std::vector<glm::vec3>, std::vector<int>, std::vector<glm::vec2>>> MeshData;

// En : The function that will analyze the XML file and extract or overwrite the data.
// Tr : XML dosyasını analiz edecek ve verileri çıkaracak veya üzerine yazacak olan fonksiyon.
void AnalyzeTag(tinyxml2::XMLElement *element, MeshData &data);

// En : The functions that will postional data from the related XML tags.
// Tr : İlgili XML etiketlerinden pozisyon verilerini alacak olan fonksiyonlar.
glm::vec3 *BoxPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *CirclePositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *CylinderPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *ConePositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *ArrayPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *PrismPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *ImportPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *ParametricPositionalData(tinyxml2::XMLElement *element, int &size);

// En : The functions that will add color data.
// Tr : Renk verilerini ekleyecek olan fonksiyonlar.
glm::vec3 *AddColorData(int size, glm::vec3 color);
glm::vec3 *AddColorData(int size, tinyxml2::XMLElement *element);
glm::vec3 *AddParametricColorData(int size, std::function<glm::vec3(glm::vec3)> colorFunction, glm::vec3 *positonalData = nullptr);

// En : The functions that will generate the index data from the related XML tags.
// Tr : İlgili XML etiketlerinden index verilerini oluşturacak olan fonksiyonlar.
int *BoxIndexData(tinyxml2::XMLElement *element, int &size);
int *CircleIndexData(tinyxml2::XMLElement *element, int &size);
int *CylinderIndexData(tinyxml2::XMLElement *element, int &size);
int *ConeIndexData(tinyxml2::XMLElement *element, int &size);
int *ArrayIndexData(tinyxml2::XMLElement *element, int &size);
int *PrismIndexData(tinyxml2::XMLElement *element, int &size);
int *ImportIndexData(tinyxml2::XMLElement *element, int &size);
int *ParametricIndexData(tinyxml2::XMLElement *element, int &size);

// En : Tranform functions for the positional data.
// Tr : Pozisyon verileri için dönüşüm fonksiyonları.
glm::vec3 *Translate(glm::vec3 *input, int size, glm::vec3 offset);
glm::vec3 *Rotate(glm::vec3 *input, int size, glm::vec3 rotation);
glm::vec3 *Rotate(glm::vec3 *input, int size, glm::quat rotaion);
glm::vec3 *Scale(glm::vec3 *input, int size, glm::vec3 scale);

glm::vec3 *ApplyTranforms(tinyxml2::XMLElement *element, glm::vec3 *input, int size);

std::function<glm::vec3(float, float)> CreateParametricFunction(tinyxml2::XMLElement *element);
#ifdef ADVANCED &&ARCHITECTURE

#include <variant>

typedef std::variant<float, std::string, int> ArchitecturalDataType;
typedef std::map<std::string, ArchitecturalDataType> ArchitecturalData;
#ifdef OBJECT_H
#include "Object.h"
typedef std::map<Object *, ArchitecturalData> ArchitecturalElementsData;
#else
typedef std::map<std::string, ArchitecturalData> ArchitecturalElementsData;
#endif

ArchitecturalData GlobalArchitecturalData;

// TODO: Add support for walls, columns, beams, stairs, doors, windows, balcons, terraces.

enum ColumnType
{
    CIRCLE,
    BOX
};

enum WayOrientation
{
    LeftInside,
    RightInside,
    Inside
};

glm::vec3 *WallPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *ColumnPositionalData(tinyxml2::XMLElement *element, int &size);
glm::vec3 *BeamPositionalData(tinyxml2::XMLElement *elememnt, int &size);
glm::vec3 *StairPositionalData(tinyxml2::XMLElement *elememnt, int &size);
glm::vec3 *DoorPositionalData(tinyxml2::XMLElement *elememnt, int &size);
glm::vec3 *WindowPositionalData(tinyxml2::XMLElement *elememnt, int &size);
glm::vec3 *BalconyPositionalData(tinyxml2::XMLElement *elememnt, int &size);
glm::vec3 *TerracePositionalData(tinyxml2::XMLElement *elememnt, int &size);

int *WallIndexData(tinyxml2::XMLElement *element, int &size);
int *ColumnIndexData(tinyxml2::XMLElement *element, int &size);
int *BeamIndexData(tinyxml2::XMLElement *elememnt, int &size);
int *StairIndexData(tinyxml2::XMLElement *elememnt, int &size);
int *DoorIndexData(tinyxml2::XMLElement *elememnt, int &size);
int *WindowIndexData(tinyxml2::XMLElement *elememnt, int &size);
int *BalconyIndexData(tinyxml2::XMLElement *elememnt, int &size);
int *TerraceIndexData(tinyxml2::XMLElement *elememnt, int &size);

#endif
#endif