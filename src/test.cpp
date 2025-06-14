#include "app/app.h"

int main(int argc, char* argv[])
{
    try
    {
        app game; 
        game.loop();
    }
    catch(s3gl::exception e)
    {
        std::cout << e.what() << '\n';
    }
    
    return 0;
}