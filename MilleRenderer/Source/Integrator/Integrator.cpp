#include "Integrator.h"

namespace MILLE
{
	void TiledIntegrator::Render(const Scene* pScene, const Camera* pCamera, Sampler* pSampler, Film* pFilm) const
	{
		for (int spp = 0; spp < mJobDesc.SamplesPerPixel; spp++)
		{
			int numTiles = mTaskSync.GetNumTiles();
			parallel_for(0, numTiles, [&](int i))
			{
				const RenderTile& tile = mTaskSync.GetTile(i); 
				UniquePtr<Sampler> pTileSampler(pSampler->Clone); 
			}
		}
	}

	Color Integrator::EstimateDirectLighting(const Scatter& scatter,
		const Vector3& outDir,
		const Light* pLight,
		const Scene* pScene,
		Sampler* pSampler,
		ScatterType scatterType)
	{
		const Vector3& position = scatter.mPosition; 
		const Vector3& normal = scatter.mNormal; 

		Color L; 

		bool requireMIS = false; 
		if (scatter.IsSurfaceScatter())
		{
			const DifferentialGeom& diffGeom = static_cast<const DifferentialGeom&>(scatter); 
			const BSDF* pBSDF = diffGeom.mpBSDF; 

			requireMIS = (pBSDF->GetScatterType() & BSDF_GLOSSY); 


		}

		// Sample light sources 
		{
			Vector3 lightDir; 
			VisibilityTester visibility; 
			float lightPdf, shadingPdf; 
			Color transmittance; 

			// �������������ɫ 
			const Color Li = pLight->Illuminate(scatter, pSampler->GetSample(), &lightDir, &visibility, &lightPdf); 

			if (lightPdf > 0.0f && !Li.IsBlack())
			{
				Color f;
				// ɢ����� 
				if (scatter.IsSurfaceScatter())
				{
					// In DirectLighting interacts with materials 
					const DifferentialGeom& diffGeom = static_cast<const DifferentialGeom&> (scatter);
					const BSDF* pBSDF = diffGeom.mpBSDF;

					f = pBSDF->Eval(outDir, lightDir, diffGeom, scatterType);
					f *= Math::AbsDot(lightDir, normal);
					shadingPdf = pBSDF->Pdf(outDir, lightDir, diffGeom);
				}
				// ���ʱ���
				else
				{
					const MediumScatter& mediumScatter = static_cast<const MediumScatter&>(scatter);
					const PhaseFunctionHG* pPhaseFunc = mediumScatter.mpPhaseFunc;

					const float phase = pPhaseFunc->Eval(outDir, lightDir);
					f = Color(phase);
					shadingPdf = phase;

				}

				// �ж��ڵ��� 
				if (!f.IsBlack() && visibility.Unoccluded(pScene))
				{
					Color transmittance = visibility.Transmittance(pScene, pSampler);
					if (pLight->IsDelta() || !requireMIS)
					{
						L += f * Li * transmittance / lightPdf; 
						return L; 
					}

					const float misWeight = Sampling::PowerHeuristic(1, lightPdf, 1, shadingPdf); 
					L += f * Li * transmittance * misWeight / lightPdf; 
				}

			}

		}

		// Sample BSDF or medium 

		if (!pLight->IsDelta() && requireMIS)
		{
			Vector3 lightDir; 
			Color f; 
			float shadingPdf; 

			// Surface Scattering 
			if (scatter.IsSurfaceScatter())
			{
				const DifferentialGeom& diffGeom = static_cast<const DifferentialGeom&>(scatter);
				const BSDF* pBSDF = diffGeom.mpBSDF; 

				ScatterType types; 
				f = pBSDF->SampleScattered(outDir, pSampler->GetSample(), diffGeom, &lightDir, &shadingPdf�� scatterType, &types); 
				f *= Math::AbsDot(lightDir, normal); 

			}
			else
			{
				const MediumScatter& mediumScatter = static_cast<const MediumScatter&>(scatter); 
				const PhaseFunctionHG* pPhaseFunc = mediumScatter.mpPhaseFunc; 

				const float phase = pPhaseFunc->Sample(outDir, &lightDir, pSampler->Get2D()); 
				f = Color(phase); 
				shadingPdf = phase; 

			}

			if (shadingPdf > 0.0f && !f.IsBlack())
			{
				float lightPdf = pLight->Pdf(position, lightDir); 
				if (lightPdf > 0.0f)
				{
					float misWeight = Sampling::PowerHeuristic(1, shadingPdf, 1, lightPDf); 
					Vector3 center; 
					float radius; 
					pScene->WorldBounds().BoundingSphere(&center, &radius); 

					DifferentialGeom diffGeom; 
					Ray rayLight = Ray(position, lightDir, scatter.mMediumInterface.GetMedium(lightDir, normal), 2.0f * radius); 
					Color Li; 

					if (pScene->Intersect(rayLight, &diffGeom))
					{
						pScene->PostIntersect(rayLight, &diffGeom); 
						if (pLight == (Light*)diffGeom.mpAreaLight)
							Li = diffGeom.Emit(-lightDir); 
					}
					else if ((Light*)pScene->GetEnvironmentLight() == pLight)
					{
						Li = pLight->Emit(-lightDir); 
					}

					if (!Li.IsBlack())
					{
						Color transmittance = Color::WHITE; 
						if (rayLight.mpMedium)
							transmittance = rayLight.mpMedium->Transmittance(rayLight, pSampler);

						L += f * Li * transmittance * misWeight / shadingPdf; 
					}
 				}
			}
		}

		return L;
	}

	Color Integrator::SpecularReflect(const TileIntegrator* pIntegrator,
		const Scene* pScene,
		Sampler* pSampler,
		const RayDifferential& ray,
		const DifferentialGeom& diffGeom,
		RandomGen& random,
		MemoryPool& memory)
	{
		const Vector3& position = diffGeom.mPosition; 
		const Vector3& normal = diffGeom.mNormal; 

	}

}