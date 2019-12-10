
class TestCGAL : public Test {
public:
	void test0() 
	{
		{
			std::vector<vec3f> points;
			points.push_back(vec3f(0, 0, 0));
			points.push_back(vec3f(0, 1, 0));
			points.push_back(vec3f(1, 1, 0));
			points.push_back(vec3f(1, 0, 0));
			points.push_back(vec3f(0.5f, 0.5f, 0));

			std::vector<vec3f> convexHull = CGALWrapper<float>::convexHull3(points);
		}

		{
			size_t size = 100000;
			std::vector<vec3f> points(size);

			RNG rng;
			for (size_t i = 0; i < size; i++) {
				points[i].x = (float)rng.rand_closed01();
				points[i].y = (float)rng.rand_closed01();
				points[i].z = (float)rng.rand_closed01();
			}

			PointCloudIOf::saveToFile("original.ply", PointCloudf(points));
			std::vector<vec3f> convexHull = CGALWrapper<float>::convexHull3(points);
			PointCloudIOf::saveToFile("convexHull.ply", PointCloudf(convexHull));
		}
		std::cout <<"CGAL test0 passed" << std::endl;
	}


	std::string getName() {
		return "cgal";
	}

private:
};