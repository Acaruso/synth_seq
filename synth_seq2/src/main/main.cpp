#include "src/main/app/app.hpp"
#include "src/main/callback/callback.hpp"

int main(int argc, char* args[])
{
    App app(setup, callback);
    app.run();
    return 0;
}
