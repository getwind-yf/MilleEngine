#include "BSDF.h"

namespace MRenderer 
{


	// Lambertian BRDF  

	float LambertianDiffuse::EvalInner(const Vector3& wo, const Vector3& wi, const DifferentialGeom& diffGeom, ScatterType types) const
	{
		if (BSDFUtilities::CosTheta(wo) <= 0.0f || !BSDFUtilities::SameHemisphere(wo, wi))
			return 0.0f; 
		return float(Math::EDX_INV_PI); 
	}

	float LambertianDiffuse::PdfInner()
	{
		
	}

	Color LambertianDiffuse : SampleScattered()
	{

	}

	// Mirror 

	Color Mirror::Eval()
	{

	}

	// Glass 



}