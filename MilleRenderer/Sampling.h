#pragma once

namespace MilleEngine
{
	class Distribution1D
	{
	private:
		Array1f mPDF; 
	};

	class Distribution2D
	{
	private: 
		Array<UniquePtr<Distribution1D>>  mConditional; 
		UniquePtr<Distribution1D>  mpMarginal; 
	};

}