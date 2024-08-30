#include "Analyze.h"
#include <stack>
#include <iostream>
#include <math.h>
#include <exception>
#include <glm/gtc/quaternion.hpp>
#include <sstream>
#ifdef OBJECT_H
#include "Object.h"
#endif
#define PI 3.14159265358979323846f

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
        {"prism", PrismPositionalData}};
const std::map<std::string, std::function<int *(tinyxml2::XMLElement *, int &)>> indexHandlers =
    {
        {"box", BoxIndexData},
        {"circle", CircleIndexData},
        {"cylinder", CylinderIndexData},
        {"cone", ConeIndexData},
        {"array", ArrayIndexData},
        {"prism", PrismIndexData}};
const std::string elementTag[] =
    {
        "vertex",
        "edge",
        "face",
        "index"};

void AnalyzeTag(tinyxml2::XMLElement *element, MeshData &data)
{
    data = MeshData();
    data["position"] = std::vector<glm::vec3>();
    data["index"] = std::vector<int>();
    data["color"] = std::vector<glm::vec3>();

    std::vector<glm::vec3> &position = std::get<std::vector<glm::vec3>>(data["position"]);
    std::vector<int> &index = std::get<std::vector<int>>(data["index"]);
    std::vector<glm::vec3> &color = std::get<std::vector<glm::vec3>>(data["color"]);

    std::stack<tinyxml2::XMLElement *> stack;
    stack.push(element);
    while (stack.size())
    {
        tinyxml2::XMLElement *current = stack.top();
        stack.pop();
        for (tinyxml2::XMLElement *sub = current->FirstChildElement(); sub != nullptr; sub = sub->NextSiblingElement())
        {
            std::string tag = sub->Name();
            std::transform(tag.begin(), tag.end(), tag.begin(), tolower);
            if (std::find(std::begin(definedTags), std::end(definedTags), tag) != std::end(definedTags))
            {

                int size;

                int lastIndex = index.size() ? *std::max_element(index.begin(), index.end()) + 1 : 0;

                glm::vec3 *_position = positionalHandlers.at(tag)(sub, size);
                _position = ApplyTranforms(sub, _position, size);

                for (int i = 0; i < size; i++)
                    position.push_back(_position[i]);

                glm::vec3 *_color = AddColorData(size, sub);
                for (int i = 0; i < size; i++)
                    color.push_back(_color[i]);

                int *_index = indexHandlers.at(tag)(sub, size);
                for (int i = 0; i < size; i++)
                    index.push_back(_index[i] + lastIndex);
            }
            else
            {
                stack.push(sub);
            }
        }
    }
}

glm::vec3 *BoxPositionalData(tinyxml2::XMLElement *element, int &size)
{
    float edge = 1.0f;
    if (element->Attribute("edge"))
        try
        {
            edge = std::stof(element->Attribute("edge"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid edge value : " << element->Attribute("edge") << std::endl;
        }
    glm::vec3 *position = new glm::vec3[8];

    position[0] = glm::vec3(-edge / 2, -edge / 2, -edge / 2);
    position[1] = glm::vec3(-edge / 2, -edge / 2, edge / 2);
    position[2] = glm::vec3(-edge / 2, edge / 2, -edge / 2);
    position[3] = glm::vec3(-edge / 2, edge / 2, edge / 2);
    position[4] = glm::vec3(edge / 2, -edge / 2, -edge / 2);
    position[5] = glm::vec3(edge / 2, -edge / 2, edge / 2);
    position[6] = glm::vec3(edge / 2, edge / 2, -edge / 2);
    position[7] = glm::vec3(edge / 2, edge / 2, edge / 2);
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
    size = 2 * resolution;
    glm::vec3 *position = new glm::vec3[2 * resolution];
    for (int i = 0; i < resolution; i++)
    {
        float angle = 2 * PI * i / resolution;
        float x = radius * cos(angle), z = radius * sin(angle);
        position[i] = glm::vec3(x, -height / 2, z);
        position[i + resolution] = glm::vec3(x, height / 2, z);
    }
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
    glm::vec3 *position = new glm::vec3[resolution + 1];
    for (int i = 0; i < resolution; i++)
    {
        float angle = 2 * PI * i / resolution;
        position[i] = glm::vec3(radius * cos(angle), -height / 2, radius * sin(angle));
    }
    position[resolution] = glm::vec3(0, height / 2, 0);
    size = resolution + 1;
    return position;
}

glm::vec3 *ArrayPositionalData(tinyxml2::XMLElement *element, int &size)
{
    std::vector<glm::vec3> position;
    for (tinyxml2::XMLElement *sub = element->FirstChildElement(); sub != element->LastChildElement(); sub = element->NextSiblingElement())
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
    float height = 1.0f;
    if (element->Attribute("height"))
        try
        {
            height = std::stof(element->Attribute("height"));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: invalid height value : " << element->Attribute("height") << std::endl;
        }
    std::vector<glm::vec3> position;
    for (tinyxml2::XMLElement *sub = element->FirstChildElement(); element != element->LastChildElement(); sub = element->NextSiblingElement())
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
    for (glm::vec3 v : position)
    {
        position.push_back(v + glm::vec3(0, height, 0));
    }
    size = position.size();
    glm::vec3 *positionArray = position.data();
    return positionArray;
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

int *BoxIndexData(tinyxml2::XMLElement *element, int &size)
{
    int index[] = {
        0, 1, 3, 0, 3, 2,
        0, 4, 6, 1, 3, 7,
        1, 5, 7, 4, 7, 6,
        6, 0, 2, 5, 4, 7,
        0, 5, 4, 0, 5, 1,
        2, 7, 6, 7, 3, 2};
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
    size = 6 * resolution;
    int *index = new int[6 * resolution];
    for (int i = 0; i < resolution; i++)
    {
        index[6 * i] = i;
        index[6 * i + 1] = (i + 1) % resolution;
        index[6 * i + 2] = (i + 1) % resolution + resolution;
        index[6 * i + 3] = i;
        index[6 * i + 4] = (i + 1) % resolution + resolution;
        index[6 * i + 5] = i + resolution;
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
    size = resolution * 3;
    int *index = new int[size];
    for (int i = 0, j = 0; j < size; i++, j += 3)
    {
        index[3 * i] = i;
        index[3 * i + 1] = (i + 1) % resolution;
        index[3 * i + 2] = resolution;
    }
    return index;
}

int *ArrayIndexData(tinyxml2::XMLElement *element, int &size)
{
    std::vector<tinyxml2::XMLElement *> matchingElements;
    for (tinyxml2::XMLElement *sub = element->FirstChildElement(); sub != element->LastChildElement(); sub = element->NextSiblingElement())
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
    // TODO:implement this function
    throw std::exception();
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
    float x = rotation.x, y = rotation.y, z = rotation.z;
    float divider = sqrt(x * x + y * y + z * z);
    float u = x / divider, v = y / divider, w = z / divider;

    glm::vec3 *output = new glm::vec3[size];
    for (int i = 0; i < size; i++)
    {
        glm::vec3 v3 = input[i];
        float a = v3.x, b = v3.y, c = v3.z;
        float
            _a = (a * cos(z * PI) - b * sin(z * PI)) * cos(y * PI) + c * sin(y * PI),
            _b = (a * sin(z * PI) + b * cos(z * PI)) * cos(x * PI) - (-(a * cos(z * PI) - b * sin(z * PI)) * sin(y * PI) + c * cos(y * PI)) * sin(x * PI),
            _c = (a * sin(z * PI) + b * cos(z * PI)) * sin(x * PI) + ((a * cos(z * PI) - b * sin(z * PI)) * sin(y * PI) + c * cos(y * PI)) * cos(x * PI);
        output[i] = glm::vec3(_a, _b, _c);
    }
    return output;
}

glm::vec3 *Rotate(glm::vec3 *input, int size, glm::quat rotaion)
{
    glm::vec3 *output = new glm::vec3[size];
    for (int i = 0; i < size; i++)
    {
        glm::vec3 v = input[i];
        output[i] = glm::vec3(
            rotaion.w * v.x + rotaion.y * v.z - rotaion.z * v.y,
            rotaion.w * v.y - rotaion.x * v.z + rotaion.z * v.z,
            rotaion.w * v.z + rotaion.x * v.y - rotaion.y * v.x);
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
