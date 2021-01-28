#include "Headers/EightQueens.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#define FLOOR_WIDTH 9
#define FLOOR_HEIGHT 9

float camera_theta = 0.0;
int init_time = 0;

// Arrays for the checkered board vertices and the indices for the black and white squares and the outline
GLfloat board_vertices[FLOOR_HEIGHT * FLOOR_WIDTH * 3];
GLuint black_square_indices[((FLOOR_HEIGHT - 1) * (FLOOR_WIDTH - 1) * 4) / 2];
GLuint white_square_indices[((FLOOR_HEIGHT - 1) * (FLOOR_WIDTH - 1) * 4) / 2];
GLuint board_outline_indices[4];

void EightQueens::render_board()
{

    // glPushAttrib(GL_LINE_BIT);
    // glLineWidth(3);
    // glBegin(GL_LINES);
    // glColor3f(1.0, 0, 0); glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);
    // glColor3f(0, 1.0, 0); glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);
    // glColor3f(0, 0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);
    // glEnd();
    // glPopAttrib();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glVertexPointer(3, GL_FLOAT, 0, board_vertices);

    // Draw the white squares
    glColor3f(1.0, 1.0, 1.0);
    glDrawElements(GL_QUADS, sizeof(white_square_indices) / 4, GL_UNSIGNED_INT, white_square_indices);

    // Draw the black squares
    glColor3f(0.0, 0.0, 0.0);
    glDrawElements(GL_QUADS, sizeof(black_square_indices) / 4, GL_UNSIGNED_INT, black_square_indices);

    // Draw the board outline
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_QUADS, sizeof(board_outline_indices) / 4, GL_UNSIGNED_INT, board_outline_indices);

    glPopAttrib();
}

void EightQueens::render()
{
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    auto diff_time = (current_time - init_time) / 1000.0;

    camera_theta = diff_time * 2.0;

    auto w = glutGet(GLUT_WINDOW_WIDTH);
    auto h = glutGet(GLUT_WINDOW_HEIGHT);

    camera_config(w, h, camera_theta);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    render_board();

    glutSwapBuffers();
}

void EightQueens::init_board()
{
    // Initializing the vertex array
    for (int i = 0; i < FLOOR_WIDTH; i++)
    {
        auto x = -(FLOOR_WIDTH / 2) + i;
        for (int j = 0; j < FLOOR_HEIGHT; j++)
        {
            auto z = -(FLOOR_HEIGHT / 2) + j;

            board_vertices[((i * FLOOR_WIDTH + j) * 3)] = x;
            board_vertices[((i * FLOOR_WIDTH + j) * 3) + 1] = 0;
            board_vertices[((i * FLOOR_WIDTH + j) * 3) + 2] = z;
        }
    }

    // Setting the indices for the black and white quads
    // An initial color is set at the start of each row, then that color is toggled for each quad across the row
    // The initial color is toggled for the next row to achieve the appropriate coloring for the chess board
    for (int i = 1, blk_arr_ind = 0, white_arr_ind = 0, current_initial_color = 0; i < FLOOR_WIDTH; i++)
    {
        auto from = (i - 1) * FLOOR_WIDTH;
        auto to = i * FLOOR_WIDTH;

        for (int j = 0, current_square_color = current_initial_color; j < FLOOR_HEIGHT - 1; j++)
        {

            // If the current square is to be white, then add the current quad indices to the white square indices array
            if (current_square_color == 1)
            {
                white_square_indices[white_arr_ind++] = from++;
                white_square_indices[white_arr_ind++] = to++;
                white_square_indices[white_arr_ind++] = to;
                white_square_indices[white_arr_ind++] = from;
            }
            // and if the square is to be black, then add the current quad indices to the black square indices array
            else
            {
                black_square_indices[blk_arr_ind++] = from++;
                black_square_indices[blk_arr_ind++] = to++;
                black_square_indices[blk_arr_ind++] = to;
                black_square_indices[blk_arr_ind++] = from;
            }

            // Toggle the current square color (from black to white and vice versa)
            current_square_color = (current_square_color == 1) ? 0 : 1;
        }
        // Toggle the starting color (from black to white and vice versa)
        current_initial_color = (current_initial_color == 1) ? 0 : 1;
    }

    // Setting the board outline indices
    board_outline_indices[0] = 0;
    board_outline_indices[1] = FLOOR_HEIGHT * (FLOOR_WIDTH - 1);
    board_outline_indices[2] = FLOOR_HEIGHT * (FLOOR_WIDTH - 1) + (FLOOR_HEIGHT - 1);
    board_outline_indices[3] = 0 + (FLOOR_HEIGHT - 1);
}

void EightQueens::init()
{
    glewInit();

    init_time = glutGet(GLUT_ELAPSED_TIME);

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    // Initializing the board
    init_board();

    glEnableClientState(GL_VERTEX_ARRAY);
}

void EightQueens::camera_config(int w, int h, float t)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(20.0, (float)w / (float)h, 2.0, 500.0);

    gluLookAt(
        10, 10, 20,
        0, 0, 0,
        0, 1, 0);

    glRotatef(t, 0, 1, 0);
}

void EightQueens::reshape(int w, int h)
{
    camera_config(w, h, camera_theta);
}

int EightQueens::main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Eight Queens");

    init();

    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
    glutMainLoop();
}
