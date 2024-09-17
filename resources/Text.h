#include <string>
#include <glm/glm.hpp>
#include <map>
#include "Shader.h"
#ifndef TEXT_H
#define TEXT_H

const char *GlobalFont = nullptr;
FT_Library ft;
struct Character
{
public:
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

class Text
{
private:
    std::string content;
    const char *font;
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec2 position;
    int size;
    std::map<char, Character> characters;
    void LoadCharacters();

public:
    Text(const char *content, const char *font = nullptr, int size = 16);
    ~Text();
    void SetColor(glm::vec3 color) noexcept;
    void SetFont(const char *font) noexcept;
    void SetSize(int size) noexcept;
    void SetPosition(glm::vec2 position) noexcept;
    void GenerateQuad();
    unsigned int GetQuadCount() const;
    float *GetVertices(int &size);
    GLuint *GetTextures(int &size);
};

#endif