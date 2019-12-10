
class TestBox : public Test
{
public:
	void test0()
	{
		size_t size = 1000;
		std::vector<vec3f> points(size);
		RNG rng;
		for (size_t i = 0; i < size; i++) {
			points[i] = vec3f((float)rng.rand_closed01(), (float)rng.rand_closed01(), (float)rng.rand_closed01());
			points[i].x *= 3.0f;
			points[i].y *= 2.0f;
			points[i] = mat3f::rotationZ(45) * points[i];
		}

		points = CGALWrapperf::convexHull3(points);

		std::cout << "PCA" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::PCA) << std::endl;
		std::cout << "CONSTRAIN_Z" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::CONSTRAIN_Z) << std::endl;
		std::cout << "MIN_PCA" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::MIN_PCA) << std::endl;
		std::cout << "CONSTRAIN_AXIS" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::CONSTRAIN_AXIS, vec3f(0,0,-1).getNormalized()) << std::endl;
		std::cout << "AABB_FALLBACK" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::AABB_FALLBACK) << std::endl;

		std::cout <<"box test0 passed" << std::endl;
	}

	void test1() 
	{
		//BoundingBox3f bb0;	bb0.include(vec3f(0, 0, 0));	bb0.include(vec3f(1, 1, 1));
		//BoundingBox3f bb1;	bb1.include(vec3f(0.9f, 0.9f, 0.9f));	bb1.include(vec3f(1.1f, 1.1f, 1.1f));

		//OrientedBoundingBox3f obb0(bb0);
		//OrientedBoundingBox3f obb1(bb1);

		//bool b = obb0.intersects(obb1);
		//std::cout << "b " << b << std::endl;

		{
			BoundingBox3f bb0;	bb0.include(vec3f(0, 0, 0));	bb0.include(vec3f(1, 1, 1));
			BoundingBox3f bb1;	bb1.include(vec3f(0.5, 0.5, 2));	bb1.include(vec3f(1, 1, 3));

			OrientedBoundingBox3f obb0(bb0);
			OrientedBoundingBox3f obb1(bb1);

			unsigned int numTests = 11;
			for (unsigned int i = 0; i < numTests; i++) {
				float t = (float)i / (numTests-1);
				bb1.translate(vec3f(0,0,2*t));

				bool resultA = bb0.intersects(bb1);
				bool resultB = obb0.intersects(obb1);
				MLIB_ASSERT_STR(resultA == resultB, "collisions inconsistent");
			}
		}

		unsigned int numTests = 1000;
		RNG rng;
		for (unsigned int i = 0; i < 1000; i++) {
			
			BoundingBox3f bb0;	bb0.include(vec3d(rng.rand_closed01(), rng.rand_closed01(), rng.rand_closed01()));	bb0.include(vec3d(rng.rand_closed01(), rng.rand_closed01(), rng.rand_closed01()));
			BoundingBox3f bb1;	bb1.include(vec3d(rng.rand_closed01(), rng.rand_closed01(), rng.rand_closed01()));	bb1.include(vec3d(rng.rand_closed01(), rng.rand_closed01(), rng.rand_closed01()));

			OrientedBoundingBox3f obb0(bb0);
			OrientedBoundingBox3f obb1(bb1);

			bool resultA = bb0.intersects(bb1);
			bool resultB = obb0.intersects(obb1);
			MLIB_ASSERT_STR(resultA == resultB, "collisions inconsistent");
		}

		std::cout <<"box test1 passed" << std::endl;

	}

	void test2() {
		OrientedBoundingBox3f obb = OrientedBoundingBox3f(vec3f::origin, vec3f::eX, vec3f::eY, vec3f::eZ);
		std::cout << "obb before\t" << obb << std::endl;
		obb.scale(1.5);
		std::cout << "obb after\t" << obb << std::endl;

		std::cout <<"box test2 passed" << std::endl;
	}

	std::string getName()
	{
		return "Box";
	}
private:

};
