#include "Headers/EightQueens.h"
#include "Headers/QueenPiece.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

#define FLOOR_WIDTH 9
#define FLOOR_HEIGHT 9

// Global Variables
bool isZoomingIn = false;
bool isZoomingOut = false;
float camera_fov = 40.0;

bool isRotatingLeft = false;
bool isRotatingRight = false;
float camera_theta = 0.0;

// Light variables
float ambient[] = {0.2f, 0.2f, 0.2f, 0.3f};
float diffuse[] = {0.4f, 0.4f, 0.4f, 0.3f};
float specular[] = {0.4f, 0.4f, 0.4f, 0.3f};
float position[] = {200.0f, 300.0f, -400.0f, 0.0f};

// Material variables
float mat_ambient[] = {0.25f, 0.148f, 0.06475f, 1.0f};
float mat_diffuse[] = {0.4f, 0.2368f, 0.1036f, 1.0f};
float mat_specular[] = {0.774597f, 0.458561f, 0.200621f, 1.0f};
float shine = 10.8f;

// Queen Pieces vector
std::vector<QueenPiece> pieces(8);

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
    // glColor3f(1.0, 0, 0);
    // glVertex3f(0, 0, 0);
    // glVertex3f(5, 0, 0);
    // glColor3f(0, 1.0, 0);
    // glVertex3f(0, 0, 0);
    // glVertex3f(0, 5, 0);
    // glColor3f(0, 0, 1.0);
    // glVertex3f(0, 0, 0);
    // glVertex3f(0, 0, 5);
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

void EightQueens::render_objs()
{
    glMatrixMode(GL_MODELVIEW);

    // Setting material properties for the pieces
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

    for (QueenPiece piece : pieces)
    {
        piece.render();
    }

    glLoadName(-1);
}

void EightQueens::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_LIGHTING);

    // Render the board
    render_board();

    glEnable(GL_LIGHTING);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Render the objects
    render_objs();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

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

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void EightQueens::init_objs()
{
    std::srand((unsigned int)time(NULL));

    // Generating the queen pieces with a random row and column for each
    for (int i = 0; i < 8; i++)
    {
        int row, col;
        bool hasConflict;

        // Calculating a random row and col and checking for conflict with other pieces' position
        // If so, calculate another random row and col
        do
        {
            row = 1 + (std::rand() % (8 - 1 + 1));
            col = 1 + (std::rand() % (8 - 1 + 1));
            hasConflict = false;
            for (QueenPiece piece : pieces)
            {
                if (piece.row == row && piece.col == col)
                {
                    hasConflict = true;
                }
            }

        } while (hasConflict);

        // Calculating the position of the square center at the given row and col
        double x = board_vertices[((row - 1) * 9 + (col - 1)) * 3] + 0.5;
        double z = board_vertices[((row - 1) * 9 + (col - 1)) * 3 + 2] + 0.5;
        double y = 0;

        QueenPiece p(row, col, i, Object3D::vertex(x, y, z));

        pieces.push_back(p);
    }
}

void EightQueens::init()
{
    glewInit();

    glClearColor(0.2, 0.2, 0.2, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    // Initializing the board
    init_board();

    // Initializing the objects
    init_objs();

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

    // Rotate left/right if the user is currently pressing the left or right arrow
    if (isRotatingLeft)
    {
        camera_theta += 0.5;
    }
    else if (isRotatingRight)
    {
        camera_theta -= 0.5;
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
        break;

    case GLUT_KEY_DOWN:
        // Start Zooming out
        isZoomingOut = true;
        break;

    case GLUT_KEY_LEFT:
        // Start rotating left
        isRotatingLeft = true;
        break;

    case GLUT_KEY_RIGHT:
        // Start rotating right
        isRotatingRight = true;
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

    case GLUT_KEY_LEFT:
        // Stop rotating left
        isRotatingLeft = false;
        break;

    case GLUT_KEY_RIGHT:
        // Stop rotating right
        isRotatingRight = false;
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
