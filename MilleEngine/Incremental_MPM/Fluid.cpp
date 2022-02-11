
#include <algorithm>
#include <math.h> 
#include <stdio.h> 

// #include "../lodepng/lodepng.h" 

using namespace std;

class FluidQuantity {

public: 
	double* m_src; 
	double* m_dst; 

	int m_width; 
	int m_height; 

	// X and Y offset from top left grid cell 
	double m_ox; 
	double m_oy; 
	double m_hx; 


public: 
	FluidQuantity(int w, int h, double ox, double oy, double hx) :
		m_width(w), m_height(h), m_ox(ox), m_oy(oy), m_hx(hx) {

		m_src = new double[m_width * m_height]; 
		m_dst = new double[m_width * m_height]; 

		memset(m_src, 0, m_width * m_height * sizeof(double));
	}

	~FluidQuantity() {
		delete[] m_src; 
		delete[] m_dst; 
	}
	
	void flip() {
		swap(m_src, m_dst); 
	}

	const double* src() const {
		return m_src; 
	}

public:

	double lerp(double a, double b, double x) const {
		return a * (1.0 - x) + b * x; 
	}

	// Simple forward Euler method for velocity integration 
	// Here lost one param x 
	void euler(double& x, double& y, double timestep, const FluidQuantity& u, const FluidQuantity& v) const {
		double uVel = u.lerp(x, y, x) / m_hx;
		double vVel = v.lerp(x, y, x) / m_hx;

		x -= uVel * timestep; 
		y -= vVel * timestep; 

	}




};