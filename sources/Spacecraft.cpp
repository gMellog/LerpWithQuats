#include "Spacecraft.h"
#include "LerpWithQuats.h"

std::vector<std::pair<bool, std::size_t>> getInitVKeyMappings()
{
    return
    {
        { false, GLUT_KEY_UP },
        { false, GLUT_KEY_DOWN },
        { false, 'x'},
        { false, 'X'},
        { false, 'y'},
        { false, 'Y'},
        { false, 'z'},
        { false, 'Z'}
    };
}

void Spacecraft::initMatrix()
{
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.f; 
}

Spacecraft::Spacecraft(const Transform& pTransform)
:
    transform{pTransform},
    interp{*this},
    vKeyMappings{getInitVKeyMappings()},
    isStartSet{},
    finalLerping{},
    eulerAngles{},
	startAngles{},
    endAngles{},
    angleOffset{5.f},
    matrix{}
{
    interp.addLerpEndedListener(
    [this]
    ()
    {   
        if(!finalLerping)
        {
            eulerAngles = startAngles;
        }
    });

    initMatrix();
}

void Spacecraft::draw()
{
    glPushMatrix();

    const auto X = transform.translation.X;
    const auto Y = transform.translation.Y;
    const auto Z = transform.translation.Z;

    const auto roll = eulerAngles.alpha;
    const auto yaw = eulerAngles.beta;
    const auto pitch = eulerAngles.gamma;

    glColor3f(1.0, 1.0, 0);

    glPushMatrix();

    glTranslatef(X, Y, Z);
    if(!interp.isLerping())
    {
        glRotatef(roll, 1.f, 0.f , 0.f);
        glRotatef(yaw, 0.f, 1.f , 0.f);
        glRotatef(pitch, 0.f, 0.f , 1.f);
    }
    else
    {
        glMultMatrixf(matrix.data());
    }
    
    glutSolidCone(5.f, 10.f, 20.f, 20.f);
    glPopMatrix();

    glPopMatrix();
}

void Spacecraft::setEulerAngles(const EulerAngles& newEulerAngles)
{
    eulerAngles = newEulerAngles;
}	

EulerAngles Spacecraft::getEulerAngles() const noexcept
{
    return eulerAngles;
}

void Spacecraft::tick(float deltaTime)
{
    interp.tick(deltaTime);
    
    if(!interp.isLerping())
        handleInput();
    else 
    {
        const auto rotMatrix = interp.getRotMatrix();
        matrix = rotMatrix.matrixInColumnForm;
    }

    draw();
}

void Spacecraft::setTransform(const Transform &newTransform)
{
    transform = newTransform;
}

Transform Spacecraft::getTransform() const
{
    return transform;
}

void Spacecraft::keyInput(int key, int x, int y)
{  
    setKeyInBindingsTo(key, true);

    switch(key)
    {
    case ' ':
        if(!interp.isLerping())
        {
            const auto currLoc = transform.translation;

            if(!isStartSet)
            {
                start = currLoc;
                isStartSet = true;
                startAngles = eulerAngles;
            }
            else
            {
                end = currLoc;
                isStartSet = false;

                endAngles = eulerAngles;
                
                const auto q1 = convertEulerAnglesToQuat(endAngles);
                const auto q2 = convertEulerAnglesToQuat(startAngles);

                interp.interpolate(q1, q2, end, start, 1.f);
            }
        }
        break;
    default:
        break;
    }
}


void Spacecraft::setKeyInBindingsTo(int key, bool down)
{
    auto it = std::find_if(vKeyMappings.begin(), vKeyMappings.end(),
        [key]
    (const std::pair<bool, std::size_t>& keyMapping)
        {
            return keyMapping.second == key;
        });

    if (it == std::end(vKeyMappings)) return;
    
    it->first = down;

    if(!down)
    {
        int inverseKey = key > 100 ? key - 32 : key + 32;

        auto itInverse = std::find_if(vKeyMappings.begin(), vKeyMappings.end(),
            [inverseKey]
        (const std::pair<bool, std::size_t>& keyMapping)
            {
                return keyMapping.second == inverseKey;
            });
        if (itInverse == std::end(vKeyMappings)) return;
        else
        {
            it->first = down;
            itInverse->first = down;
        }
    }
}

void Spacecraft::keyInputUp(unsigned char key, int x, int y)
{
    setKeyInBindingsTo(key, false);
}
                                                                                                        
void Spacecraft::specialDownFunc(int key, int x, int y)
{
    setKeyInBindingsTo(key, true);
}

void Spacecraft::specialUpFunc(int key, int x, int y)
{
    setKeyInBindingsTo(key, false);
}

Vector rotTransform(const std::array<float, 16> m, const Vector& v)
{
    Vector r;

    r.X = m[0] * v.X + m[4] * v.Y + m[8] * v.Z;
    r.Y = m[1] * v.X + m[5] * v.Y + m[9] * v.Z;
    r.Z = m[2] * v.X + m[6] * v.Y + m[10] * v.Z;
    
    return r;
}
 
void Spacecraft::handleInput()
{
     for (const auto& keyMapping : vKeyMappings)
        {
            if (!keyMapping.first) continue;

            int key = keyMapping.second;

            auto newTransform = transform;
            auto& loc = newTransform.translation;
            auto& rot = newTransform.rotation;

            const auto q = convertEulerAnglesToQuat(eulerAngles);
            const auto rotMatrix = q.getRotMatrix();

            const Vector v{
                0.f,
                0.f,
                1.f
            };

            const auto move = rotTransform(rotMatrix.matrixInColumnForm, v);
            
            if (key == GLUT_KEY_UP)
            {   
                loc.X += move.X;
                loc.Y += move.Y;
                loc.Z += move.Z;
            }
            else if (key == GLUT_KEY_DOWN)
            {
                loc.X -= move.X;
                loc.Y -= move.Y;
                loc.Z -= move.Z;
            }
            else if(key == 'x')
            {    
                eulerAngles.alpha += angleOffset;
        
                if(eulerAngles.alpha >= 360.f)
                    eulerAngles.alpha -= 360.f;
            }
            else if(key == 'X')
            {
                eulerAngles.alpha -= angleOffset;
            
                if(eulerAngles.alpha <= -360.f)
                    eulerAngles.alpha += 360.f;
            }
            else if(key == 'z')
            {
                eulerAngles.gamma += angleOffset;
       
                if(eulerAngles.gamma >= 360.f)
                     eulerAngles.gamma -= 360.f;
            }
            else if(key == 'Z')
            {
                
                eulerAngles.gamma -= angleOffset;
            
                if(eulerAngles.gamma <= -360.f)
                    eulerAngles.gamma += 360.f;
            }
            else if(key == 'y')
            {
                eulerAngles.beta += angleOffset;
       
                if(eulerAngles.beta >= 360.f)
                    eulerAngles.beta -= 360.f;
            }
            else if(key == 'Y')
            {
                eulerAngles.beta -= angleOffset;
            
                if(eulerAngles.beta <= -360.f)
                    eulerAngles.beta += 360.f;

            }

            transform = newTransform;
        }
}