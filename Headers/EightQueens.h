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

} // namespace EightQueens