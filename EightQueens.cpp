#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "Headers/QueenPiece.h"
#include "Headers/EightQueens.h"
#include "Headers/Board.h"

#define FLOOR_WIDTH 9
#define FLOOR_HEIGHT 9
#define BUFFSIZE 512

// Global Variables
bool isZoomingIn = false;
bool isZoomingOut = false;
float camera_fov = 40.0;

bool isRotatingLeft = false;
bool isRotatingRight = false;
float camera_theta = 0.0;

// Light variables
float ambient[] = {0.1f, 0.1f, 0.1f, 0.2f};
float diffuse[] = {0.5f, 0.5f, 0.5f, 0.2f};
float specular[] = {0.5f, 0.5f, 0.5f, 0.2f};
float position[] = {200.0f, 300.0f, -400.0f, 1.0f};

// Selection variables
bool inSelection;
QueenPiece *selectedPiece;
Square destination;

// Queen Pieces vector
std::vector<QueenPiece> pieces;

// Board Object
Board board;

// Array for the checkered board vertices
GLfloat board_vertices[FLOOR_HEIGHT * FLOOR_WIDTH * 3];

void EightQueens::render_board()
{
    glMatrixMode(GL_MODELVIEW);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    board.render();

    glLoadName(-1);
}

void EightQueens::render_objs()
{
    glMatrixMode(GL_MODELVIEW);

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
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisable(GL_LIGHTING);

    // Render the board
    render_board();

    glEnable(GL_LIGHTING);

    // Render the objects
    render_objs();

    glDisableClientState(GL_COLOR_ARRAY);
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
        }
    }

    std::vector<Square> tiles;

    // Making the tiles
    // For the tile colors, a current initial color for the row is set and it is toggled across the row for each tile
    // For the next row, the initial color is also toggled
    for (int i = 1, current_initial_color = 0; i < FLOOR_WIDTH; i++)
    {
        auto from = (i - 1) * FLOOR_WIDTH;
        auto to = i * FLOOR_WIDTH;

        for (int j = 0, current_square_color = current_initial_color; j < FLOOR_HEIGHT - 1; j++)
        {
            // Setting the vertices in the right order
            vertex v1 = vertex(board_vertices[from * 3], board_vertices[from * 3 + 1], board_vertices[from * 3 + 2]);
            vertex v2 = vertex(board_vertices[to * 3], board_vertices[to * 3 + 1], board_vertices[to * 3 + 2]);
            from += 1;
            to += 1;
            vertex v3 = vertex(board_vertices[to * 3], board_vertices[to * 3 + 1], board_vertices[to * 3 + 2]);
            vertex v4 = vertex(board_vertices[from * 3], board_vertices[from * 3 + 1], board_vertices[from * 3 + 2]);

            // If the square is supposed to be white, make a white tile with the vertices and add it to the list
            if (current_square_color == 1)
            {
                Square square(v1, v2, v3, v4, color4(1.0, 1.0, 1.0, 1.0), 10 + ((i - 1) * (FLOOR_WIDTH - 1) + j));
                tiles.push_back(square);
            }
            // If the square is supposed to be black, make a black tile with the vertices and add it to the list
            else
            {
                Square square(v1, v2, v3, v4, color4(0.0, 0.0, 0.0, 1.0), 10 + ((i - 1) * (FLOOR_WIDTH - 1) + j));
                tiles.push_back(square);
            }

            // Toggle the current square color (from black to white and vice versa)
            current_square_color = (current_square_color == 1) ? 0 : 1;
        }
        // Toggle the starting color (from black to white and vice versa)
        current_initial_color = (current_initial_color == 1) ? 0 : 1;
    }

    board = Board(FLOOR_WIDTH - 1, FLOOR_HEIGHT - 1, tiles);
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

        QueenPiece p(row, col, i, vertex(x, y, z));

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

void EightQueens::mouse_func(int button, int state, int x, int y)
{
    GLuint selectBuf[BUFFSIZE];
    GLint hits;
    GLint viewport[4];
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(BUFFSIZE, selectBuf);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(-1);

    // PICK MATRIX

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 10.0, 10.0, viewport);

    // ORIGINAL PROJECTION MATRIX

    auto w = glutGet(GLUT_WINDOW_WIDTH);
    auto h = glutGet(GLUT_WINDOW_HEIGHT);

    gluPerspective(camera_fov, (float)w / (float)h, 2.0, 500.0);

    gluLookAt(
        10, 10, 20,
        0, 0, 0,
        0, 1, 0);

    glRotatef(camera_theta, 0, 1, 0);

    // RENDER

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisable(GL_LIGHTING);

    render_board();

    glEnable(GL_LIGHTING);

    render_objs();

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glutSwapBuffers();

    // PROCESS HITS
    hits = glRenderMode(GL_RENDER);
    select(hits, selectBuf);
}

void EightQueens::select(GLint hits, GLuint buffer[])
{
    GLuint *ptr;

    ptr = (GLuint *)buffer;

    ptr += (hits - 1) * 4 + 3;

    // If a queen was selected
    if ((*ptr) < 10)
    {
        // Search and update the pieces
        for (std::vector<QueenPiece>::iterator it = pieces.begin(); it != pieces.end(); it++)
        {
            if ((it->model).name == (*ptr))
            {
                if (it->isSelected)
                {
                    it->setIsSelected(false);
                    inSelection = false;
                }
                else
                {
                    it->setIsSelected(true);
                    selectedPiece = &(*it);
                    inSelection = true;
                }
            }
            else if (it->isSelected)
            {
                it->setIsSelected(false);
            }
        }
    }
    // If a tile was selected
    else
    {
        // If a piece has already been selected, now we want to choose the tile
        if (inSelection)
        {
            // Search in tiles
            for (std::vector<Square>::iterator it = board.tiles.begin(); it != board.tiles.end(); it++)
            {
                if (it->name == (*ptr))
                {
                    destination = (*it);

                    vertex vd = destination.getVertices()[0];
                    vd.x += 0.5;
                    vd.z += 0.5;

                    auto slope = abs((vd.z - selectedPiece->model.getPosition().z) / (vd.x - selectedPiece->model.getPosition().x));

                    // Move only if the destination is valid
                    if ((vd.x == selectedPiece->model.getPosition().x) || (vd.z == selectedPiece->model.getPosition().z) ||slope == 1){
                        auto init_motion_time = glutGet(GLUT_ELAPSED_TIME);
                        selectedPiece->startMoving(init_motion_time, vd);
                    }


                    selectedPiece->setIsSelected(false);
                    inSelection = false;
                }
            }
        }
    }
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

    auto current_time = glutGet(GLUT_ELAPSED_TIME);

    for (std::vector<QueenPiece>::iterator it = pieces.begin(); it != pieces.end(); it++)
    {
        it->update(current_time);
    }

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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Eight Queens");

    init();

    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutSpecialUpFunc(keyboard_up);
    glutMouseFunc(mouse_func);
    glutTimerFunc(25, timer, 0);
    glutMainLoop();
}
