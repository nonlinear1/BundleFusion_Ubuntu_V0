//class TestMath : public Test
//{
//public:
//	void test0()
//	{
//		using namespace ml;
//		SparseMatrix<double> a("{{2,6,-1},{-9,-7,3}}", MatrixStringFormatMathematica);
//		SparseMatrix<double> b("{{3,4,1},{2,5,0},{-6,6,-1},{8,7,-2}}", MatrixStringFormatMathematica);
//		SparseMatrix<double> correctProduct("{{29,34,25,60},{-52,-53,9,-127}}", MatrixStringFormatMathematica);
//		
//		SparseMatrix<double> product = a * b.transpose();
//		SparseMatrix<double> sum = a + a;
//		SparseMatrix<double> sumCheck = a * 2.0;
//
//		double productError = (product - correctProduct).maxMagnitude();
//		MLIB_ASSERT_STR(productError == 0.0, "matrix product test failed");
//
//		double sumError = (sumCheck - sum).maxMagnitude();
//		MLIB_ASSERT_STR(sumError == 0.0, "matrix sum test failed");
//
//		LinearSolverConjugateGradient<double> solver;
//		MathVector<double> rhs = SparseMatrix<double>("{{3,6,-1}}", MatrixStringFormatMathematica).denseRow(0);
//		MathVector<double> x = solver.solve(b.transpose() * b, rhs);
//		double solveError = solver.solveError(b.transpose() * b, x, rhs);
//
//		MLIB_ASSERT_STR(solveError <= 1e-5, "solve failed");
//
//		std::cout << "sparse matrix test passed" << std::endl;
//	}
//
//	void test1()
//	{
//		using namespace ml;
//		DenseMatrix<double> a("{{2,6,-1},{-9,-7,3}}", MatrixStringFormatMathematica);
//		DenseMatrix<double> b("{{3,4,1},{2,5,0},{-6,6,-1},{8,7,-2}}", MatrixStringFormatMathematica);
//		DenseMatrix<double> correctProduct("{{29,34,25,60},{-52,-53,9,-127}}", MatrixStringFormatMathematica);
//
//		DenseMatrix<double> product = a * b.transpose();
//		DenseMatrix<double> sum = a + a;
//		DenseMatrix<double> sumCheck = a * 2.0;
//
//		double productError = (product - correctProduct).maxMagnitude();
//		MLIB_ASSERT_STR(productError == 0.0, "matrix product test failed");
//
//		double sumError = (sumCheck - sum).maxMagnitude();
//		MLIB_ASSERT_STR(sumError == 0.0, "matrix sum test failed");
//
//		DenseMatrix<double> aTa = a * a.transpose();
//		DenseMatrix<double> inverseCheck = aTa.inverse() * aTa + aTa * aTa.inverse() - DenseMatrix<double>::identity(aTa.rows()) * 2.0;
//		double inverseError = inverseCheck.maxMagnitude();
//		MLIB_ASSERT_STR(inverseError < 1e-6, "matrix inverse test failed");
//
//		std::cout << "dense matrix test passed" << std::endl;
//	}
//
//	void test2()
//	{
//		using namespace ml;
//		SparseMatrix<double> A(100, 10);
//		MathVector<double> b(100);
//
//		for(UINT row = 0; row < A.rows(); row++)
//			for(UINT col = 0; col < A.cols(); col++)
//				A(row, col) = ((double)rand() / (double)RAND_MAX - 0.5) * 100.0;
//
//		for(UINT val = 0; val < b.size(); val++)
//			b[val] = ((double)rand() / (double)RAND_MAX - 0.5) * 100.0;
//
//		MathVector<double> bSquare = A.transpose() * b;
//
//		SparseMatrix<double> ASquare = A.transpose() * A;
//
//		LinearSolverConjugateGradient<double> linearSolver;
//		LinearSolverEigen<double> eigenSolver(LinearSolverEigen<double>::ConjugateGradient_Diag);
//
//		MathVector<double> x0 = linearSolver.solve(ASquare, bSquare);
//		MathVector<double> x1 = eigenSolver.solve(ASquare, bSquare);
//		MathVector<double> x2 = linearSolver.solveLeastSquares(A, b);
//		MathVector<double> x3 = eigenSolver.solveLeastSquares(A, b);
//
//		double solveError0 = LinearSolver<double>::solveError(ASquare, x0, bSquare);
//		double solveError1 = LinearSolver<double>::solveError(ASquare, x1, bSquare);
//		double solveError2 = LinearSolver<double>::solveError(ASquare, x2, bSquare);
//		double solveError3 = LinearSolver<double>::solveError(ASquare, x3, bSquare);
//
//		MLIB_ASSERT_STR(solveError0 <= 1e-5, "solve failed");
//		MLIB_ASSERT_STR(solveError1 <= 1e-5, "solve failed");
//		MLIB_ASSERT_STR(solveError2 <= 1e-5, "solve failed");
//		MLIB_ASSERT_STR(solveError3 <= 1e-5, "solve failed");
//
//		std::cout << "Eigen test passed" << std::endl;
//	}
//
//	void test3()
//	{
//		using namespace ml;
//		const UINT pointCount = 1000;
//		const UINT clusterCount = 10;
//		const UINT maxIterations = 25;
//
//		std::vector<vec3f> points(pointCount);
//		for(UINT i = 0; i < pointCount; i++)
//			points[i] = vec3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
//
//		for(UINT iteration = 0; iteration < 2; iteration++)
//		{
//			Timer timer;
//			KMeansClustering<vec3f, vec3fKMeansMetric> clustering;
//			clustering.cluster(points, clusterCount, maxIterations, false);
//			std::cout << "iteration " << iteration << ": " << timer.getElapsedTime() << std::endl;
//		}
//		
//		std::cout << "clustering test passed" << std::endl;
//	}
//
//	std::string name()
//	{
//		return "math";
//	}
//};