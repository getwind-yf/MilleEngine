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

	Color Mirror::Eval(const Vector3& wo, const Vector3& wi, const DifferentialGeom& diffGeom, ScatterType types) const
	{
		return Color::BLACK; 
	}

	float Mirror::EvalInner(const Vector3& wo, const Vector3& wi, const DifferentialGeom& diffGeom, ScatterType types)
	{
		return 0.0f; 
	}

	float Mirror::Pdf(const Vector3& wo, const Vector3& wi, const DifferentialGeom& diffGeom, ScatterType types)
	{
		return 0.0f; 
	}

	float Mirror::PdfInner(const Vector3& wo, const Vector3& wi, const DifferentialGeom& diffGeo)
	{
		
	}

	// Glass 
	Color Glass::Eval(const Vector3& vOut, const Vector3& vIn, const DifferentialGeom& diffGeom, ScatterType types) const
	{
		return Color::BLACK; 
	}

	float Glass::EvalInner(const Vector3& vOut, const Vector3& vIn, const DifferentialGeom& diffGeom, ScatterType types) const 
	{
		return 0.0f; 
	}

	float Glass::Pdf(const Vector3& vIn, const Vector3& vOut, const DifferentialGeom& diffGeom, ScatterType types) const
	{
		return 0.0f; 
	}

	float Glass::PdfInner(const Vector3& vIn, const Vector3& vOut, const DifferentialGeom& diffGeom, ScatterType types) const
	{
		return 0.0f; 
	}

	Color Glass::SampleScattered(const Vector3& vOut, const Sample& sample, const DifferentialGeom& diffGeom, Vector3* pVIn, flaot* pPdf, ScatterType types, ScatterType* pSampledTypes) const
	{
		bool sampleReflect = (types & (BSDF_REFLECION | BSDF_SPECULAR)) == (BSDF_REFLECION | BSDF_SPECULAR); 
		bool sampleRefract = (types & (BSDF_TRANSMISSION | BSDF_SPECULAR)) == (BSDF_TRANSMISSION | BSDF_SPECULAR); 

		if (!sampleReflect && !sampleRefract)
		{
			*pPdf = 0.0f; 
			return Color::BLACK;
		}

		bool sampleBoth = sampleReflect == sampleRefract; 

		Vector3 wo = diffGeom.WorldToLocal(vOut), vWi; 
		float fresnel = FresnelDielectric(BSDFCoordinate::CosTheta(vWo), mEtai, mEtat); 
		float prob = 0.5f * fresnel + 0.25f; 

		// Sample Reflection 
		if (sample.w <= prob && sampleBoth || (sampleReflect && !sampleBoth))
		{
			vWi = Vector3(-vWo.x, -vWo.y, vWo.z); 
			*pvIn = diffGeom.LocalToWorld(vWi); 
			*pPdf = !sampleBoth ? 1.0 : prob; 

			if (pSampleTypes != nullptr)
			{
				*pSampledTypes = ScatterType(BSDF_REFLECION | BSDF_SPECULAR); 
			}

			return fresnel * Color::WHITE / BSDFCoordinate::AbsCosTheta(vWi); 
		}

		// Sample refraction 
		else if (sample.w > prob && sampleBoth || (sampleRefract && !sampleBoth))
		{
			bool entering = BSDFCoordinate::CosTheta(vWo) > 0.0f; 
			float etai = mEtai, etat = mEtat; 

			if (!entering)
				Swap(etai, etat); 

			float sini2 = BSDFCoordinate::SinTheta2(vWo); 
			float eta = etai / etat; 
			float sint2 = eta * eta * sini2; 

			if (sint2 > 1.0f)
				return Color::BLACK; 

			float cost = Math::Sqrt(Math::Max(0.0f, 1.0f - sint2)); 
			if (entering)
				cost = -cost; 
			float sintOverSini = eta; 

			vWi = Vector3(sintOverSini * -vWo.x, sintOverSini * -vWo.y, cost); 
			*pvIn = diffGeom.LocalToWorld(vWi); 
			*pPdf = !sampleBoth ? 1.0f : 1.0f - prob; 

			if(pSampledTypes != nullptr)
			{
				*pSampledTypes = ScatterType(BSDF_TRANSMISSION | BSDF_SPECULAR); 
			}

			return (1.0f - fresnel) * eta * eta * GetValue(mpTexture.Get(), diffGeom) / BSDFCoordinate::AbsCosTheta(vWi); 
		}

		return Color::BLACK

	}

}