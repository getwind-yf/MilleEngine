#pragma once

namespace MilleRenderer
{
	class Integrator
	{
	public:
		Integrator(const RenderDesc& desc, const TaskSynchronizer& taskSync)
			: mDesc(desc), mTaskSync(taskSync) {}

		virtual void Render(const Scene* pScene, const Camera* pCamera, Sampler* pSampler, Film* pFilm) const = 0;

		virtual ~Integrator() {}

	public:
		// 直接光照积分 
		static Color DirectLighting(const Scatter& scatter, const Vector3& outVec, const Light* pLight, const Scene* pScene,
			Sampler* pSampler, ScatterType scatterType = ScatterType(BSDF_ALL & ~BSDF_SPECULAR));
		// 镜面反射部分 
		static Color SpecularReflect(const TiledIntegrator* pIntegrator, const Scene* pScene, Sampler* pSampler, const RayDifferential& ray,
			const DifferentialGeom& diffGeom, RandomGenerator& random, MemoryPool& memory);
		// 镜面穿透 
		static Color SpecularTransmit(const TiledIntegrator* pIntegrator, const Scene* pScene, Sampler* pSampler, const RayDifferetial& ray,
			const DifferentialGeom& diffGeom, RandomGenerator& random, MemoryPool& memory);

	protected:

		// RenderJob的描述
		const RenderDesc& mDesc;

		// Sync managing objects  
		const TaskSynchronizer& mTaskSync;

	};

	// Tiled Integrator
	class TiledIntegrator : public Integrator
	{
	};

}