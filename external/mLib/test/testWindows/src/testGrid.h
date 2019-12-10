
class TestGrid : public Test {
public:
	void test0() 
	{
		unsigned int s = 10;
		BinaryGrid3 grid(s,s,s);
		for (unsigned int i = 0; i < s; i++) {
			for (unsigned int j = 0; j < s; j++) {
				for (unsigned int k = 0; k < s; k++) {
					grid.setVoxel(i,j,k);
					MLIB_ASSERT_STR(grid.isVoxelSet(i,j,k), "voxel set failed");
					MLIB_ASSERT_STR(checkIfAllOtherAreZero(grid, i,j,k), "voxel set failed");
					grid.clearVoxel(i,j,k);
				}
			}
		}

		grid.clearVoxels();
		for (unsigned int i = 0; i < s; i++) {
			grid.setVoxel(i,i,i);
		}

		std::cout <<  __FUNCTION__ << " passed" << std::endl;
	}

	void test1()
	{
		unsigned int s = 50;
		BinaryGrid3 grid(s,s,s);
		for (unsigned int i = 0; i < s; i++) {
			grid.setVoxel(i,i,i);
			grid.setVoxel(50-1-i,i,i);
			grid.setVoxel(i,50-1-i,i);
			grid.setVoxel(i,i,50-1-i);
		}
		PointCloudf pc(grid, 1.0f);
		PointCloudIOf::saveToFile("gridcloud0.ply", pc);

		{
			auto rePC = PointCloudIOf::loadFromFile("gridcloud0.ply");
			BinaryGrid3 re(s);
			for (const auto& p : rePC.m_points) {
				re.setVoxel(math::round(p));
			}
			MLIB_ASSERT_STR(re == grid, "point cloud conversion failed");
			
		}
		util::deleteFile("gridcloud0.ply");

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

	void test2() 
	{
		BoundingBox3f bb;
		bb.include(vec3f(0.0f,0.0f,0.0f));
		bb.include(vec3f(10.0f,10.0f,10.0f));
		mat4f trans = bb.cubeToWorldTransform();
		vec3f p0 = trans * vec3f(0.0f,0.0f,0.0f);
		vec3f p1 = trans * vec3f(1.0f,1.0f,1.0f);

		//bb.include(vec3f(1.0f,1.0f,1.0f));
		//std::vector<vec3f> p;	std::vector<vec3ui> i;
		//bb.makeTriMesh(p, i);
		//TriMeshf tmesh(p.size(), i.size() * 3, p.data(), (unsigned int*)i.data());
		//MeshIOf::saveToFile("box.ply", tmesh.getMeshData());

		ml::TriMeshf sphere = ml::Shapesf::sphere(5.0f, ml::vec3f(0,0,0), 128, 128);

		sphere.transform(ml::mat4f::translation(ml::vec3f(-6.0053f)));
		sphere.transform(ml::mat4f::rotation(0.0012f, 0.021f, 0.0024f));

		Timer t;
		std::pair<ml::BinaryGrid3, ml::mat4f> grid = sphere.voxelize(0.25f);
		//std::cout << "voxelization time " << t.getElapsedTimeMS() << std::endl;
		ml::PointCloudf pc(grid.first, 1.0f);
		ml::PointCloudIOf::saveToFile("gridcloud1.ply", pc);

		TriMeshf voxelMesh(grid.first, grid.second.getInverse());
		MeshIOf::saveToFile("box.ply", voxelMesh.computeMeshData());

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}


	void test3() 
	{
		{
			//simplest case
			size_t s = 5;
			BinaryGrid3 grid(s, 1, 1);
			grid.setVoxel(0, 0, 0);
			DistanceField3f df(grid);
			std::pair<float,size_t> res = df.evalDist(grid, mat4f::identity());
			float expectedResult = 0.5f * (s * (s - 1.0f));
			MLIB_ASSERT_STR(math::floatEqual(res.first, (float)expectedResult), "mismatch between df - grid comparison");
		}
		{
			//2d test case
			size_t s = 5;
			BinaryGrid3 grid(s, s, 1);
			grid.setVoxel(0, 0, 0);
			grid.setVoxel(s - 1, s - 1, 0);
			DistanceField3f df(grid);
			std::pair<float, size_t> res = df.evalDist(grid, mat4f::identity());
			//for (size_t i = 0; i < s; i++) {
			//	for (size_t j = 0; j < s; j++) {
			//		std::cout << math::round(df(i, j, 0)) << " ";
			//	}
			//	std::cout << std::endl;
			//}

		}


		TriMeshf sphere = Shapesf::sphere(5.0f, vec3f(0, 0, 0), 128, 128);

		sphere.transform(mat4f::translation(ml::vec3f(-6.0053f)));
		sphere.transform(mat4f::rotation(0.0012f, 0.021f, 0.0024f));
		std::pair<BinaryGrid3, mat4f> grid = sphere.voxelize(0.25f * 5.0f);
		
		Timer t;
		DistanceField3f df(grid.first);
		//std::cout << t.getElapsedTimeMS() << " ms" << std::endl;

		MeshIOf::saveToFile("voxelizedSphere.ply", TriMeshf(grid.first).computeMeshData());

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

    void test4()
    {
        Grid3<float> a;
        Grid3<float> b = std::move(a);

		std::cout << __FUNCTION__ << " passed" << std::endl;
    }

	void test5() 
	{
		//testing the binary stream operator for serialization (Grid3)
		vec3ul dim(10, 8, 5);
		Grid3<float> grid(dim);
		auto fillFunc = [](size_t x, size_t y, size_t z) { return (float)(z * 8 * 10 + y * 10 + x); };
		grid.fill(fillFunc); 
		BinaryDataStreamFile out("tmp.bin", true);
		out << grid;
		out.close(); 
		BinaryDataStreamFile in("tmp.bin", false);
		Grid3<float> re;
		in >> re;
		MLIB_ASSERT_STR(grid == re, "binary stream grid3 and re don't match");

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

	void test6()
	{
		//testing the binary stream operator for serialization (Grid2)
		vec2ul dim(10, 8);
		Grid2<float> grid(dim);
		auto fillFunc = [](size_t x, size_t y) { return (float)(y * 10 + x); };
		grid.fill(fillFunc);
		BinaryDataStreamFile out("tmp.bin", true);
		out << grid;
		out.close();
		BinaryDataStreamFile in("tmp.bin", false);
		Grid2<float> re;
		in >> re;
		MLIB_ASSERT_STR(grid == re, "binary stream grid2 and re don't match");

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

	void test7() 
	{
		//test printing
		{
			//binary grid
			BinaryGrid3 grid(2,3,4);
			for (size_t z = 0; z < grid.getDimZ(); z++) {
				for (size_t y = 0; y < grid.getDimY(); y++) {
					for (size_t x = 0; x < grid.getDimX(); x++) {
						if (math::randomCointoss()) {
							grid.setVoxel(x, y, z);
						}
					}
				}
			}
			std::cout << grid << std::endl;
		}

		{
			//grid3
			Grid3f grid(2,3,4);
			grid.fill([](size_t x, size_t y, size_t z){ return math::randomUniform(0.0f, 1.0f);  });
			std::cout << grid << std::endl;
		}

		{
			//grid2
			Grid2f grid(2, 3);
			grid.fill([](size_t x, size_t y){ return math::randomUniform(0.0f, 1.0f);  });
			std::cout << grid << std::endl;
		}

		{
			//base image
			BaseImage<float> image(2, 4);
			image.fill([](size_t x, size_t y){ return math::randomUniform(0.0f, 1.0f);  });
			std::cout << image;

		}

		std::cout << __FUNCTION__ << " passed" << std::endl;
	}

	std::string getName() {
		return "grid";
	}
private:

	bool checkIfAllOtherAreZero(const ml::BinaryGrid3& grid, unsigned int _i, unsigned int _j, unsigned int _k) {
		for (unsigned int i = 0; i < grid.getDimX(); i++) {
			if (i == _i) continue;
			for (unsigned int j = 0; j < grid.getDimY(); j++) {
				if (j == _j) continue;
				for (unsigned int k = 0; k < grid.getDimZ(); k++) {
					if (k == _k) continue;
					if (grid.isVoxelSet(i,j,k)) return false;
				}
			}
		}
		return true;
	}
};