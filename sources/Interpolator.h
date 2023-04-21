#include "Actor.h"

struct Interpolator
{
    explicit Interpolator(Actor& pActor)
    :
        actor{pActor}, 
        t{-1.f},
        rotStart{},
        rotEnd{},
        interQuat{},
        start{},
        end{},
        lerpListener{},
        speed{1},
        rotMatrix{}
    {

    }

    void interpolate(const Quaternion& newRotStart, const Quaternion& newRotEnd, 
                     const Vector& newStart, const Vector& newEnd, float speed = 1.f);

    void tick(float deltaTime);

    bool isLerping() const noexcept;

    void addLerpEndedListener(const std::function<void()>& listener);
    
    RotationMatrix getRotMatrix() const noexcept;

    private:

    std::reference_wrapper<Actor> actor;
    float t;
    
    Quaternion rotStart;
    Quaternion rotEnd;
    Quaternion interQuat;

    Vector start;
    Vector end;

    std::function<void()> lerpListener;

    float speed;

    RotationMatrix rotMatrix;

};