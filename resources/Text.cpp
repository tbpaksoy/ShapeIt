#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include "Text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <filesystem>
Text::Text(const char *content, const char *font, int size)
{
    this->content = std::string(content, content + strlen(content));
    this->font = font;
    this->size = size;
}
Text::~Text()
{
}
void Text::LoadCharacters()
{
    // En: Load a font face from a file.
    // Tr: Bir dosyadan font yüzünü yükle.
    FT_Face face;
    if (FT_New_Face(ft, font, 0, &face))
    {
        std::cout << "Failed to load font" << std::endl;
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, size);

    // En: Generate character textures.
    // Tr: Karakter dokularını oluştur.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++)
    {
        if (content.find(c) == std::string::npos)
            continue;

        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)};

        characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
}
void Text::SetColor(glm::vec3 color) noexcept
{
    this->color = color;
}
void Text::SetFont(const char *font) noexcept
{
    this->font = font;
}
void Text::SetSize(int size) noexcept
{
    this->size = size;
}
void Text::SetPosition(glm::vec2 position) noexcept
{
    this->position = position;
}
unsigned int Text::GetQuadCount() const
{
    return content.size();
}
float *Text::GetVertices(int &size)
{
    if (!characters.size())
        LoadCharacters();

    float *vertices = new float[24 * content.size()];

    float xpos = 0;
    int index = 0;
    for (std::string::const_iterator c = content.begin(); c != content.end(); c++)
    {
        Character ch = characters[*c];

        float x = (position.x + ch.bearing.x) / (2 << 6),
              y = (position.y - (ch.size.y - ch.bearing.y)) / (2 << 6),
              w = ch.size.x / (2 << 6),
              h = ch.size.y / (2 << 6);

        vertices[index] = x;
        vertices[index + 1] = y + h;
        vertices[index + 2] = 0.0f;
        vertices[index + 3] = 0.0f;

        vertices[index + 4] = x;
        vertices[index + 5] = y;
        vertices[index + 6] = 0.0f;
        vertices[index + 7] = 1.0f;

        vertices[index + 8] = x + w;
        vertices[index + 9] = y;
        vertices[index + 10] = 1.0f;
        vertices[index + 11] = 1.0f;

        vertices[index + 12] = x;
        vertices[index + 13] = y + h;
        vertices[index + 14] = 0.0f;
        vertices[index + 15] = 0.0f;

        vertices[index + 16] = x + w;
        vertices[index + 17] = y;
        vertices[index + 18] = 1.0f;
        vertices[index + 19] = 1.0f;

        vertices[index + 20] = x + w;
        vertices[index + 21] = y + h;
        vertices[index + 22] = 1.0f;
        vertices[index + 23] = 1.0f;

        index += 24;
        xpos += ch.advance >> 6;
    }

    size = content.size() * 24;

    return vertices;
}
GLuint *Text::GetTextures(int &size)
{
    size = content.size();
    GLuint *textures = new GLuint[size];
    for (int i = 0; i < size; i++)
    {
        textures[i] = characters[content[i]].textureID;
    }
    return textures;
}
