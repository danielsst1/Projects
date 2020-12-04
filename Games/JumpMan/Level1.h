#include "Scene.h"


class Level1 : public Scene {
public:
	enum gameMode {PLAY, WIN, LOSE};

	void Initialize() override;
	void Update(float deltaTime) override;
	void playJumpSound() override;
	void Render(ShaderProgram* program) override;

	int setLives(int num);
	int getLives();
	int loseLife();

	void stopMotion();
	void loseGame();
};