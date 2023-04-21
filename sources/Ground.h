#pragma once

#include "Actor.h"

struct Ground : Actor
{
	explicit Ground(const Transform& pTransform)
		:
		transform{pTransform}
	{

	}

	void tick(float deltaTime) override;
	void setTransform(const Transform& newTransform) override;
	Transform getTransform() const override;

private:

	Transform transform;
};
	