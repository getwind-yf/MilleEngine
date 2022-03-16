#pragma once

#include <algorithm> 
#include <math.h> 
#include <stdio.h> 

// Fluid Particles 
class FluidQuantity {

	double* _src; 
	double* _dst; 

	int _w; 
	int _h; 

	double _ox; 
	double _oy; 
	double _hx; 

	double lerp1(double a, double b, double x) const {
		return a * (1.0 - x) + b * x; 
	} 
	
	// Read and Read Write access to grid cells  
	double at(int x, int y) const {
		return _src[x + y * _w]; 
	}

	//double &at(int x, int y) const {
	//	return _src[x + y * _w];
	//}

	// Linear Interpolate on grid at (x,y) 
	double lerp(double x, double y) const {
		x = std::min(std::max(x - _ox, 0.0), _w -1.001); 
		y = std::min(std::max(y - _oy, 0.0), _h - 1.001); 
		int ix = (int)x; 
		int iy = (int)y; 
		x -= ix; 
		y -= iy; 

		double x00 = at(ix + 0, iy + 0), x10 = at(ix + 1, iy + 0); 
		double x01 = at(ix + 0, iy + 1), x11 = at(ix + 1, iy + 1); 

		return lerp1(lerp1(x00, x10, x), lerp1(x01, x11, x), y);
	}

	// Simple Forward Euler method for velocity integration in time 
	double euler(double &x, double &y, double timestep,const FluidQuantity &u, const FluidQuantity &v) const {
		double uVel = u.lerp(x, y) / _hx; 
		double vVel = v.lerp(x, y) / _hx; 


	} 
	
public: 
	FluidQuantity() {}  
	~FluidQuantity() {} 
	void flip() {} 

	double lerp(double x, double y) const { } 

	// Advect  
	void advect(double timestep, const FluidQuantity& u, const FluidQuantity& v) {

		// First step : euler integrate in time  

		// Second step : Interpolate from grid  
	}

	// Sets fluid quantity inside the given rect  
	void addInflow(double x0, double y0, double x1, double y1, double v) {

	}
};

// Fluid Solver  
class FluidSolver {
	// Fluid Quantities  
	FluidQuantity* _d; 
	FluidQuantity* _u; 
	FluidQuantity* _v; 


	void buildRhs() {} 

	void project() {} 

};

