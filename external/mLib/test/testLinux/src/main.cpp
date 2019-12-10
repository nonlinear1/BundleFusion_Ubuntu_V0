
#include "mLibCore.h"
#include "mLibLodePNG.h"

using namespace ml;
using namespace std;

int main(int argc, char** argv)
{
	cout << "hello world!" << endl;

    ColorImageR8G8B8A8 image(512, 512);
    LodePNG::save(image, "test.png");

    cout << "testing timer" << endl;
    Timer t;
    for (int i = 0; i < 100; i++)
    {
        if(i % 10 == 0) cout << i << endl;
        ColorImageR8G8B8A8 test = LodePNG::load("test.png");
    }
    cout << t.getElapsedTime() << "s" << endl;

	return 0;
}