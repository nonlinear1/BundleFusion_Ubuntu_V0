
template <class T>
class p3d
{
public:
	p3d() {
		x = 0;
	}

	p3d(const T &_x) {
		x = _x;
	}

	static const p3d<T> origin;
	
	T x;
};

typedef p3d<float> p3df;

int main2()
{
	p3df p = p3df::origin;
	return 0;
}

template<> const p3df p3df::origin(0.0f);