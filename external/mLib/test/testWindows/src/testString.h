class TestString : public Test
{
public:
	void test0()
	{
		String s("this is ");
		s = s + "a string";
		MLIB_ASSERT_STR(s.split(" ").size() == 4, "split test0 failed");
		MLIB_ASSERT_STR(s.split(' ').back() == "string", "split test1 failed");

		MLIB_ASSERT_STR(s.startsWith("this is"), "startsWith test failed");

		for(int i = 0; i < 100; i++)
		{
			MLIB_ASSERT_STR(String(i).toINT() == i, "convert test failed");
		}

		std::cout << "string test0 passed" << std::endl;
	}

	String getName()
	{
		return "vector";
	}
};