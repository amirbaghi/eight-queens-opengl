#ifndef MY_EIGHTQUEENS_H
#define MY_EIGHTQUEENS_H

#include "QueenPiece.h"
#include <GL/glut.h>

namespace EightQueens
{
    // Keyboard Function
    void keyboard(int key, int x, int y);

    // Keyboard Up Function
    void keyboard_up(int key, int x, int y);

    // Render Function
    void render();

    // Function for rendering the chess board
    void render_board();

    // Function for rendering the chess pieces
    void render_objs();

    // Initialization Function
    void init();

    // Function for initializing the chess board arrays and data
    void init_board();

    // Funciton for initializing the lightning
    void init_light();

    // Function for initializing the objects
    void init_objs();
    
    // Main Function
    int main(int argc, char **argv);

    // Camera Configuration Function
    void camera_config(int w, int h, float t, float fov);

    // Timer Function
    void timer(int value);

    // Reshape Function
    void reshape(int w, int h);

    // Mouse Func Function
    void mouse_func(int button, int state, int x, int y);

    // Check for threats for all pieces
    void check_for_threats();

    // Check for threats for a piece
    void check_for_piece_threats(QueenPiece& piece);

    // Selection Function
    void select(GLint hits, GLuint buffer[]);

    // Keystroke function
    void keyboard_options(unsigned char key, int x, int y);

} // namespace EightQueens

#endif