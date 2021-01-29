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

    // Initialization Function
    void init();

    // Function for initializing the chess board arrays and data
    void init_board();

    // Funciton for initializing the lightning
    void init_light();
    
    // Main Function
    int main(int argc, char **argv);

    // Camera Configuration Function
    void camera_config(int w, int h, float t, float fov);

    // Reshape Function
    void reshape(int w, int h);

} // namespace EightQueens