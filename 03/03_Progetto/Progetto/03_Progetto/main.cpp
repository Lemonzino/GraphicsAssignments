


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>

#include <chrono>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Game.h"

#define W_WIDTH 260.0
#define W_HEIGHT 500.0

float aspect_ratio = W_WIDTH / W_HEIGHT;

#define GAME_START_POS_X 50.0
#define GAME_START_POS_Y 70.0

#define CUBE_SIZE 16.0

static Game* game;

typedef struct { 
	double r, g, b, a; 
} Color;
typedef struct {
	float x;
	float y;
	float alpha;
	float xFactor;
	float yFactor;
	float drag;
	Color colorP;
} PARTICLE;

std::vector <PARTICLE> particles;

#pragma region ParticleFunctions

	Color computeRainbow() {
		static float rgb[3] = { 1.0, 0.0, 0.0 };
		static int fase = 0, counter = 0;
		const float step = 0.1;
		Color paint;

		switch (fase) {
		case 0: rgb[1] += step;
			break;
		case 1: rgb[0] -= step;
			break;
		case 2: rgb[2] += step;
			break;
		case 3: rgb[1] -= step;
			break;
		case 4: rgb[0] += step;
			break;
		case 5: rgb[2] -= step;
			break;
		default:
			break;
		}

		counter++;
		if (counter > 1.0 / step) {
			counter = 0;
			fase < 5 ? fase++ : fase = 0;
		}

		paint.r = rgb[0];
		paint.g = rgb[1];
		paint.b = rgb[2];
		return paint;
	}

	void makeParticles(float x, float y) {
		Color rgb = computeRainbow();
		for (int i = 0; i < 10; i++) {
			PARTICLE p;
			p.x = x;
			p.y = y;
			p.alpha = 1.0;
			p.drag = 1.25;
			p.xFactor = (rand() % 1000 + 1) / 300 * (rand() % 2 == 0 ? -1 : 1);
			p.yFactor = (rand() % 1000 + 1) / 300 * (rand() % 2 == 0 ? -1 : 1);
			p.colorP.r = rgb.r;
			p.colorP.g = rgb.g;
			p.colorP.b = rgb.b;
			particles.push_back(p);
		}
	}
	
	void drawParticles() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPointSize(3.0);
		glBegin(GL_POINTS);
		for (unsigned long i = 0; i < particles.size(); i++) {
			particles.at(i).xFactor /= particles.at(i).drag;
			particles.at(i).yFactor /= particles.at(i).drag;

			particles.at(i).x += particles.at(i).xFactor;
			particles.at(i).y += particles.at(i).yFactor;

			particles.at(i).alpha -= 0.05;

			glColor4f(static_cast<GLfloat>(particles.at(i).colorP.r), static_cast<GLfloat>(particles.at(i).colorP.g),
				static_cast<GLfloat>(particles.at(i).colorP.b), particles.at(i).alpha);
			glVertex2f(particles.at(i).x, particles.at(i).y);

			if (particles.at(i).alpha <= 0.0) {
				particles.erase(particles.begin() + i);
			}
		}
		glEnd();
		glDisable(GL_BLEND);

	}

#pragma endregion

	int counter = 0;
void endGameScreen(int value) {

	if (counter == 5) {
		makeParticles((rand() % ((int)W_WIDTH + 1)), (rand() % ((int)W_HEIGHT + 1)));
		counter = 0;
	}

	counter++;
	glutPostRedisplay();
	glutTimerFunc(50, endGameScreen, 0);
}

void drawCube(int x, int y, float r, float g, float b) {
	glPushMatrix();
	glTranslatef(GAME_START_POS_X + (x * CUBE_SIZE),GAME_START_POS_Y + (y * CUBE_SIZE),0);
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, 15);
	glVertex2f(15, 15);
	glVertex2f(15, 0);
	glEnd();
	glPopMatrix();
}

void drawGrid() {
	for (int y = 0; y < RIGHE; y++) {
		for (int x = 0; x < COLONNE; x++) {
			float* blockInfo = game->getBlockInfo(x, y);
			if (blockInfo[1] != 0 | blockInfo[2] != 0 | blockInfo[3] != 0) {
				drawCube(x, y, blockInfo[1], blockInfo[2], blockInfo[3]);
			}
		}
	}
}

void drawGameSpace() {
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(GAME_START_POS_X - 1, GAME_START_POS_Y - 1);
	glVertex2f(GAME_START_POS_X - 1 + (COLONNE * CUBE_SIZE), GAME_START_POS_Y - 1);
	glVertex2f(GAME_START_POS_X + (COLONNE * CUBE_SIZE), GAME_START_POS_Y + (RIGHE * CUBE_SIZE));
	glVertex2f(GAME_START_POS_X + (COLONNE * CUBE_SIZE) + 10, GAME_START_POS_Y + (RIGHE * CUBE_SIZE));
	glVertex2f(GAME_START_POS_X + (COLONNE * CUBE_SIZE) + 10, GAME_START_POS_Y - 10);
	glVertex2f(GAME_START_POS_X - 10, GAME_START_POS_Y - 10);
	glVertex2f(GAME_START_POS_X - 10, GAME_START_POS_Y + (RIGHE * CUBE_SIZE));
	glVertex2f(GAME_START_POS_X, GAME_START_POS_Y + (RIGHE * CUBE_SIZE));
	glEnd();
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, W_WIDTH, 0.0, W_HEIGHT, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void drop_loop(int value) {
	game->dropTetramino();

	if (game->gameOver) {
		endGameScreen(0);
		glutPostRedisplay();
		return;
	}

	glutPostRedisplay();
	glutTimerFunc(20, drop_loop, 0);
}

void bitmap_output(int x, int y, int z, std::string s, void *font)
{
	int i;
	glRasterPos3f(x, y, 0);

	for (i = 0; i < s.length(); i++)
		glutBitmapCharacter(font, s[i]);
}

void drawScene() {
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (game->gameOver) {
		glColor3f(1.0, 1.0, 1.0);
		bitmap_output(50, W_HEIGHT / 2, 0, "Final Score", GLUT_BITMAP_TIMES_ROMAN_24);
		bitmap_output(50, W_HEIGHT / 2 - 30, 0, std::to_string(game->getScore()), GLUT_BITMAP_TIMES_ROMAN_24);
		bitmap_output(50, W_HEIGHT / 2 - 60, 0, "y -> Restart", GLUT_BITMAP_TIMES_ROMAN_24);
		bitmap_output(50, W_HEIGHT/2 - 90, 0, "n -> Exit", GLUT_BITMAP_TIMES_ROMAN_24);
		drawParticles();
	}
	else {
		drawGameSpace();
		drawGrid();

		glColor3f(1.0, 0.0, 0.0);
		std::string testo = "Punteggio: ";
		testo.append(std::to_string(game->getScore()));
		bitmap_output(10, 10, 0, testo, GLUT_BITMAP_TIMES_ROMAN_24);
	}

	glutSwapBuffers();
}




void KeyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		if (!game->gameOver) {
			game->moveTetramino(MoveType::LEFT);
		}
		break;
	case 'd':
		if (!game->gameOver) {
			game->moveTetramino(MoveType::RIGHT);
		}
		break;
	case 's':
		if (!game->gameOver) {
			game->rotateTetramino();
		}
		break;
	case 'p':
		if (!game->gameOver) {
			game->togglePause();
		}
		break;
	case 'y':
		if (game->gameOver) {
			game->restart();
			glutTimerFunc(200, drop_loop, 0);
		}
		break;
	case 'n':
		if (game->gameOver) {
			exit(0);
		}
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Tetris");

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	glutKeyboardFunc(KeyboardFunc);

	game = new Game();

	glutTimerFunc(200, drop_loop, 0);
	glutMainLoop();

	return 0;
}
