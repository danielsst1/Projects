#include "Scene.h"
class Menu : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void playJumpSound() override;
	void Render(ShaderProgram* program) override;

	int setLives(int num);
	int getLives();
	int loseLife();

	void stopMotion();
};