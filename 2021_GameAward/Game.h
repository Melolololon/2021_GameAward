#pragma once
class Game final
{
private:
	Game();
	~Game();

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();

public:
	Game(const Game& g) = delete;
	Game& operator=(const Game& g) = delete;

	static Game* GetInstance();
	static const int WIN_WIDTH = 1280;
	static const int WIN_HEIGHT = 720;
	void Run();
};

