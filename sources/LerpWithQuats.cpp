#include "LerpWithQuats.h"
#include "Ground.h"
#include "Spacecraft.h"

	std::string makeLabelWithVal(const std::string& label, float val)
	{
		return label + std::to_string(val);
	}

	void LerpWithQuats::drawPlayerHUD()
	{
		glColor3f(0.f, 0.f, 0.f);

		const auto [alpha, beta, gamma] = spacecraft->getEulerAngles();

		const auto alphaLabel = makeLabelWithVal("alpha: ", alpha);
		const auto betaLabel = makeLabelWithVal("beta:  ", beta);
		const auto gammaLabel = makeLabelWithVal("gamma: ", gamma);

		glRasterPos3d(2.8f, 4.f, -5.f);
		writeBitmapString(GLUT_BITMAP_9_BY_15, alphaLabel);

		glRasterPos3d(2.8f, 3.7f, -5.f);
		writeBitmapString(GLUT_BITMAP_9_BY_15, betaLabel);

		glRasterPos3d(2.8f, 3.4f, -5.f);
		writeBitmapString(GLUT_BITMAP_9_BY_15, gammaLabel);
	}

    void LerpWithQuats::tick()
	{	
		const float dist = 40.f;
		glPushMatrix();

		drawPlayerHUD();

		gluLookAt(0.f, dist, dist, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

		for(const auto& actor : actors)
			actor->tick(deltaTime);	
		glPopMatrix();

	}

	void LerpWithQuats::drawScene(void)
	{
		using namespace std::chrono;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		auto d = duration_cast<milliseconds>(system_clock::now() - tp);
		deltaTime = float(d.count()) * milliseconds::period::num / milliseconds::period::den;
		tp = system_clock::now();

		tick();
	
		glutSwapBuffers();
	}

	void LerpWithQuats::animate(int value)
	{
		glutPostRedisplay();
		glutTimerFunc(animationPeriod, animate, 1);
	}

	std::unique_ptr<Ground> createGround()
	{
		return std::make_unique<Ground>(
			Transform{{}, {100.f, 100.f, 1.f}}
		);
	}

	std::unique_ptr<Spacecraft> createSpacecraft()
	{
		return std::make_unique<Spacecraft>(
			Transform{{0.f, 5.f, 0.f}}
		);
	}

	void LerpWithQuats::initActors()
	{	
		auto freshSpacecraft = createSpacecraft();

		if(freshSpacecraft == nullptr)
		{
			std::cerr << "Error! Can't initialize spacecraft!" << std::endl;
			std::exit(1);
		}

		spacecraft = freshSpacecraft.get();

		auto ground = createGround();

		actors.push_back(std::move(freshSpacecraft));
		actors.push_back(std::move(ground));
		
		for(const auto& actor : actors)
			actor->init();
	}
	void LerpWithQuats::setup(void)
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glEnable(GL_DEPTH_TEST);

		initActors();

		tp = std::chrono::system_clock::now();
		animate(1);
	}

	void LerpWithQuats::resize(int w, int h)
	{
		glViewport(0, 0, w, h);
		
		width = w;
		height = h;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);

		glMatrixMode(GL_MODELVIEW);
	}

	void LerpWithQuats::keyInput(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 27:
			exit(0);
			break;
		default:
			break;
		}

		spacecraft->keyInput(key, x, y);
	}

	void LerpWithQuats::keyInputUp(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 27:
			exit(0);
			break;
		default:
			break;
		}

		spacecraft->keyInputUp(key, x, y);
	}

	void LerpWithQuats::specialFunc(int key, int x, int y)
	{	
		spacecraft->specialDownFunc(key, x, y);
		glutPostRedisplay();
	}

	void LerpWithQuats::specialUpFunc(int key, int x, int y)
	{
		spacecraft->specialUpFunc(key, x, y);
		glutPostRedisplay();
	}

	void LerpWithQuats::printInteraction()
	{
		std::cout << "Walk around with arrow buttons (UP/DOWN),\n";
		std::cout << "rotate yourself with x/X on Roll, y/Y on Yaw, z/Z on Pitch\n";
		std::cout << "which doesn't matter cause it's a cone xD\n";
		std::cout << "You can turn on interpolation in next few moves: \n";
		std::cout << "1. Press space bar for FINAL position and rotation(yes, your first tap will specify that))\n";
		std::cout << "2. Press space bar again when you will specify your start position\n";
		std::cout << "3. Enjoy" << std::endl;
	}

	int LerpWithQuats::main(int argc, char** argv)
	{
		printInteraction();
		glutInit(&argc, argv);

		glutInitContextVersion(4, 3);
		glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

		glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowSize(width, height);
		glutInitWindowPosition(100, 100);
		glutCreateWindow("LerpWithQuats");
		glutDisplayFunc(drawScene);
		glutReshapeFunc(resize);
		glutKeyboardFunc(keyInput);
		glutKeyboardUpFunc(keyInputUp);
		glutSpecialFunc(specialFunc);
		glutSpecialUpFunc(specialUpFunc);
		glewExperimental = GL_TRUE;
		glewInit();

		setup();

		glutMainLoop();

		return 0;
	}

	Spacecraft* LerpWithQuats::spacecraft{};
	std::chrono::system_clock::time_point LerpWithQuats::tp{};
	float LerpWithQuats::deltaTime{};
	int LerpWithQuats::animationPeriod{};
	int LerpWithQuats::width{800};
	int LerpWithQuats::height{600};

	std::vector<std::unique_ptr<Actor>> LerpWithQuats::actors{};
