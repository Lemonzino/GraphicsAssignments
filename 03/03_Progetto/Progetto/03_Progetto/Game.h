#pragma once

#define RIGHE 24
#define COLONNE 10

typedef enum {
	LEFT,
	RIGHT
} MoveType;

class Game
{
public:
	Game();

	bool gameOver = false;

	//Restituisco la schermate per la renderizzazione
	float* getBlockInfo(int x, int y);
	int getScore();
	void setBlockInfo(int x, int y, float status, float r, float g, float b);

	void dropTetramino();
	void moveTetramino(MoveType type);
	void rotateTetramino();
	void togglePause();
	void restart();

	~Game();

private:
	//da mettere il tipo del tetramino
	void spawnTetramino(int type);
	void checkRows();
	bool checkRowCompleted(int y);
	void removeRow(int y_start);
	bool checkLose();
	void AddScore(int rowRemoved);
};

