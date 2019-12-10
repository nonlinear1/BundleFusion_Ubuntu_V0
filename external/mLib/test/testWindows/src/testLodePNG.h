class TestLodePNG : public Test
{
public:
	void test0()
	{
		using namespace ml;
		ColorImageR8G8B8A8 bmp(17, 41);
		bmp.setPixels(RGBColor::Magenta);
		bmp(1, 1) = RGBColor::Blue;
		bmp(1, 0) = RGBColor::Red;
		bmp(13, 6) = RGBColor::Green;
		bmp(2, 7) = RGBColor::White;

		LodePNG::save(bmp, "test.png");
		ColorImageR8G8B8A8 bmpReloaded = LodePNG::load("test.png");

		MLIB_ASSERT_STR(bmp == bmpReloaded, "bitmaps do not match");
	}

	std::string getName()
	{
		return "LodePNG";
	}
};
