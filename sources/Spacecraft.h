#pragma once

#include "Actor.h"
#include "Interpolator.h"

struct Spacecraft : Actor
{
	explicit Spacecraft(const Transform& pTransform);

	void tick(float deltaTime) override;
	void setTransform(const Transform& newTransform) override;
	Transform getTransform() const override;
	void keyInput(int key, int x, int y);
	void keyInputUp(unsigned char key, int x, int y);
	void specialDownFunc(int key, int x, int y);
	void specialUpFunc(int key, int x, int y);
	
	void setEulerAngles(const EulerAngles& newEulerAngles);
	EulerAngles getEulerAngles() const noexcept;

private:

	Transform transform;
	Interpolator interp;
	std::vector< std::pair<bool, std::size_t> > vKeyMappings;

	bool isStartSet;
	Vector start;
	Vector end;

	bool finalLerping;

	EulerAngles eulerAngles;
	EulerAngles startAngles;
	EulerAngles endAngles;

	float angleOffset;

	std::array<float, 16> matrix;

	void initMatrix();
	void draw();
	void handleInput();
	void setKeyInBindingsTo(int key, bool down);
};
	