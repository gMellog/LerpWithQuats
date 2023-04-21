#include "Ground.h"

void Ground::tick(float deltaTime)
{
    glPushMatrix();

    const float width = transform.scale.X;
    const float height = transform.scale.Y;

    glColor3f(1.0, 0, 0);
    glTranslatef(0.f, -15.f, 0.f);
    glScalef(width, 1.f, height);
    glutSolidCube(1.f);

    glPopMatrix();
}

void Ground::setTransform(const Transform &newTransform)
{
    transform = newTransform;
}

Transform Ground::getTransform() const
{
    return transform;
}