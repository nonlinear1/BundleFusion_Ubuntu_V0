
struct ConstantBuffer
{
	ml::mat4f worldViewProj;
	ml::vec4f modelColor;
};

class AppTest : public ml::ApplicationCallback
{
public:
	void init(ml::ApplicationData &app);
	void render(ml::ApplicationData &app);
	void keyDown(ml::ApplicationData &app, UINT key);
	void keyPressed(ml::ApplicationData &app, UINT key);
	void mouseDown(ml::ApplicationData &app, ml::MouseButtonType button);
	void mouseMove(ml::ApplicationData &app);
	void mouseWheel(ml::ApplicationData &app, int wheelDelta);
	void resize(ml::ApplicationData &app);

private:
	ml::D3D11TriMesh m_mesh, m_pointCloud;
	
	ml::D3D11ShaderManager m_shaderManager;

    ml::D3D11Font m_font;
    ml::FrameTimer m_timer;

	ml::D3D11ConstantBuffer<ConstantBuffer> m_constants;
	ml::D3D11Canvas2D m_canvas;
	ml::Cameraf m_camera;

	ml::D3D11Buffer<vec4f> m_buffer;
};