#include "Animation.h"

Animation::Animation(Object *object)
{
    this->object = object;
}
int Animation::GetFrame() const
{
    return frame;
}
int Animation::GetBegin() const
{
    int begin = 0;
    for (Action action : actions)
    {
        if (action.frame < begin)
        {
            begin = action.frame;
        }
    }
    return begin;
}
int Animation::GetEnd() const
{
    int end = 0;
    for (Action action : actions)
    {
        if (action.frame > end)
        {
            end = action.frame;
        }
    }
    return end;
}
void Animation::AddAction(std::function<void(float)> action, int frame)
{
    actions.push_back({action, frame});
}
void Animation::Update(float delta)
{
    if (!playing)
        return;
    for (Action action : actions)
        if (action.frame == -1 || (action.frame > frame && action.frame <= frame))
            action.action(delta);
    frame++;
}
void Animation::Reset()
{
    frame = 0;
}
void Animation::Play()
{
    playing = true;
}
void Animation::Pause()
{
    playing = false;
}
void Animation::Stop()
{
    playing = false;
    frame = 0;
}
void Animation::SetFrame(int frame)
{
    this->frame = frame;
}
