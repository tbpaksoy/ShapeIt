#include <string>
#include <glm/glm.hpp>
#ifndef OBJECT_H
#define OBJECT_H
const char *GlobalFont = nullptr;
class Text
{
private:
    std::wstring content;
    const char *font;
    glm::vec3 color = glm::vec3(1.0f);

public:
    Text(const char *content, const char *font = nullptr);
    ~Text();
    void Render(glm::vec2 position, glm::vec2 scale);
    void SetColor(glm::vec3 color) noexcept;
    void SetFont(const char *font) noexcept;
};
#endif