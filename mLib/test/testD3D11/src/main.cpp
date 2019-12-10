
#include "main.h"

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	AppTest callback;
	ml::ApplicationWin32 app(hInstance, 1280, 1024, "D3D11 Test", ml::GraphicsDeviceTypeD3D11, callback, 100, 200);
	app.messageLoop();

	return EXIT_SUCCESS;
}

int main()
{
	vec6f;
    AppTest callback;
	ml::ApplicationWin32 app(nullptr, 1280, 1024, "D3D11 Test", ml::GraphicsDeviceTypeD3D11, callback, 100, 200);
    app.messageLoop();

	return EXIT_SUCCESS;
}
