class Test
{
public:
	void run()
	{
		test0();
		test1();
		test2();
		test3();
		test4();
		test5();
		test6();
		test7();
		test8();
		test9();
	}

	virtual std::string getName() = 0;

	virtual void test0() {}
	virtual void test1() {}
	virtual void test2() {}
	virtual void test3() {}
	virtual void test4() {}
	virtual void test5() {}
	virtual void test6() {}
	virtual void test7() {}
	virtual void test8() {}
	virtual void test9() {}
};
