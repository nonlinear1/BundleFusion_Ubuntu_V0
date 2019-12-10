
class TestBinaryStreamTestData {
public:
	TestBinaryStreamTestData() { m_Data = nullptr; m_Size = 0;}
	~TestBinaryStreamTestData() { clear(); }

	void clear() {
		SAFE_DELETE_ARRAY(m_Data);	
		m_Size = 0;
	}

	void init() {
		m_Size = 1000;
		m_Data = new int[m_Size];
		for (unsigned int i = 0; i < m_Size; i++) {
			m_Data[i] = i;
		}
	}

	bool operator==(const TestBinaryStreamTestData& other) {
		if (other.m_Size != m_Size)	return false;
		for (unsigned int i = 0; i < m_Size; i++) {
			if (m_Data[i] != other.m_Data[i])	return false;
		}
		return true;
	}

	unsigned int m_Size;
	int* m_Data;
};

//! read from binary stream overload
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline ml::BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(ml::BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, TestBinaryStreamTestData& data) {
	data.clear();
	s >> data.m_Size;
	data.m_Data = new int[data.m_Size];
	s.readData((BYTE*)data.m_Data, sizeof(int)*data.m_Size);
	return s;
}
//! write to binary stream overload
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline ml::BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(ml::BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const TestBinaryStreamTestData& data) {
	s << data.m_Size;
	s.writeData((BYTE*)data.m_Data, sizeof(int)*data.m_Size);
	return s;
}

class TestBinaryStream : public Test {

	void test0()
	{
		using namespace ml;
		const std::string filenameCompressed = "testStreamCompressed.out";
		const std::string filename = "testStream.out";

		std::vector<int> v(10000, 0);

		//for an output stream, 'clearBuffer' should be set to true to reset it
		BinaryDataStreamZLibFile streamOut(filenameCompressed, true);
		TestBinaryStreamTestData data;	data.init();
		streamOut << data << v;
		streamOut.close();	//must call this here to make sure everything has been written to disk

		BinaryDataStreamFile streamOutComp(filename, true);
		TestBinaryStreamTestData data2;	data2.init();
		streamOutComp << data2 << v;
		streamOutComp.close();	//must call this here to make sure everything has been written to disk
		

		UINT64 dataSize = util::getFileData(filename).size();
		UINT64 dataSizeCompressed = util::getFileData(filenameCompressed).size();

		TestBinaryStreamTestData reRead;
		BinaryDataStreamZLibFile streamIn(filenameCompressed, false);
		streamIn >> reRead;
		streamIn >> v;

		MLIB_ASSERT(data == reRead);

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

	void test1() 
	{
		//test string (pod)
		std::string s = "123abc\n";
		{
			BinaryDataStreamFile out("tmp.bin", true);
			out << s;
		}
		{
			std::string re;
			BinaryDataStreamFile in("tmp.bin", false);
			in >> re;
			MLIB_ASSERT_STR(s == re, "binary stream string doesn't match out");
		}
		util::deleteFile("tmp.bin");

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

	void test2()
	{
		//test vector (pod)
		size_t s = 1000;
		std::vector<float> v(s);
		for (size_t i = 0; i < v.size(); i++) {
			v[i] = math::randomUniform(0.0f, 1.0f);
		}
		BinaryDataStreamFile out("tmp.bin", true);
		out << v;
		out.close();
		
		BinaryDataStreamFile in("tmp.bin", false);
		std::vector<float> re;
		in >> re;
		
		MLIB_ASSERT_STR(v == re, "binary stream vector doesn't match out");

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

	void test3() 
	{
		{
			BinaryDataStreamFile out("test.bin", true);
			int a = 5;
			out.writeData(a);
		}
		{
			BinaryDataStreamFile in("test.bin", false);
			int a;
			in.readData(a);

			MLIB_ASSERT_STR(a == 5, "binary stream in/out doesn't match");
		}

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

	std::string getName()
	{
		return "Binary Stream";
	}
};
