
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


// Set up Fluid quantities, forces incompressiblity, performs advection and adds inflows  

class FluidSolver {

	// Fluid Particles/ Quantities  
	FluidQuantity* d; 
	FluidQuantity* u;
	FluidQuantity* v;
	
	// Width and Height  
	int width; 
	int height; 
	// Grid cell size and fluid density 
	double hx; 
	double density; 
	// r: right hand side of pressure solve ; pressure : pressure solution 
	double* r;    
	double* pressure; 

	// 根据方程构建压力的右手作为负的散度 
	void buildRhs() {

		double scale = 1.0 / hx; 

		for (int y = 0, idx = 0; y < height; y++) {
			for (int x = 0; x < width; x++, idx++) {
				r[idx] = -scale * (u->at(x + 1, y) - u->at(x, y) +
					v->at(x, y + 1) - v->at(x, y));
			}
		}
	}


	// Perform pressure solve using Gauss-Seidel  
	// The solver will run as long as it takes to get the relative error below a threshold 

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


					// Build the matrix implicitly as the five-point stencil, Grid boarders are assumed to be solid 
					// There is no fluid outside the simulation domain 

					if (x > 0) {
						diag	+= scale; 
						offDiag -= scale * pressure[idx -1];  
					}
					if (y > 0) {
						diag += scale; 
						offDiag -= scale * pressure[idx - width]; 
					}
					if (x < width - 1) {
						diag += scale; 
						offDiag -= scale * pressure[idx + 1]; 
					}
					if (y < height - 1) {
						diag += scale; 
						offDiag -= scale * pressure[idx + width]; 
					}

					double newP = (r[idx] - offDiag) / diag; 
					maxDelta = max(maxDelta, fabs(pressure[idx] - newP)); 
					pressure[idx] = newP; 

				}
			}

			if (maxDelta < 1e-5) {
				return; 
			}
		}
	}

};