
#include "main.h"

int main(int argc, char** argv)
{
    //AdjustmentProblem aProblem;
    //aProblem.solve();

    Vizzer callback;
    ApplicationWin32 app(nullptr, 1280, 1024, "D3D11 Test", GraphicsDeviceTypeD3D11, callback);
    app.messageLoop();

    return 0;
}
