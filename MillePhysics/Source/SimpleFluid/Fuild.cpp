#pragma once

#include <algorithm> 
#include <math.h> 
#include <stdio.h> 

// Fluid Particles 
/*

Steps for simple solver using Guass-Seidel Solver : 
- Add sources (inflows, body forces)
- Force incompressiblity  ²»¿ÉÑ¹Ëõ 
	- Build pressure right-hand side  
	- Build pressure matrix 
	- Solve for pressure 
	- Apply pressure  
- Advection 
	- Semi-Lagrangian scheme  
*/

class FluidQuantity {
	// Memory
	double* _src; 
	double* _dst; 

	// Width and Height 
	int _w; 
	int _h; 

	// x, y offset from top left grid cell ; (0.5, 0.5) for centered quantities , (0.0, 0.5), (0.5,0.0) jittered quantities 
	double _ox; 
	double _oy; 
	// Grid cell size 
	double _hx; 

	// Linear interpolate between a and b 
	double lerp(double a, double b, double x) const {
		return a * (1.0 - x) + b * x; 
	} 
	


	// Simple Forward Euler method for velocity integration in time 
	double euler(double &x, double &y, double timestep,const FluidQuantity &u, const FluidQuantity &v) const {
		double uVel = u.lerp(x, y) / _hx; 
		double vVel = v.lerp(x, y) / _hx; 

		x -= uVel * timestep; 
		y -= vVel * timestep; 
	} 
	
public: 
	FluidQuantity(int w, int h, double ox, double oy, double hx)
		: _w(w), _h(h), _ox(ox), _oy(oy), _hx(hx) {
		_src = new double[_w * _h]; 
		_dst = new double[_w * _h]; 
	}  
	~FluidQuantity() {
		delete[] _src; 
		delete[] _dst;
	}
	void flip() {
		std::swap(_src, _dst);
	}

	const double* src() const {
		return _src; 
	}

	double at(int x, int y) const {
		return _src[x + y * _w];
	}

	double& at(int x, int y) {
		return _src[x + y * _w];
	}

	// Linear Interpolate on grid at (x,y) 
	double lerp(double x, double y) const {
		x = std::min(std::max(x - _ox, 0.0), _w - 1.001);
		y = std::min(std::max(y - _oy, 0.0), _h - 1.001);
		int ix = (int)x;
		int iy = (int)y;
		x -= ix;
		y -= iy;

		double x00 = at(ix + 0, iy + 0), x10 = at(ix + 1, iy + 0);
		double x01 = at(ix + 0, iy + 1), x11 = at(ix + 1, iy + 1);

		return lerp(lerp(x00, x10, x), lerp(x01, x11, x), y);
	}

	// Advect  
	void advect(double timestep, const FluidQuantity& u, const FluidQuantity& v) {

		for (int iy = 0, idx = 0; iy < _h; iy++) {
			for (int ix = 0; ix < _w; ix++, idx++) {
				double x = ix + _ox; 
				double y = iy + _oy; 

				euler(x, y, timestep, u, v); 

				_dst[idx] = lerp(x, y);
			}
		}
	}

	// Sets fluid quantity inside the given rect  
	void addInflow(double x0, double y0, double x1, double y1, double v) {

		int ix0 = (int)(x0 / _hx - _ox); 
		int iy0 = (int)(y0 / _hx - _oy); 
		int ix1 = (int)(x1 / _hx - _ox);
		int iy1 = (int)(y1 / _hx - _oy); 

		for (int y = std::max(iy0, 0); y < std::min(iy1, _h); y++)
			for (int x = std::max(ix0, 0); x < std::min(ix1, _h); x++)
				if (fabs(_src[x + y * _w]) < fabs(v))
					_src[x + y * _w] = v;
	}
};

// Fluid Solver  
class FluidSolver {
	// Fluid Quantities  
	FluidQuantity* _d; 
	FluidQuantity* _u; 
	FluidQuantity* _v; 

	// width and height 
	int _w; 
	int _h; 

	double _hx; 
	double _density; 

	double* _r; 
	double* _p; 

	// pressure 
	void buildRhs() {
		double scale = 1.0 / _hx; 
		for (int y = 0, idx = 0; y < _h; y++) {
			for (int x = 0; x < _w; x++, idx++) {
				_r[idx] = -scale * (_u->at(x + 1, y) - _u->at(x, y) +
					                _v->at(x, y + 1) - _v->at(x, y));
			}
		}
	} 

	// Perform the pressure solve using GS method  
	// limit iterations 
	void project(int limit, double timestep) {
		double scale = timestep / (_density * _hx * _hx); 
		double maxDelta;   // threshold 

		for (int iter = 0; iter < limit; iter++) {
			maxDelta = 0; 
			for (int y = 0, idx = 0; y < _h; y++) {
				for (int x = 0; x < _w; x++, idx++) {
					int idx = x + y * _w; 
					double diag = 0.0, offDiag = 0.0; 

					if (x > 0) {
						diag += scale; 
						offDiag -= scale * _p[idx - 1]; 
					}
					if (y > 0) {
						diag += scale; 
						offDiag -= scale * _p[idx - _w]; 
					}
					if (x < _w - 1) {
						diag += scale; 
						offDiag -= scale * _p[idx + 1]; 
					}
					if (y < _h - 1) {
						diag += scale; 
						offDiag -= scale * _p[idx + _w]; 
					}

					double newP = (_r[idx] - offDiag) / diag; 
					maxDelta = std::max(maxDelta, fabs(_p[idx] - newP)); 
					_p[idx] = newP; 
				}
			}

			if (maxDelta < 1e-5) {
				printf("Exiting solver after %d iterations, maximum change is %f\n ", iter, maxDelta);
				return; 
			}

		}
	
	} 

};

