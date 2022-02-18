#pragma once

#include<vector>
#include "../Core/Camera.h"




typedef unsigned int uint; 
typedef std::vector Array;


enum class EIntegratorType
{
	DirectLighting, 
	PathTracing, 
	BidirectionalPathTracing, 
	MultipixeledMLT, 
	StochasticPPM 
};

enum class ESamplerType
{
	Random,
	Sobol, 
	Metropolis 
};

enum class EFilterType
{
	Box, 
	Gaussian, 
	MitchellNetravali 
};


// �������ݵĶ��� 

struct RenderDesc
{
	CameraParams CameraParams; 
	EIntegratorType IntegratorType; 
	ESamplerType SamplerType; 
	EFilterType FilterType; 
	bool AdaptiveSample; 
	bool UseRHF; 
	uint ImageWidth, ImageHeight; 
	uint SamplersPerPixel; 
	uint MaxPathLength; 
	Array<String> ModelPaths; 
};


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
		// ֱ�ӹ��ջ��� 
		static Color DirectLighting(const Scatter& scatter, const Vector3& outVec, const Light* pLight, const Scene* pScene,
			Sampler* pSampler, ScatterType scatterType = ScatterType(BSDF_ALL & ~BSDF_SPECULAR));
		// ���淴�䲿�� 
		static Color SpecularReflect(const TiledIntegrator* pIntegrator, const Scene* pScene, Sampler* pSampler, const RayDifferential& ray,
			const DifferentialGeom& diffGeom, RandomGenerator& random, MemoryPool& memory);
		// ���洩͸ 
		static Color SpecularTransmit(const TiledIntegrator* pIntegrator, const Scene* pScene, Sampler* pSampler, const RayDifferetial& ray,
			const DifferentialGeom& diffGeom, RandomGenerator& random, MemoryPool& memory);

	protected:

		// RenderJob������
		const RenderDesc& mDesc;

		// Sync managing objects  
		const TaskSynchronizer& mTaskSync;

	};

	// Tiled Integrator
	class TiledIntegrator : public Integrator
	{
	};

}