#pragma once
#include <chrono>
#include "Actor.h"
#include "Utils.h"
#include "Spacecraft.h"

struct LerpWithQuats
{
	static int main(int argc, char** argv);
	
	static float getDeltaTime() 
	{
		return deltaTime;
	}

	template<typename T>
	static inline T* getActorPointer()
	{	
		T* r{};

		for (auto& actor : actors)
		{
			for (const auto& tag : actor->tags)
			{
				if (tag == T::tag)
					r = static_cast<T*>(actor.get());
			}
		}
			
		return r;
	}

	template<typename T>
	static inline T& getActorRef()
	{
		T* r{ getActorPointer<T>() };

		if (r == nullptr)
		{
			std::cerr << "can't find " << T::tag << '\n';
			std::exit(1);
		}

		return *r;
	}

	static std::vector<std::unique_ptr<Actor>> actors;
	static void setMatrix(const std::array<float, 16>& newMatrix);

	private:

	static void tick();
	static void drawScene();
	static void animate(int value);
	static void initActors();
	static void setup();
	static void resize(int w, int h);
	static void keyInput(unsigned char key, int x, int y);
	static void keyInputUp(unsigned char key, int x, int y);
	static void specialFunc(int key, int x, int y);
	static void specialUpFunc(int key, int x, int y);
	static void printInteraction();
	static void drawPlayerHUD();

	static Spacecraft* spacecraft;
	static std::chrono::system_clock::time_point tp;
	static float deltaTime;
	static int animationPeriod;
	static int width;
	static int height;

	};