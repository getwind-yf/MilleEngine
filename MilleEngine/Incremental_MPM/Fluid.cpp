
#include <algorithm>
#include <math.h> 
#include <stdio.h> 

// #include "../lodepng/lodepng.h" 

using namespace std;

// Matrixless Gauss-Seidel Solver 

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

	void at(double x, double y) {} 



};


class FluidSolver {

	FluidQuantity* d; 
	FluidQuantity* u;
	FluidQuantity* v;

	int width; 
	int height; 

	double hx; 
	double density; 

	double* r; 
	double* pressure; 

	// Pressure right hand side as the negative divergence 
	void buildRhs() {

		double scale = 1.0 / hx; 

		for (int y = 0, idx = 0; y < height; y++) {
			for (int x = 0; x < width; x++, idx++) {
				r[idx] = -scale * (u->at(x + 1, y) - u->at() + v->at() - v->at());
			}
		}
	}


	// Perform pressure solve using Gauss-Seidel 

	void project(int limit, double timestep) {
		
		double scale = timestep / (density * hx * hx); 

		double maxDelta; 

		for (int iter = 0; iter < limit; iter++)
		{
			maxDelta = 0.0f; 
			for (int y = 0, int idx = 0; y < height; y++) {
				for (int x = 0; x < width; x++, idx++) {
					int idx = x + y * width; 

					double diag = 0.0f, offDiag = 0.0; 


					// build the matrix implicitly as the five-point stencil, Grid boarders are assumed to be solid 

					if (x > 0) {
						diag += scale; 
						offDiag -= scale * p; 
					}
				}
			}
		}
	}

};