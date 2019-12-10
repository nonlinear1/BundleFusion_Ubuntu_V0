
struct ConstantBuffer
{
	ml::mat4f worldViewProj;
};

class Vizzer : public ml::ApplicationCallback
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
	
    ml::D3D11VertexShader m_vsColor;
	ml::D3D11PixelShader m_psColor;
    ml::D3D11VertexShader m_vsPointCloud;
    ml::D3D11PixelShader m_psPointCloud;

    ml::D3D11Font m_font;
    ml::FrameTimer m_timer;

	ml::D3D11ConstantBuffer<ConstantBuffer> m_constants;
	ml::Cameraf m_camera;
};