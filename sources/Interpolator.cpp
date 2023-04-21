#include "Interpolator.h"

void Interpolator::interpolate(const Quaternion& newRotStart, const Quaternion& newRotEnd,
                        const Vector& newStart, const Vector& newEnd, float newSpeed)
{      
    rotStart = newRotStart;
    rotEnd = newRotEnd;
    start = newStart;
    end = newEnd;
    speed = newSpeed;
    t = 0.f;
}

void Interpolator::tick(float deltaTime)
{
    if(t != -1.f)
    {
        t = clamp(0.f, 1.f, t + 0.01f * speed);
        
        const auto interLoc = lerp(start, end, t);
        interQuat = slerp(rotStart, rotEnd, t);

        rotMatrix = interQuat.getRotMatrix();

        auto transform = actor.get().getTransform();
        transform.translation = interLoc;
        actor.get().setTransform(transform);
        
        if(t == 1.f)
        {
            t = -1.f;
            lerpListener();
        }
    }
}

bool Interpolator::isLerping() const noexcept
{
    return t != -1.f;
}

void Interpolator::addLerpEndedListener(const std::function<void()>& listener)
{
    lerpListener = listener;
}

RotationMatrix Interpolator::getRotMatrix() const noexcept
{
    return rotMatrix;
}
