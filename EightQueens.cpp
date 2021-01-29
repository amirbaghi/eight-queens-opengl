#include "Headers/EightQueens.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#define FLOOR_WIDTH 9
#define FLOOR_HEIGHT 9

// Global Variables
int init_zoom_time = 0.0;
bool isZoomingIn = false;
bool isZoomingOut = false;
float camera_fov = 40.0;

float camera_theta = 0.0;
int init_time = 0;

// Vertex Buffer Object Ids
GLuint board_vcs_vbo, board_index_vbo;

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

    glBindBuffer(GL_ARRAY_BUFFER, board_vcs_vbo);
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    // Draw the white squares
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_index_vbo);
    glColor4f(0.85, 0.85, 0.85, 1.0);
    glDrawElements(GL_QUADS, sizeof(white_square_indices) / 4, GL_UNSIGNED_INT, NULL);

    // Draw the black squares
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_index_vbo + 1);
    glColor4f(0.1, 0.1, 0.1, 1.0);
    glDrawElements(GL_QUADS, sizeof(black_square_indices) / 4, GL_UNSIGNED_INT, NULL);

    // Draw the board outline
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_index_vbo + 2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_QUADS, sizeof(board_outline_indices) / 4, GL_UNSIGNED_INT, NULL);

    glPopAttrib();
}

void EightQueens::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glDisable(GL_LIGHTING);

    // Render the board
    render_board();

    glEnable(GL_LIGHTING);

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
            camera_fov -= 0.2;
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

    // Initialzing the board vertex and index buffers
    glGenBuffers(1, &board_vcs_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, board_vcs_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(board_vertices), board_vertices, GL_STATIC_DRAW);

    glGenBuffers(3, &board_index_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_index_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(white_square_indices), white_square_indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_index_vbo + 1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(black_square_indices), black_square_indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_index_vbo + 2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(board_outline_indices), board_outline_indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EightQueens::init_light()
{
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);

    // Candle Yellow Light
    float light0_diffuse[] = {255.0 / 255.0, 147.0 / 255.0, 41.0 / 255.0, 1.0};
    float light0_specular[] = {255.0 / 255.0, 147.0 / 255.0, 41.0 / 255.0, 1.0};
    float light0_ambient[] = {255.0 / 255.0, 147.0 / 255.0, 41.0 / 255.0, 1.0};
    float light0_pos[] = {3, 3, 3, 1};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
}

void EightQueens::init()
{
    glewInit();

    init_time = glutGet(GLUT_ELAPSED_TIME);

    glClearColor(0.2, 0.2, 0.2, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initializing the board
    init_board();

    glEnableClientState(GL_VERTEX_ARRAY);

    // Initializing the lightning
    init_light();
}

void EightQueens::camera_config(int w, int h, float t, float fov)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fov, (float)w / (float)h, 2.0, 500.0);

    gluLookAt(
        10, 10, 20,
        0, 0, 0,
        0, 1, 0);

    glRotatef(t, 0, 1, 0);
}

void EightQueens::timer(int value)
{
    camera_theta += 0.5;

    // Zoom in/out if the user is currently pressing the up or down arrow
    if (isZoomingIn)
    {
        if (camera_fov >= 0)
            camera_fov -= 0.5;
    }
    else if (isZoomingOut)
    {
        if (camera_fov <= 100)
            camera_fov += 0.5;
    }

    auto w = glutGet(GLUT_WINDOW_WIDTH);
    auto h = glutGet(GLUT_WINDOW_HEIGHT);

    camera_config(w, h, camera_theta, camera_fov);

    glutTimerFunc(25, timer, value + 1);
}

void EightQueens::reshape(int w, int h)
{
    camera_config(w, h, camera_theta, camera_fov);
}

void EightQueens::keyboard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        // Start Zooming in
        isZoomingIn = true;
        init_zoom_time = glutGet(GLUT_ELAPSED_TIME);
        break;

    case GLUT_KEY_DOWN:
        // Start Zooming out
        isZoomingOut = true;
        init_zoom_time = glutGet(GLUT_ELAPSED_TIME);
        break;

    default:
        break;
    }
}

void EightQueens::keyboard_up(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        // Stop Zooming in
        isZoomingIn = false;
        break;

    case GLUT_KEY_DOWN:
        // Stop Zooming out
        isZoomingOut = false;
        break;

    default:
        break;
    }
}

int EightQueens::main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Eight Queens");

    init();

    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutSpecialUpFunc(keyboard_up);
    glutTimerFunc(25, timer, 0);
    glutMainLoop();
}
