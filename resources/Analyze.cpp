// En : Definitions of Analyze.h file.
// Tr : Analyze.h dosyasının tanımlamaları.
#include "Analyze.h"
#include <stack>
#include <iostream>
#include <math.h>
#include <exception>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <sstream>
#include <numeric>

#ifdef OBJECT_H
#include "Object.h"
#endif

#define PI 3.14159265358979323846f

// En : Variables only will be used in this file.
// Tr : Bu dosyada sadece kullanılacak olan değişkenler.
const std::string definedTags[] =
    {
        "box",
        "circle",
        "cylinder",
        "cone",
        "array",
        "prism",
        "import"};
const std::map<std::string, std::function<glm::vec3 *(tinyxml2::XMLElement *, int &)>> positionalHandlers =
    {
        {"box", BoxPositionalData},
        {"circle", CirclePositionalData},
        {"cylinder", CylinderPositionalData},
        {"cone", ConePositionalData},
        {"array", ArrayPositionalData},
        {"prism", PrismPositionalData},
        {"import", ImportPositionalData}};
const std::map<std::string, std::function<int *(tinyxml2::XMLElement *, int &)>> indexHandlers =
    {
        {"box", BoxIndexData},
        {"circle", CircleIndexData},
        {"cylinder", CylinderIndexData},
        {"cone", ConeIndexData},
        {"array", ArrayIndexData},
        {"prism", PrismIndexData},
        {"import", ImportIndexData}};
const std::string elementTag[] =
    {
        "vertex",
        "edge",
        "face",
        "index"};
/* IDEA: Add multiple compiling options. (21.09.2024)
 IDEA: Add SIMPLE compiling option. (21.09.2024)
 IDEA: Add ADVANCED compiling option. (21.09.2024)*/

void AnalyzeTag(tinyxml2::XMLElement *element, MeshData &data)
{
    // En : Data definitons.
    // Tr : Veri tanımları.
    data = MeshData();
    data["position"] = std::vector<glm::vec3>();
    data["index"] = std::vector<int>();
    data["color"] = std::vector<glm::vec3>();

    std::vector<glm::vec3> &position = std::get<std::vector<glm::vec3>>(data["position"]);
    std::vector<int> &index = std::get<std::vector<int>>(data["index"]);
    std::vector<glm::vec3> &color = std::get<std::vector<glm::vec3>>(data["color"]);

#ifdef OBJECT_H &ADVANCED
    int elementSize = 0;
    std::map<tinyxml2::XMLElement *, Object *> objects;
#endif

    // En : Stack for the depth first search.
    // Tr : Derinlik öncelikli arama için yığın.
    std::stack<tinyxml2::XMLElement *> stack;
    stack.push(element);
    while (stack.size())
    {
        tinyxml2::XMLElement *current = stack.top();
        stack.pop();
        for (tinyxml2::XMLElement *sub = current->FirstChildElement(); sub != nullptr; sub = sub->NextSiblingElement())
        {
            // En : Get the tag and lower its name.
            // Tr : Etiketi al ve ismini küçült.
            std::string tag = sub->Name();
            std::transform(tag.begin(), tag.end(), tag.begin(), tolower);

            // En : If the tag is predefined, extract the data else push to the stack.
            // Tr : Eğer etiket önceden tanımlanmışsa, veriyi çıkar aksi halde yığına ekle.
            if (std::find(std::begin(definedTags), std::end(definedTags), tag) != std::end(definedTags))
            {
                // En : Get the positional data and apply the transforms. If object model is defined, create the object.
                // Tr : Pozisyon verilerini al ve dönüşümleri uygula. Eğer nesne modeli tanımlanmışsa, nesneyi oluştur.

                // En : Step 1 : Assign the needed variables.
                // Tr : Adım 1 : Gerekli değişkenleri ata.
                int size;
#ifdef OBJECT_H
                int begin = position.size();
#endif
                int lastIndex = index.size() ? *std::max_element(index.begin(), index.end()) + 1 : 0;

                // En : Step 2 : Get the positional data and apply the transforms.
                // Tr : Adım 2 : Pozisyon verilerini al ve dönüşümleri uygula.
                glm::vec3 *_position = positionalHandlers.at(tag)(sub, size);
                _position = ApplyTranforms(sub, _position, size);
                for (int i = 0; i < size; i++)
                    position.push_back(_position[i]);

#ifdef OBJECT_H
                // En : Optional Step : Create the an object.
                // Tr : İsteğe bağlı Adım : Bir nesne oluştur.
                int end = position.size();
                Object *object;
#ifdef SIMPLE
                object = new Object(begin, end);
#elif defined(ADVANCED)
                std::string name;
                if (sub->Attribute("name"))
                    name = sub->Attribute("name");
                else
                    name = "Object#" + std::to_string(elementSize++);

                Object *parent = nullptr;
                if (sub->Parent() && std::find(objects.begin(), objects.end(), sub->Parent()->ToElement()) != objects.end())
                    parent = objects[sub->Parent()->ToElement()];

                object = new Object(begin, end, name, parent);
#endif
                objects[sub] = object;
#endif
                // En : Step 3 : Get the color data.
                // Tr : Adım 3 : Renk verilerini al.
                glm::vec3 *_color = AddColorData(size, sub);
                for (int i = 0; i < size; i++)
                    color.push_back(_color[i]);

                // En : Step 4 : Get the index data.
                // Tr : Adım 4 : Index verilerini al.
                int *_index = indexHandlers.at(tag)(sub, size);
                for (int i = 0; i < size; i++)
                    index.push_back(_index[i] + lastIndex);
            }
            else
            {
                // En : If the tag is not predefined, push to the stack.
                // Tr : Eğer etiket önceden tanımlanmamışsa, yığına ekle.
                stack.push(sub);
            }
        }
    }
}

glm::vec3 *BoxPositionalData(tinyxml2::XMLElement *element, int &size)
{
    float x = 1.0f, y = 1.0f, z = 1.0f;
    if (element->Attribute("edge"))
        try
        {
            x = y = z = std::stof(element->Attribute("edge"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid edge value : " << element->Attribute("edge") << std::endl;
        }
    else
    {
        if (element->Attribute("width"))
            try
            {
                x = std::stof(element->Attribute("width"));
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: invalid x value : " << element->Attribute("x") << std::endl;
            }
        if (element->Attribute("height"))
            try
            {
                y = std::stof(element->Attribute("height"));
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: invalid y value : " << element->Attribute("y") << std::endl;
            }
        if (element->Attribute("depth"))
            try
            {
                z = std::stof(element->Attribute("depth"));
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: invalid z value : " << element->Attribute("depth") << std::endl;
            }
    }

    glm::vec3 *position = new glm::vec3[8];

    position[0] = glm::vec3(-x / 2, -y / 2, -z / 2);
    position[1] = glm::vec3(-x / 2, -y / 2, z / 2);
    position[2] = glm::vec3(-x / 2, y / 2, -z / 2);
    position[3] = glm::vec3(-x / 2, y / 2, z / 2);
    position[4] = glm::vec3(x / 2, -y / 2, -z / 2);
    position[5] = glm::vec3(x / 2, -y / 2, z / 2);
    position[6] = glm::vec3(x / 2, y / 2, -z / 2);
    position[7] = glm::vec3(x / 2, y / 2, z / 2);
    size = 8;
    return position;
}
glm::vec3 *CirclePositionalData(tinyxml2::XMLElement *element, int &size)
{
    float radius = 1.0f;
    if (element->Attribute("radius"))
        try
        {
            radius = std::stof(element->Attribute("radius"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid radius value : " << element->Attribute("radius") << std::endl;
        }
    int resolution = 32;
    if (element->Attribute("resolution"))
        try
        {
            resolution = std::stoi(element->Attribute("resolution"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid resolution value : " << element->Attribute("resolution") << std::endl;
        }
    glm::vec3 *position = new glm::vec3[resolution];
    for (int i = 0; i < resolution; i++)
    {
        float angle = 2 * PI * i / resolution;
        position[i] = glm::vec3(radius * cos(angle), 0, radius * sin(angle));
    }
    size = resolution;
    return position;
}
glm::vec3 *CylinderPositionalData(tinyxml2::XMLElement *element, int &size)
{
    float radius = 1.0f, height = 1.0f;
    if (element->Attribute("radius"))
        try
        {
            radius = std::stof(element->Attribute("radius"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid radius value : " << element->Attribute("radius") << std::endl;
        }
    if (element->Attribute("height"))
        try
        {
            height = std::stof(element->Attribute("height"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid height value : " << element->Attribute("height") << std::endl;
        }
    int resolution = 32;
    if (element->Attribute("resolution"))
        try
        {
            resolution = std::stoi(element->Attribute("resolution"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid resolution value : " << element->Attribute("resolution") << std::endl;
        }
    size = 2 * resolution + 2;
    glm::vec3 *position = new glm::vec3[size];
    for (int i = 0; i < resolution; i++)
    {
        float angle = 2 * PI * i / resolution;
        float x = radius * cos(angle), z = radius * sin(angle);
        position[i] = glm::vec3(x, -height / 2, z);
        position[i + resolution] = glm::vec3(x, height / 2, z);
    }
    position[size - 1] = glm::vec3(0, -height / 2, 0);
    position[size - 2] = glm::vec3(0, height / 2, 0);
    return position;
}
glm::vec3 *ConePositionalData(tinyxml2::XMLElement *element, int &size)
{
    float radius = 1.0f, height = 1.0f;
    if (element->Attribute("radius"))
        try
        {
            radius = std::stof(element->Attribute("radius"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid radius value : " << element->Attribute("radius") << std::endl;
        }
    if (element->Attribute("height"))
        try
        {
            height = std::stof(element->Attribute("height"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid height value : " << element->Attribute("height") << std::endl;
        }
    int resolution = 32;
    if (element->Attribute("resolution"))
        try
        {
            resolution = std::stoi(element->Attribute("resolution"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid resolution value : " << element->Attribute("resolution") << std::endl;
        }
    glm::vec3 *position = new glm::vec3[resolution + 2];
    for (int i = 0; i < resolution; i++)
    {
        float angle = 2 * PI * i / resolution;
        position[i] = glm::vec3(radius * cos(angle), -height / 2, radius * sin(angle));
    }
    position[resolution] = glm::vec3(0, height / 2, 0);
    position[resolution + 1] = glm::vec3(0, -height / 2, 0);
    size = resolution + 2;
    return position;
}
glm::vec3 *ArrayPositionalData(tinyxml2::XMLElement *element, int &size)
{
    std::vector<glm::vec3> position;
    for (tinyxml2::XMLElement *sub = element->FirstChildElement(); sub != nullptr; sub = sub->NextSiblingElement())
    {
        std::string tag = sub->Name();
        std::transform(tag.begin(), tag.end(), tag.begin(), tolower);
        if (tag == "vertex")
        {
            float x = 0, y = 0, z = 0;
            if (sub->Attribute("x"))
                try
                {
                    x = std::stof(sub->Attribute("x"));
                }
                catch (const std::exception &e)
                {
                    std::cout << "Error: invalid x value : " << sub->Attribute("x") << std::endl;
                }
            if (sub->Attribute("y"))
                try
                {
                    y = std::stof(sub->Attribute("y"));
                }
                catch (const std::exception &e)
                {
                    std::cout << "Error: invalid y value : " << sub->Attribute("y") << std::endl;
                }
            if (sub->Attribute("z"))
                try
                {
                    z = std::stof(sub->Attribute("z"));
                }
                catch (const std::exception &e)
                {
                    std::cout << "Error: invalid z value : " << sub->Attribute("z") << std::endl;
                }
            position.push_back(glm::vec3(x, y, z));
        }
    }
    size = position.size();
    glm::vec3 *positionArray = position.data();
    return positionArray;
}
glm::vec3 *PrismPositionalData(tinyxml2::XMLElement *element, int &size)
{
    // FIXME: Prism positional data is not correct. (6.10.2024)
    float height = 1.0;
    if (element->Attribute("height"))
    {
        height = std::stof(element->Attribute("height"));
    }
    std::vector<glm::vec2> points;
    for (tinyxml2::XMLElement *sub = element->FirstChildElement(); sub; sub = sub->NextSiblingElement())
    {
        float x = 0, y = 0;
        if (sub->Attribute("x"))
            x = std::stof(sub->Attribute("x"));
        if (sub->Attribute("y"))
            y = std::stof(sub->Attribute("y"));
        points.push_back(glm::vec2(x, y));
    }
    size = points.size() * 2;
    glm::vec3 *result = new glm::vec3[size];
    for (int i = 0; i < points.size(); i++)
    {
        result[i] = glm::vec3(points[i].x, height / 2, points[i].y);
        result[i] = glm::vec3(points[i].x, -height / 2, points[i].y);
    }
    return result;
}
glm::vec3 *ImportPositionalData(tinyxml2::XMLElement *element, int &size)
{
    // TODO: Implement the import positional data. (7.10.2024)
}

glm::vec3 *AddColorData(int size, glm::vec3 color)
{
    glm::vec3 *output = new glm::vec3[size];
    for (int i = 0; i < size; i++)
        output[i] = color;
    return output;
}
glm::vec3 *AddColorData(int size, tinyxml2::XMLElement *element)
{
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 *output = new glm::vec3[size];
    if (element->Attribute("color"))
    {
        try
        {
            float r = 0, g = 0, b = 0;
            int _r = 0, _g = 0, _b = 0;

            std::string s = element->Attribute("color");

            std::stringstream ss;

            ss << s.substr(0, 2);
            ss >> std::hex >> _r;
            r = (float)_r / 255.0f;
            ss.clear();

            ss << s.substr(2, 2);
            ss >> std::hex >> _g;
            g = (float)_g / 255.0f;
            ss.clear();

            ss << s.substr(4, 2);
            ss >> std::hex >> _b;
            b = (float)_b / 255.0f;
            ss.clear();

            color = glm::vec3(r, g, b);
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid color value : " << element->Attribute("color") << ". Accepted as 0x000000." << std::endl;
        }
    }
    for (int i = 0; i < size; i++)
        output[i] = color;
    return output;
}
glm::vec3 *AddParametricColorData(int size, std::function<glm::vec3(glm::vec3)> colorFunction, glm::vec3 *positonalData)
{
    glm::vec3 *output = new glm::vec3[size];
    for (int i = 0; i < size; i++)
        output[i] = colorFunction(positonalData[i]);
    return output;
}

int *BoxIndexData(tinyxml2::XMLElement *element, int &size)
{
    int index[] = {
        0, 1, 3, 0, 3, 2,
        6, 4, 0, 0, 2, 6,
        4, 7, 5, 6, 7, 4,
        7, 3, 1, 1, 5, 7,
        4, 5, 0, 0, 5, 1,
        7, 6, 2, 2, 3, 7};
    size = 36;
    int *indexArray = new int[size];
    for (int i = 0; i < size; i++)
        indexArray[i] = index[i];
    return indexArray;
}
int *CircleIndexData(tinyxml2::XMLElement *element, int &size)
{
    int resolution = 32;
    if (element->Attribute("resolution"))
        try
        {
            resolution = std::stoi(element->Attribute("resolution"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid resolution value : " << element->Attribute("resolution") << std::endl;
        }
    size = 3 * resolution;
    int *index = new int[3 * resolution];
    for (int i = 0; i < resolution; i++)
    {
        index[3 * i] = i;
        index[3 * i + 1] = (i + 1) % resolution;
        index[3 * i + 2] = resolution;
    }
    return index;
}
int *CylinderIndexData(tinyxml2::XMLElement *element, int &size)
{
    int resolution = 32;
    if (element->Attribute("resolution"))
        try
        {
            resolution = std::stoi(element->Attribute("resolution"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid resolution value : " << element->Attribute("resolution") << std::endl;
        }
    size = 12 * resolution;
    int *index = new int[size];
    for (int i = 0; i < resolution; i++)
    {
        // En : Side faces.
        // Tr : Yan yüzler.
        index[12 * i] = i;
        index[12 * i + 1] = (i + 1) % resolution + resolution;
        index[12 * i + 2] = (i + 1) % resolution;
        index[12 * i + 3] = i;
        index[12 * i + 4] = i + resolution;
        index[12 * i + 5] = (i + 1) % resolution + resolution;
        // En : Bottom face.
        // Tr : Alt yüz.
        index[12 * i + 6] = i;
        index[12 * i + 7] = (i + 1) % resolution;
        index[12 * i + 8] = 2 * resolution + 1;
        // En : Top face.
        // Tr : Üst yüz.
        index[12 * i + 9] = (i + 1) % resolution + resolution;
        index[12 * i + 10] = i + resolution;
        index[12 * i + 11] = 2 * resolution;
    }
    return index;
}
int *ConeIndexData(tinyxml2::XMLElement *element, int &size)
{
    int resolution = 32;
    if (element->Attribute("resolution"))
    {
        resolution = std::stoi(element->Attribute("resolution"));
    }
    size = resolution * 6;
    int *index = new int[size];
    for (int i = 0, j = 0; j < size; i++, j += 6)
    {
        index[6 * i] = i;
        index[6 * i + 1] = (i + 1) % resolution;
        index[6 * i + 2] = resolution;

        index[6 * i + 3] = (i + 1) % resolution;
        index[6 * i + 4] = i;
        index[6 * i + 5] = resolution + 1;
    }
    return index;
}
int *ArrayIndexData(tinyxml2::XMLElement *element, int &size)
{
    std::vector<tinyxml2::XMLElement *> matchingElements;
    for (tinyxml2::XMLElement *sub = element->FirstChildElement(); sub != nullptr; sub = sub->NextSiblingElement())
    {
        std::string tag = sub->Name();
        std::transform(tag.begin(), tag.end(), tag.begin(), tolower);
        if (tag == "indices" || tag == "index")
        {
            matchingElements.push_back(sub);
        }
    }
    std::vector<int> indices;
    for (tinyxml2::XMLElement *sub : matchingElements)
    {
        std::string s;
        for (char c : std::string(sub->GetText()))
        {
            if (std::isdigit(c))
                s.push_back(c);
            else if (!s.empty() && !std::isdigit(c))
            {
                indices.push_back(std::stoi(s));
                s.erase();
            }
        }
    }
    size = indices.size();
    int *indexArray = indices.data();
    return indexArray;
}
int *PrismIndexData(tinyxml2::XMLElement *element, int &size)
{
    // FIXME: Prism index data is not correct. (6.10.2024)
    int count = 0;
    for (tinyxml2::XMLElement *sub = element->FirstChildElement(); sub != nullptr; sub = sub->NextSiblingElement())
    {
        std::string tag = sub->Name();
        std::transform(tag.begin(), tag.end(), tag.begin(), tolower);
        if (tag == "vertex")
            count++;
    }
    if (!count)
        return nullptr;
    size = 12 * count;
    int *result = new int[size];
    for (int i = 0, j = 0; i < size; i += 12, j++)
    {
        result[i] = j;
        result[i + 1] = (j + 1) % count;
        result[i + 2] = (j + 2) % count;

        result[i + 3] = j + count;
        result[i + 4] = (j + 1) % count + count;
        result[i + 5] = (j + 2) % count + count;

        result[i + 6] = j;
        result[i + 7] = (j + 2) % count;
        result[i + 8] = (j + 3) % count;

        result[i + 9] = j;
        result[i + 10] = (j + 3) % count;
        result[i + 11] = j + count;
    }

    return result;
}
int *ImportIndexData(tinyxml2::XMLElement *element, int &size)
{
    // TODO: Implement the import index data. (7.10.2024)
}

glm::vec3 *Translate(glm::vec3 *input, int size, glm::vec3 offset)
{
    glm::vec3 *output = new glm::vec3[size];
    for (int i = 0; i < size; i++)
        output[i] = input[i] + offset;
    return output;
}
glm::vec3 *Rotate(glm::vec3 *input, int size, glm::vec3 rotation)
{
    // FIXME: Rotation is not in correct unit.
    float x = rotation.x, y = rotation.y, z = rotation.z;
    float divider = sqrt(x * x + y * y + z * z);
    float u = x / divider, v = y / divider, w = z / divider;
    glm::vec3 center = std::accumulate(input, input + size, glm::vec3(0, 0, 0)) / (float)size;
    glm::vec3 *output = new glm::vec3[size];
    for (int i = 0; i < size; i++)
    {
        glm::vec3 v3 = input[i] - center;
        v3 = glm::rotateX(v3, rotation.x);
        v3 = glm::rotateY(v3, rotation.y);
        v3 = glm::rotateZ(v3, rotation.z);
        output[i] = v3 + center;
    }
    return output;
}
glm::vec3 *Rotate(glm::vec3 *input, int size, glm::quat rotation)
{
    // FIXME: Rotation is not in correct unit.
    glm::vec3 *output = new glm::vec3[size];
    glm::vec3 center = std::accumulate(input, input + size, glm::vec3(0, 0, 0)) / (float)size;
    for (int i = 0; i < size; i++)
    {
        glm::vec3 v = input[i];
        v -= center;
        v = glm::vec3(
            rotation.w * v.x + rotation.y * v.z - rotation.z * v.y,
            rotation.w * v.y - rotation.x * v.z + rotation.z * v.z,
            rotation.w * v.z + rotation.x * v.y - rotation.y * v.x);
        v += center;
        output[i] = v;
    }
    return output;
}
glm::vec3 *Scale(glm::vec3 *input, int size, glm::vec3 scale)
{
    glm::vec3 *output = new glm::vec3[size];
    for (int i = 0; i < size; i++)
        output[i] = glm::vec3(input[i].x * scale.x, input[i].y * scale.y, input[i].z * scale.z);
    return output;
}
glm::vec3 *ApplyTranforms(tinyxml2::XMLElement *element, glm::vec3 *input, int size)
{
    glm::vec3 scale = glm::vec3(1, 1, 1);
    if (element->Attribute("scale"))
    {
        try
        {
            std::stringstream ss;
            float *vec = new float[3];
            int i = 0;
            for (char c : std::string(element->Attribute("scale")))
            {
                if (c == ',')
                {
                    vec[i] = std::stof(ss.str());
                    ss.str("");
                    i++;
                }
                else if (std::isdigit(c) || c == '.' || c == '-')
                    ss << c;
            }
            vec[2] = std::stof(ss.str());
            scale = glm::vec3(vec[0], vec[1], vec[2]);
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid scale value : " << element->Attribute("scale") << ". Accepted as (1,1,1)." << std::endl;
        }
        input = Scale(input, size, scale);
    }
    if (element->Attribute("rotation"))
    {
        std::string s = element->Attribute("rotation");
        switch (std::count(s.begin(), s.end(), ','))
        {
        case 2:
        {
            glm::vec3 rotation;
            try
            {
                std::stringstream ss;
                float *vec = new float[3];
                int i = 0;
                for (char c : s)
                {
                    if (c == ',')
                    {
                        vec[i] = std::stof(ss.str());
                        ss.str("");
                        i++;
                    }
                    else if (std::isdigit(c) || c == '.' || c == '-')
                        ss << c;
                }
                vec[2] = std::stof(ss.str());
                rotation = glm::vec3(vec[0], vec[1], vec[2]);
                input = Rotate(input, size, rotation);
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: invalid rotation value : " << element->Attribute("rotation") << ". Accepted as (0,0,0)." << std::endl;
            }
        }
        break;
        case 3:
        {
            glm::quat rotation;
            try
            {
                std::stringstream ss;
                float *vec = new float[4];
                int i = 0;
                for (char c : s)
                {
                    if (c == ',')
                    {
                        vec[i] = std::stof(ss.str());
                        ss.str("");
                        i++;
                    }
                    else if (std::isdigit(c) || c == '.')
                        ss << c;
                }
                vec[3] = std::stof(ss.str());
                glm::quat rotation = glm::quat(vec[0], vec[1], vec[2], vec[3]);
                input = Rotate(input, size, rotation);
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: invalid rotation value : " << element->Attribute("rotation") << ". Accepted as (0,0,0,1)." << std::endl;
            }
        }
        break;
        }
    }
    glm::vec3 offset = glm::vec3(0, 0, 0);
    if (element->Attribute("offset"))
    {
        try
        {
            std::stringstream ss;
            float *vec = new float[3];
            int i = 0;
            for (char c : std::string(element->Attribute("offset")))
            {
                if (c == ',')
                {
                    vec[i] = std::stof(ss.str());
                    ss.str("");
                    i++;
                }
                else if (std::isdigit(c) || c == '.' || c == '-')
                    ss << c;
            }
            offset = glm::vec3(vec[0], vec[1], vec[2]);
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid offset value : " << element->Attribute("offset") << ". Accepted as (0,0,0)." << std::endl;
        }
        input = Translate(input, size, offset);
    }
    return input;
}