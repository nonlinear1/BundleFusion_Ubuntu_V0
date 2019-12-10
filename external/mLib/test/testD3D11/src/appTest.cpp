
#include "main.h"


unsigned int test0(unsigned int size) {
	std::vector<std::vector<unsigned int>> faces(size, std::vector<unsigned int>(3));
	for (unsigned int i = 0; i < faces.size(); i++) {
		faces[i][0] = i * 3 + 0;
		faces[i][1] = i * 3 + 1;
		faces[i][2] = i * 3 + 2;
	}
	unsigned int sum = 0;
	for (unsigned int i = 0; i < faces.size(); i++) {
		sum += faces[i][0];
		sum += faces[i][1];
		sum += faces[i][2];
	}
	return sum;
}

unsigned int test1(unsigned int size) {
	std::vector<unsigned int> faces(size * 3);
	for (unsigned int i = 0; i < size; i++) {
		faces[3 * i + 0] = i * 3 + 0;
		faces[3 * i + 1] = i * 3 + 1;
		faces[3 * i + 2] = i * 3 + 2;
	}
	unsigned int sum = 0;
	for (unsigned int i = 0; i < size; i++) {
		sum += faces[3 * i + 0];
		sum += faces[3 * i + 1];
		sum += faces[3 * i + 2];
	}
	return sum;
}

unsigned int test2(unsigned int size) {
	MeshDataf::Indices faces;
	faces.reserve(size);
	for (unsigned int i = 0; i < size; i++) {
		unsigned int curr[] = { i * 3 + 0, i * 3 + 1, i * 3 + 2 };
		faces.addFace(curr, 3);
		//faces[3*i+0] = i*3+0;
		//faces[3*i+1] = i*3+1;
		//faces[3*i+2] = i*3+2;
	}
	unsigned int sum = 0;
	for (unsigned int i = 0; i < size; i++) {
		auto f = faces.getFace(i);
		sum += f[0];
		sum += f[1];
		sum += f[2];
		//sum += faces[3*i+0];
		//sum += faces[3*i+1];
		//sum += faces[3*i+2];
	}
	return sum;
}

//unsigned int test3(unsigned int size) {
//	MeshDataf::Indices0 faces;
//	faces.reserve(size);
//	for (unsigned int i = 0; i < size; i++) {
//		unsigned int curr[] = {i*3+0, i*3+1, i*3+2};
//		faces.addFace(curr, 3);
//		//faces[3*i+0] = i*3+0;
//		//faces[3*i+1] = i*3+1;
//		//faces[3*i+2] = i*3+2;
//	}
//	unsigned int sum = 0;
//	for (unsigned int i = 0; i < size; i++) {
//		auto f = faces.getFace(i);
//		sum += f[0];
//		sum += f[1];
//		sum += f[2];
//		//sum += faces[3*i+0];
//		//sum += faces[3*i+1];
//		//sum += faces[3*i+2];
//	}
//	return sum;
//}

void testCollisions() {
	BoundingBox3f bb0(vec3f(0, 0, 0), vec3f(1, 1, 1));
	BoundingBox3f bb1(vec3f(-1, -1, -1), vec3f(-0.5f, 0.5f, 0.5f));
	for (unsigned int i = 0; i < 10000; i++) {
		bb1.translate(vec3f(0.001f, 0.001f, 0.001f));
		if (bb0.intersects(bb1)) {
			std::cout << i << std::endl;
			std::cout << bb1 << std::endl;
			break;
		}
	}

	{
		Timer cTime;
		TriMeshf s0 = Shapesf::sphere(0.5f, vec3f(-2.0f + 1300 * 0.002f), 50, 50);
		TriMeshf s1 = Shapesf::sphere(0.5f, vec3f(1.0f), 15, 15);
		TriMeshAcceleratorBVHf accels0(s0);
		TriMeshAcceleratorBVHf accels1(s1);
		unsigned int numCols = 0;
		for (unsigned int i = 0; i < 10000; i++) {
			if (accels0.collision(accels1)) {
				numCols++;
			}
		}
		std::cout << "bvh collision time:\t" << cTime.getElapsedTimeMS() << " ms | " << numCols << std::endl;
	}

	Timer tbvh;
	for (unsigned int i = 0; i < 10000; i++) {
		TriMeshf s0 = Shapesf::sphere(0.5f, vec3f(-2.0f + i*0.002f), 50, 50);
		TriMeshf s1 = Shapesf::sphere(0.5f, vec3f(1.0f), 15, 15);
		TriMeshAcceleratorBVHf accels0(s0);
		TriMeshAcceleratorBVHf accels1(s1);
		//std::cout << i << std::endl;
		if (accels0.collision(accels1)) {
			std::cout << "BVH:\t" << i << std::endl;
			break;
		}
	}
	std::cout << "tbvh:\t" << tbvh.getElapsedTimeMS() << std::endl;

	Timer tbrute;
	for (unsigned int i = 0; i < 10000; i++) {
		TriMeshf s0 = Shapesf::sphere(0.5f, vec3f(-2.0f + i*0.002f), 50, 50);
		TriMeshf s1 = Shapesf::sphere(0.5f, vec3f(1.0f), 15, 15);
		TriMeshAcceleratorBruteForcef accels0(s0);
		TriMeshAcceleratorBruteForcef accels1(s1);
		//std::cout << i << std::endl;
		if (accels0.collision(accels1)) {
			std::cout << "BruteForce:\t" << i << std::endl;
			break;
		}
	}
	std::cout << "tbrute:\t" << tbrute.getElapsedTimeMS() << std::endl;
}

void AppTest::init(ml::ApplicationData &app)
{
	MeshDataf meshData = MeshIOf::loadFromFile("scans/gates381.ply");
	ml::TriMeshf triMesh(meshData);
	//triMesh.setColor(vec4f(0.0f, 1.0f, 0.0f, 1.0f));
	//auto p = meshData.getBoundingBox().getBottomPlane();

;
	std::vector<ml::TriMeshf> meshes;
	meshes.push_back(triMesh);

	ml::TriMeshf unifiedMesh = ml::meshutil::createUnifiedMesh(meshes);

	m_mesh.init(app.graphics, unifiedMesh);

	auto lambdaPoints = [=](ml::vec3f& v) { v = ml::vec3f(-2.f*(float)rand() / RAND_MAX, -2.f*(float)rand() / RAND_MAX, (float)rand() / RAND_MAX); };
	std::vector<ml::vec3f> points(5000);
	std::for_each(points.begin(), points.end(), lambdaPoints);

	m_pointCloud.init(app.graphics, ml::meshutil::createPointCloudTemplate(ml::Shapesf::box(0.01f), points));

	m_shaderManager.init(app.graphics);
	m_shaderManager.registerShader("shaders/pointCloud.hlsl", "pointCloud");
	//m_shaderManager.registerShader("shaders/test.hlsl", "geometryShaderTest");
	m_shaderManager.registerShaderWithGS("shaders/test.hlsl", "geometryShaderTest");
	
	m_constants.init(app.graphics);

	bbox3f bb = unifiedMesh.computeBoundingBox();
	std::cout << bb << std::endl;


	ml::vec3f eye(-0.5f, -0.5f, 1.5f);
	//m_camera = Cameraf(eye, vec3f::eZ, -vec3f::eY, 60.0f, (float)app.window.getWidth() / app.window.getHeight(), 0.01f, 10.0f);		//same as idenity matrix
	m_camera = Cameraf(eye, -vec3f::eY, vec3f::eZ, 60.0f, (float)app.window.getWidth() / app.window.getHeight(), 0.01f, 10.0f);

	//mat4f view = mat4f::identity();
	//m_camera = Cameraf(view, 60.0f, (float)app.window.getWidth() / app.window.getHeight(), 0.01f, 10.0f);	
	//m_camera.updateWorldUp(vec3f::eZ);


	m_font.init(app.graphics, "Calibri");


	m_canvas.init(app.graphics);
	m_canvas.addCircle(vec2f(500, 100), 50, RGBColor::Green, 0.51f);
	ColorImageR8G8B8A8 image(500, 500);
	image.setPixels(RGBColor::Blue);
	for (unsigned int x = 0; x < 500; x++) {
		for (unsigned int y = 0; y < 250; y++)
			image(x, y) = RGBColor::Red;
	}

	//ColorImageR8G8B8A8 image; FreeImageWrapper::loadImage("refined.png", image);
	//Bitmap bmp(image.getWidth(), image.getHeight());
	//for (unsigned int i = 0; i < image.getWidth()*image.getHeight(); i++) {
	//	const vec4uc& p = image.getDataPointer()[i];
	//	bmp.ptr()[i] = RGBColor(p.x, p.y, p.z, p.w);
	//}
	m_canvas.addBillboard(bbox2i(vec2i(100, 100), vec2i(500, 500)), image, 0.5f);
	m_canvas.addBillboard(bbox2i(vec2i(100, 100), vec2i(500, 500)), image, 0.5f);
	m_canvas.addBox(bbox2i(vec2i(50, 50), vec2i(10, 10)), RGBColor::Yellow, 0.4f);

	std::vector<vec4f> bufferData(m_mesh.getTriMesh().getVertices().size());
	for (size_t i = 0; i < m_mesh.getTriMesh().getVertices().size(); i++) {
		bufferData[i] = m_mesh.getTriMesh().getVertices()[i].color;
		//bufferData[i].x = 0.0f;
	}
	m_buffer.init(app.graphics, bufferData);


	D3D11Canvas2D canvas = m_canvas;
	D3D11Buffer<vec4f> tmp = m_buffer;

	//app.graphics.castD3D11().printAssets();
	std::cout << "\nInit done!\n\n" << std::endl;

	std::cout << "NDC: " << D3D11GraphicsDevice::pixelToNDC(vec2i(0, 0), 640, 480) << std::endl;
	std::cout << "NDC: " << D3D11GraphicsDevice::pixelToNDC(vec2i(640-1, 0), 640, 480) << std::endl;
	std::cout << "NDC: " << D3D11GraphicsDevice::pixelToNDC(vec2i(0, 480-1), 640, 480) << std::endl;
	std::cout << "NDC: " << D3D11GraphicsDevice::pixelToNDC(vec2i(640-1, 480-1), 640, 480) << std::endl;
}

void AppTest::render(ml::ApplicationData& app)
{
	m_timer.frame();

	m_canvas.render();

	ConstantBuffer constants;
	mat4f proj = Cameraf::visionToGraphicsProj(app.window.getWidth(), app.window.getHeight(), 1108.51f, 1108.51f, m_camera.getNearPlane(), m_camera.getFarPlane());
	//constants.worldViewProj = proj * m_camera.getView();
	
	constants.worldViewProj = m_camera.getViewProj();
	constants.modelColor = ml::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	m_constants.updateAndBind(constants, 0);
	//app.graphics.castD3D11().getShaderManager().bindShaders("defaultBasic");
	m_shaderManager.bindShaders("geometryShaderTest");

	m_buffer.bindSRV(0);
	m_mesh.render();
	m_buffer.unbindSRV(0);

	m_shaderManager.bindShaders("pointCloud");
	m_constants.bind(0);
	m_pointCloud.render();
	
	m_font.drawString("FPS: " + ml::convert::toString(m_timer.framesPerSecond()), ml::vec2i(10, 5), 24.0f, ml::RGBColor::Red);
}

void AppTest::resize(ml::ApplicationData &app)
{
	m_canvas.resize();
	m_camera.updateAspectRatio((float)app.window.getWidth() / app.window.getHeight());
}

void AppTest::keyDown(ml::ApplicationData& app, UINT key)
{
	if (key == KEY_Z) {
		const unsigned int width = app.window.getWidth();
		const unsigned int height = app.window.getHeight();
		mat4f intrinsic = Cameraf::graphicsToVisionProj(m_camera.getProj(), width, height);
		mat4f extrinsic = m_camera.getExtrinsic();

		std::cout << "intrinsic before " << intrinsic << std::endl;
		std::cout << "extrinsic before " << extrinsic << std::endl;
		std::cout << "camera before: " << m_camera.toString() << std::endl;
		Cameraf newCam = Cameraf::visionToGraphics(extrinsic, width, height, intrinsic(0, 0), intrinsic(1, 1), m_camera.getNearPlane(), m_camera.getFarPlane());
		newCam.updateWorldUp(m_camera.getWorldUp());
		m_camera = newCam;

		intrinsic = Cameraf::graphicsToVisionProj(m_camera.getProj(), width, height);
		extrinsic = m_camera.getExtrinsic();
		std::cout << "camera after: " << m_camera.toString() << std::endl;
		std::cout << "intrinsic after " << intrinsic << std::endl;
		std::cout << "extrinsic after " << extrinsic << std::endl; 
	}
}


void backProjectGraphics(const DepthImage32& depth, const ColorImageR8G8B8A8& color, const Cameraf& camera) {
	PointCloudf pc;
	mat4f inv = camera.getProj().getInverse();
	for (auto& d : depth) {
		if (depth.isValidValue(d.value)) {
			vec2f p = D3D11GraphicsDevice::pixelToNDC(vec2i((int)d.x, (int)d.y), depth.getWidth(), depth.getHeight());
			vec3f v = (inv * vec3f((float)p.x, (float)p.y, d.value));
			pc.m_points.push_back(v);
			pc.m_colors.push_back(vec4f(color(d.x, d.y)) / 255.0f);
		}
	}
	PointCloudIOf::saveToFile("test_backproj_graphics.ply", pc);
}

void backProjectVision(const DepthImage32& _depth, const ColorImageR8G8B8A8& color, const Cameraf& camera) {
	DepthImage32 depth = _depth;

	{
		//first get it to world space z
		mat4f inv = camera.getProj().getInverse();
		for (auto& d : depth) {
			if (depth.isValidValue(d.value)) {
				vec2f p = D3D11GraphicsDevice::pixelToNDC(vec2i((int)d.x, (int)d.y), depth.getWidth(), depth.getHeight());
				vec3f v = (inv * vec3f((float)p.x, (float)p.y, d.value));
				d.value = v.z;
			}
			else {
				d.value = 0.0f;
			}
		}
		depth.setInvalidValue(0.0f);
	}

	PointCloudf pc;
	mat4f inv = camera.getIntrinsic(depth.getWidth(), depth.getHeight()).getInverse();
	for (auto& d : depth) {
		if (depth.isValidValue(d.value)) {
			vec3f p((float)d.x * d.value, (float)d.y * d.value, d.value);
			p = inv * p;

			pc.m_points.push_back(p);
			pc.m_colors.push_back(vec4f(color(d.x, d.y)) / 255.0f);
		}
	}
	PointCloudIOf::saveToFile("test_backproj_vision.ply", pc);
}

void AppTest::keyPressed(ml::ApplicationData& app, UINT key)
{
	const float distance = 0.1f;
	const float theta = 5.0f;

	if (key == KEY_S) m_camera.move(-distance);
	if (key == KEY_W) m_camera.move(distance);
	if (key == KEY_A) m_camera.strafe(-distance);
	if (key == KEY_D) m_camera.strafe(distance);
	if (key == KEY_E) m_camera.jump(-distance);
	if (key == KEY_Q) m_camera.jump(distance);

	if (key == KEY_UP) m_camera.lookUp(theta);
	if (key == KEY_DOWN) m_camera.lookUp(-theta);
	if (key == KEY_LEFT) m_camera.lookRight(theta);
	if (key == KEY_RIGHT) m_camera.lookRight(-theta);

	if (key == KEY_F1) {
		auto color = app.graphics.castD3D11().captureBackBufferColor();
		auto depth = app.graphics.castD3D11().captureBackBufferDepth();
		FreeImageWrapper::saveImage("screenshot_color.png", color);
		FreeImageWrapper::saveImage("screenshot_depth.png", ColorImageR32G32B32A32(depth));
	}


	if (key == KEY_C) {
		std::cout << "proj:\n";
		std::cout << m_camera.getProj() << std::endl;
		std::cout << "view:\n";
		std::cout << m_camera.getView() << std::endl;
		std::cout
			<< m_camera.getEye() << "\n"
			<< m_camera.getRight() << "\n"
			<< m_camera.getUp() << "\n"
			<< m_camera.getLook() << "\n" << std::endl;		
	}

	if (key == KEY_P) {
		auto color = app.graphics.castD3D11().captureBackBufferColor();
		auto depth = app.graphics.castD3D11().captureBackBufferDepth();
		
		backProjectGraphics(depth, color, m_camera);
		backProjectVision(depth, color, m_camera);		
	}
	
	if (key == KEY_F2) {
		ml::D3D11RenderTarget renderTarget;
		renderTarget.init(app.graphics.castD3D11(), app.window.getWidth(), app.window.getHeight(), std::vector < DXGI_FORMAT > {DXGI_FORMAT_R8G8B8A8_UNORM}, true);
		renderTarget.clear();
		renderTarget.bind();
		render(app);
		renderTarget.unbind();
		ml::ColorImageR8G8B8A8 color;
		ml::DepthImage32 depth;
		renderTarget.captureColorBuffer(color);
		renderTarget.captureDepthBuffer(depth);
		FreeImageWrapper::saveImage("color.png", color);
		FreeImageWrapper::saveImage("depth.png", ml::ColorImageR32G32B32A32(depth));


		//auto depth2 = app.graphics.castD3D11().captureBackBufferDepth();
	}

	if (key == 'R') {
		//float fovX = m_camera.getFoV();
		//float fovY = fovX/m_camera.getAspect();
		//float focalLengthX = 0.5f * (float)app.window.width() / tan(0.5f * math::degreesToRadians(fovX));
		//float focalLengthY = 0.5f * (float)app.window.height() / tan(0.5f * math::degreesToRadians(fovY));
		//mat4f intrinsics = 
		//	ml::mat4f(
		//	focalLengthX, 0.0f, (float)app.window.width()/2.f, 0.0f,
		//	0.0f, focalLengthY, (float)app.window.height()/2.f, 0.0f,
		//	0.0f, 0.0f, 1.0f, 0.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f);
		//std::cout << intrinsics << std::endl;
		mat4f intrinsics = m_camera.getIntrinsic(app.window.getWidth(), app.window.getHeight());
		
		//std::cout << intrinsics << std::endl;
		mat4f intrinsicsInverse = intrinsics.getInverse();

		ml::mat4f projToCam = m_camera.getProj().getInverse();
		ml::mat4f camToWorld = m_camera.getView().getInverse();
		ml::ColorImageR32G32B32 image(app.window.getWidth(), app.window.getHeight());

		const std::string testFilename = "scans/gates381.ply";
		ml::MeshDataf meshData = ml::MeshIOf::loadFromFile(testFilename);
		ml::TriMeshf triMesh(meshData);

		//std::cout << triMesh.getBoundingBox() << std::endl;

		ml::Timer c0;
		c0.start();
		ml::TriMeshAcceleratorBVHf accel(triMesh, false);
		//ml::TriMeshAcceleratorBruteForcef accel(triMesh);
		std::cout << "time construct " << c0.getElapsedTimeMS() << std::endl;

		std::vector<const TriMeshRayAcceleratorf*> accelVec;
		accelVec.push_back(&accel);
		int s = sizeof(accel);

		ml::Timer c;
		c.start();
#pragma omp parallel for
		for (int y_ = 0; y_ < (int)app.window.getHeight(); y_++) {
			unsigned int y = (unsigned int)y_;

			for (unsigned int x = 0; x < app.window.getWidth(); x++) {

				float depth0 = 0.5f;
				float depth1 = 1.0f;
				vec4f p0 = camToWorld*intrinsicsInverse*vec4f((float)x*depth0, (float)(y)*depth0, depth0, 1.0f);
				vec4f p1 = camToWorld*intrinsicsInverse*vec4f((float)x*depth1, (float)(y)*depth1, depth1, 1.0f);

				vec3f eye = m_camera.getEye();
				Rayf r(m_camera.getEye(), (p1.getVec3() - p0.getVec3()).getNormalized());

				Rayf _check = m_camera.getScreenRay((float)x / app.window.getWidth(), (float)y / app.window.getHeight());

				//r = _check;
				int a = 5;

				//mat4f tmp = mat4f::rotationZ(45.0f);
				//r = tmp * r;

				//ml::vec4f p((float)j, (float)i, 0.5f, 1.0f);
				//p.x /= app.window.width();
				//p.y /= app.window.height();
				//p.x = 2.0f*p.x - 1.0f;
				//p.y = 1.0f-2.0f*p.y;
				//p = trans * p;
				//p /= p.w;
				//Rayf r1(m_camera.getEye(), (ml::vec3f(p.x,p.y,p.z)-m_camera.getEye()).getNormalized());

				float t, u, v;
				//const ml::TriMeshf::Trianglef* tri = accel.intersect(r, t, u, v);
				const ml::TriMeshf::Triangle* tri;
				unsigned int objIdx;
				TriMeshRayAcceleratorf::Intersection intersect = TriMeshRayAcceleratorf::getFirstIntersection(r, accelVec, objIdx);
				t = intersect.t;
				u = intersect.u;
				v = intersect.v;
				tri = intersect.triangle;

				if (tri) {
					image(x, y) = tri->getSurfaceColor(u, v).getVec3();
				}
				else {
					image(x, y) = 0;
				}

			}
		}
		double elapsed = c.getElapsedTimeMS();
		std::cout << "time trace " << elapsed << std::endl;
		unsigned int raysPerSec = (unsigned int)((double)(app.window.getHeight()*app.window.getWidth()) / (elapsed / 1000.0));
		std::cout << "million rays/s " << (float)raysPerSec / 1000000.0 << std::endl;

		ml::FreeImageWrapper::saveImage("test.jpg", image);

	}


}

void AppTest::mouseDown(ml::ApplicationData &app, ml::MouseButtonType button)
{

}

void AppTest::mouseWheel(ml::ApplicationData &app, int wheelDelta)
{
	const float distance = 0.002f;
	m_camera.move(distance * wheelDelta);
}

void AppTest::mouseMove(ml::ApplicationData &app)
{
	const float distance = 0.01f;
	const float theta = 0.5f;

	ml::vec2i posDelta = app.input.mouse.pos - app.input.prevMouse.pos;

	if (app.input.mouse.buttons[ml::MouseButtonRight])
	{
		m_camera.strafe(distance * posDelta.x);
		m_camera.jump(distance * posDelta.y);
	}

	if (app.input.mouse.buttons[ml::MouseButtonLeft])
	{
		m_camera.lookRight(theta * posDelta.x);
		m_camera.lookUp(theta * posDelta.y);
	}

}
