#include <GLFW/glfw3.h>
#include "Text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
Text::Text(const char *content, const char *font = nullptr)
{
    this->content = std::wstring(content, content + strlen(content));
    this->font = font;
}
Text::~Text()
{
}
void Text::Render(glm::vec2 position, glm::vec2 scale)
{
    glPushMatrix();
    glTranslatef(position.x, position.y, 0.0f);
    glScalef(scale.x, scale.y, 1.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}
void Text::SetColor(glm::vec3 color) noexcept
{
    this->color = color;
    FT_Face face;
    for (char c : content)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        glBitmap(
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            face->glyph->advance.x,
            face->glyph->advance.y,
            face->glyph->bitmap.buffer);
    }
    glPopMatrix();
    delete face;
}
void Text::SetFont(const char *font) noexcept
{
    this->font = font;
}