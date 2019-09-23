#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define X_SPAWN 5

float gameBoardMatrix[RIGHE * COLONNE * 4] = {};

int tetramino[4][2];
float tetraminoColor[3];
int score = 0;
bool paused = false;

Game::Game()
{
	srand(time(NULL));
	spawnTetramino(rand() % 7);
}

void Game::restart() {
	for (int i = 0; i < RIGHE * COLONNE * 4; i++) {
		gameBoardMatrix[i] = 0;
	}
	gameOver = false;
}

float* Game::getBlockInfo(int x, int y) {
	float* blockInfo = new float[4];
	int index = y * COLONNE + x;

	blockInfo[0] = gameBoardMatrix[index * 4];
	blockInfo[1] = gameBoardMatrix[index * 4 + 1];
	blockInfo[2] = gameBoardMatrix[index * 4 + 2];
	blockInfo[3] = gameBoardMatrix[index * 4 + 3];

	return blockInfo;
}

void Game::togglePause() {
	paused = !paused;
}

void Game::setBlockInfo(int x, int y, float status, float r, float g, float b) {
	int index = y * COLONNE + x;

	gameBoardMatrix[index * 4] = status;
	gameBoardMatrix[index * 4 + 1] = r;
	gameBoardMatrix[index * 4 + 2] = g;
	gameBoardMatrix[index * 4 + 3] = b;
}

void Game::spawnTetramino(int type) {

	switch (type) {
	case(0):
		//SAETTA
		tetramino[2][0] = X_SPAWN;
		tetramino[2][1] = RIGHE;

		tetramino[1][0] = X_SPAWN;
		tetramino[1][1] = RIGHE + 1;

		tetramino[0][0] = X_SPAWN - 1;
		tetramino[0][1] = RIGHE + 1;

		tetramino[3][0] = X_SPAWN - 1;
		tetramino[3][1] = RIGHE + 2;

		tetraminoColor[0] = 1;
		tetraminoColor[1] = 0;
		tetraminoColor[2] = 0;
		break;
	case(1):
		//LUNGO
		tetramino[1][0] = X_SPAWN;
		tetramino[1][1] = RIGHE;

		tetramino[0][0] = X_SPAWN;
		tetramino[0][1] = RIGHE + 1;

		tetramino[2][0] = X_SPAWN;
		tetramino[2][1] = RIGHE + 2;

		tetramino[3][0] = X_SPAWN;
		tetramino[3][1] = RIGHE + 3;

		tetraminoColor[0] = 0;
		tetraminoColor[1] = 1;
		tetraminoColor[2] = 0;
		break;
	case(2):
		//CUBO
		tetramino[0][0] = X_SPAWN;
		tetramino[0][1] = RIGHE;

		tetramino[1][0] = X_SPAWN - 1;
		tetramino[1][1] = RIGHE;

		tetramino[2][0] = X_SPAWN;
		tetramino[2][1] = RIGHE + 1;

		tetramino[3][0] = X_SPAWN - 1;
		tetramino[3][1] = RIGHE + 1;

		tetraminoColor[0] = 0;
		tetraminoColor[1] = 0;
		tetraminoColor[2] = 1;
		break;
	case(3):
		//T
		tetramino[0][0] = X_SPAWN;
		tetramino[0][1] = RIGHE;

		tetramino[1][0] = X_SPAWN - 1;
		tetramino[1][1] = RIGHE;

		tetramino[2][0] = X_SPAWN + 1;
		tetramino[2][1] = RIGHE;

		tetramino[3][0] = X_SPAWN;
		tetramino[3][1] = RIGHE + 1;

		tetraminoColor[0] = 1;
		tetraminoColor[1] = 1;
		tetraminoColor[2] = 0;
		break;
	case(4):
		//L
		tetramino[0][0] = X_SPAWN;
		tetramino[0][1] = RIGHE;

		tetramino[1][0] = X_SPAWN + 1;
		tetramino[1][1] = RIGHE;

		tetramino[2][0] = X_SPAWN;
		tetramino[2][1] = RIGHE + 1;

		tetramino[3][0] = X_SPAWN;
		tetramino[3][1] = RIGHE + 2;

		tetraminoColor[0] = 0;
		tetraminoColor[1] = 1;
		tetraminoColor[2] = 1;
		break;
	case(5):
		//SAETTA REVERSE
		tetramino[1][0] = X_SPAWN;
		tetramino[1][1] = RIGHE;

		tetramino[2][0] = X_SPAWN;
		tetramino[2][1] = RIGHE + 1;

		tetramino[0][0] = X_SPAWN + 1;
		tetramino[0][1] = RIGHE + 1;

		tetramino[3][0] = X_SPAWN + 1;
		tetramino[3][1] = RIGHE + 2;

		tetraminoColor[0] = 1;
		tetraminoColor[1] = 1;
		tetraminoColor[2] = 1;
		break;
	case(6):
		//L REVERSE
		tetramino[0][0] = X_SPAWN;
		tetramino[0][1] = RIGHE;

		tetramino[1][0] = X_SPAWN - 1;
		tetramino[1][1] = RIGHE;

		tetramino[2][0] = X_SPAWN;
		tetramino[2][1] = RIGHE + 1;

		tetramino[3][0] = X_SPAWN;
		tetramino[3][1] = RIGHE + 2;

		tetraminoColor[0] = 1;
		tetraminoColor[1] = 0;
		tetraminoColor[2] = 1;
		break;
	}
	
	
}

bool Game::checkRowCompleted(int y) {
	bool canDestroy = true;

	for (int x = 0; x < COLONNE; x++) {
		int index = y * COLONNE + x;

		if (gameBoardMatrix[index * 4] != 1) {
			canDestroy = false;
			break;
		}
	}

	return canDestroy;
}

void Game::removeRow(int y_start) {
	for (int y = y_start; y < RIGHE; y++) {
		for (int x = 0; x < COLONNE; x++) {
			int index = y * COLONNE + x;

			if (y < RIGHE - 1) {
				gameBoardMatrix[index * 4] = gameBoardMatrix[(index + 10) * 4];
				gameBoardMatrix[index * 4 + 1] = gameBoardMatrix[((index + 10) * 4) + 1];
				gameBoardMatrix[index * 4 + 2] = gameBoardMatrix[((index + 10) * 4) + 2];
				gameBoardMatrix[index * 4 + 3] = gameBoardMatrix[((index + 10) * 4) + 3];
			} else {
				gameBoardMatrix[index * 4] = 0;
				gameBoardMatrix[index * 4 + 1] = 0;
				gameBoardMatrix[index * 4 + 2] = 0;
				gameBoardMatrix[index * 4 + 3] = 0;

			}
		}
	}
}

void Game::checkRows() {
	int y = 0;
	int rowRemoved = 0;
	while (y < RIGHE) {
		if (checkRowCompleted(y)) {
			rowRemoved++;
			removeRow(y);
		} else {
			y++;
		}
	}

	if (rowRemoved > 0) {
		AddScore(rowRemoved);
	}
}

void Game::AddScore(int rowRemoved) {
	switch (rowRemoved) {
	case(1):
		score += 100 * rowRemoved;
		break;
	case(2):
		score += 100 * rowRemoved * 1.5;
		break;
	case(3):
		score += 100 * rowRemoved * 2;
		break;
	case(4):
		score += 100 * rowRemoved * 3;
		break;
	}
}

bool Game::checkLose() {
	bool lose = false;
	for (int x = 0; x < COLONNE; x++) {
		if (getBlockInfo(x, RIGHE - 1)[0] == 1) {
			lose = true;
		}
	}
	return lose;
}

int Game::getScore() {
	return score;
}

void Game::dropTetramino() {
	if (!paused) {
		bool canDrop = true;
		for (int* tetrCoord : tetramino) {
			if (getBlockInfo(tetrCoord[0], tetrCoord[1] - 1)[0] == 1 || tetrCoord[1] == 0) {
				canDrop = false;
				break;
			}
		}

		if (canDrop) {
			for (int* tetrCoord : tetramino) {
				if (tetrCoord[1] < RIGHE) {
					setBlockInfo(tetrCoord[0], tetrCoord[1], 0.0, 0.0, 0.0, 0.0);
				}
			}

			for (int* tetrCoord : tetramino) {
				tetrCoord[1] = tetrCoord[1] - 1;

				if (tetrCoord[1] < RIGHE) {
					setBlockInfo(tetrCoord[0], tetrCoord[1], 0, tetraminoColor[0], tetraminoColor[1], tetraminoColor[2]);
				}
				else if (tetrCoord[1] == RIGHE) {
					setBlockInfo(tetrCoord[0], tetrCoord[1], 0, tetraminoColor[0], tetraminoColor[1], tetraminoColor[2]);
				}

			}
		}
		else {
			for (int* tetrCoord : tetramino) {
				if (tetrCoord[1] <= RIGHE) {
					setBlockInfo(tetrCoord[0], tetrCoord[1], 1, tetraminoColor[0], tetraminoColor[1], tetraminoColor[2]);
				}
			}

			checkRows();

			gameOver = checkLose();

			if (!gameOver) {
				spawnTetramino(rand() % 7);
			}
		}
	}
}

void Game::moveTetramino(MoveType type) {
	if (!paused) {
		bool canMove = true;
		int movement;

		if (type == MoveType::LEFT) {
			movement = -1;
		}
		else {
			movement = 1;
		}

		for (int* tetrCoord : tetramino) {
			if (tetrCoord[0] <= 0 && type == MoveType::LEFT) {
				canMove = false;
				break;
			}

			if (tetrCoord[0] >= COLONNE - 1 && type == MoveType::RIGHT) {
				canMove = false;
				break;
			}

			if (getBlockInfo(tetrCoord[0] + movement, tetrCoord[1])[0] == 1) {
				canMove = false;
				break;
			}
		}

		if (canMove) {
			for (int* tetrCoord : tetramino) {
				if (tetrCoord[1] <= RIGHE) {
					setBlockInfo(tetrCoord[0], tetrCoord[1], 0.0, 0.0, 0.0, 0.0);
				}
			}

			for (int* tetrCoord : tetramino) {
				tetrCoord[0] = tetrCoord[0] + movement;

				if (tetrCoord[1] <= RIGHE) {
					setBlockInfo(tetrCoord[0], tetrCoord[1], 0, tetraminoColor[0], tetraminoColor[1], tetraminoColor[2]);
				}
			}

		}
	}
}

void Game::rotateTetramino() {
	if (!paused) {
		bool canRotate = true;

		for (int* tetrCoord : tetramino) {
			int newX = tetrCoord[0] - tetramino[0][0];
			int newY = tetrCoord[1] - tetramino[0][1];

			//x <- -y | y <- x
			int supp = newY;
			newY = newX;
			newX = -supp;

			newX += tetramino[0][0];
			newY += tetramino[0][1];

			if (getBlockInfo(newX, newY)[0] == 1 || newX >= COLONNE || newX < 0) {
				canRotate = false;
				break;
			}
		}

		if (canRotate) {
			for (int* tetrCoord : tetramino) {
				setBlockInfo(tetrCoord[0], tetrCoord[1], 0.0, 0.0, 0.0, 0.0);
			}

			for (int* tetrCoord : tetramino) {
				int newX = tetrCoord[0] - tetramino[0][0];
				int newY = tetrCoord[1] - tetramino[0][1];

				//x <- -y | y <- x
				int supp = newY;
				newY = newX;
				newX = -supp;

				tetrCoord[0] = newX + tetramino[0][0];
				tetrCoord[1] = newY + tetramino[0][1];

				setBlockInfo(tetrCoord[0], tetrCoord[1], 0, tetraminoColor[0], tetraminoColor[1], tetraminoColor[2]);
			}
		}
	}
}

Game::~Game()
{
}
