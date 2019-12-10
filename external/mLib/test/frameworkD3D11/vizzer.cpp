
#include "main.h"

void Vizzer::init(ml::ApplicationData &app)
{
	TriMeshf torus = Shapesf::torus(vec3f::origin, 1.0f, 0.2f, 20, 20);
	MeshDataf torusMesh = torus.getMeshData();
	torusMesh.computeVertexNormals();
	MeshIOf::saveToFile("test.obj", torusMesh);

	TriMeshf scan = MeshIOf::loadFromFile("gates381.ply");

	//m_mesh.load(app.graphics, torus);
	m_mesh.load(app.graphics, scan);


	m_vsColor.load(app.graphics, "shaders/test.shader");
	m_psColor.load(app.graphics, "shaders/test.shader");

	m_constants.init(app.graphics);

	ml::vec3f eye(-0.5f, -0.5f, 1.5f);
	//m_camera = ml::Cameraf(eye, worldUp, ml::vec3f::eX, 60.0f, (float)app.window.getWidth() / app.window.getHeight(), 0.01f, 1000.0f);
	m_camera = ml::Cameraf(eye, vec3f::eY, ml::vec3f::eZ, 60.0f, (float)app.window.getWidth() / app.window.getHeight(), 0.01f, 1000.0f);

	m_font.init(app.graphics, "Calibri");

}

void Vizzer::render(ml::ApplicationData &app)
{
	m_timer.frame();

	ConstantBuffer constants;
	constants.worldViewProj = m_camera.getCameraPerspective();
	m_constants.update(constants);

	m_vsColor.bind();
	m_psColor.bind();
	m_constants.bindVertexShader(0);

	m_mesh.render();

	m_font.drawString(app.graphics, "FPS: " + ml::convert::toString(m_timer.framesPerSecond()), ml::vec2i(10, 5), 24.0f, ml::RGBColor::Red);
}

void Vizzer::resize(ml::ApplicationData &app)
{
	m_camera.updateAspectRatio((float)app.window.getWidth() / app.window.getHeight());
}

void Vizzer::keyDown(ml::ApplicationData &app, UINT key)
{

}

void Vizzer::keyPressed(ml::ApplicationData &app, UINT key)
{
	const float distance = 0.1f;
	const float theta = 5.0f;

	if (key == KEY_S) m_camera.move(-distance);
	if (key == KEY_W) m_camera.move(distance);
	if (key == KEY_A) m_camera.strafe(-distance);
	if (key == KEY_D) m_camera.strafe(distance);
	if (key == KEY_E) m_camera.jump(distance);
	if (key == KEY_Q) m_camera.jump(-distance);

	if (key == KEY_UP) m_camera.lookUp(theta);
	if (key == KEY_DOWN) m_camera.lookUp(-theta);
	if (key == KEY_LEFT) m_camera.lookRight(theta);
	if (key == KEY_RIGHT) m_camera.lookRight(-theta);
}

void Vizzer::mouseDown(ml::ApplicationData &app, ml::MouseButtonType button)
{

}

void Vizzer::mouseWheel(ml::ApplicationData &app, int wheelDelta)
{
	const float distance = 0.002f;
	m_camera.move(distance * wheelDelta);
}

void Vizzer::mouseMove(ml::ApplicationData &app)
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
		m_camera.lookRight(-theta * posDelta.x);
		m_camera.lookUp(theta * posDelta.y);
	}

}
