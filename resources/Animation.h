#include "Object.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <functional>

#ifndef ANIMATION_H
#define ANIMATION_H
struct Action
{
    std::function<void(float)> action;
    int frame;
};
struct Animation
{
private:
    Object *object;
    std::vector<Action> actions;
    int frame = 0;
    bool playing = false;

public:
    Animation(Object *object);
    int GetFrame() const;
    int GetBegin() const;
    int GetEnd() const;
    void AddAction(std::function<void(float)> action, int frame = -1);
    void Update(float delta = 1.0f);
    void Reset();
    void Play();
    void Pause();
    void Stop();
    void SetFrame(int frame);
};
#endif