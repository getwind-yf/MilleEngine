#pragma once

namespace MRenderer
{
	enum ScatterType
	{
		BSDF_REFLECION = 1 << 0, 
		BSDF_TRANSMISSION = 1 << 1, 
		BSDF_DIFFUSE = 1 << 2, 
		BSDF_GLOSSY = 1 << 3, 
		BSDF_SPECULAR = 1 << 4, 
		BSDF_ALL_TYPES = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR, 
		BSDF_ALL_REFLECTION = BSDF_REFLECION | BSDF_ALL_TYPES, 
		BSDF_ALL_TRANSMISSION = BSDF_TRANSMISSION | BSDF_ALL_TYPES, 
		BSDF_ALL = BSDF_ALL_REFLECTION | BSDF_ALL_TRANSMISSION 
	};

	enum class BSDFType
	{
		Diffuse, 
		Mirror, 
		Glass, 
		RoughConductor, 
		RoughDielectric, 
		Disney 
	};

	struct Parameter
	{
		enum
		{
			Float, 
			Color, 
			TextureMap, 
			NormalMap, 
			None 
		}Type; 

		union
		{
			struct { float R; float G; float B;  };
			struct { float Value; float Min; float Max;  }; 
			char TexPath[260];    // Max Path 
		};
	};


	class IEditable
	{
	public: 
		virtual int GetParameterCount() const = 0; 
		virtual String GetParameterName(const int idx) const = 0; 
		virtual Parameter GetParameter(const String& name) const = 0; 
		virtual void SetParameter(const String& name, const Parameter& param) = 0; 
	}; 


	class BSDF : public IEditable
	{
	protected:
		const ScatterType m_ScatterType; 
		const BSDFType m_BSDFType; 
		UniquePtr<Texture2D<Color>> m_pTexture; 
		UniquePtr<Texture2D<Color>> m_pNormalMap; 

	public:
		BSDF(ScatterType t1, BSDFType t2, const Color& color); 
		BSDF(ScatterType t1, BSDFType t2, UniquePtr<Texture2D<Color>> pTex£¬UniquePtr<Texture2D<Color>> pNormal); 
		BSDF(ScatterType t1, BSDFType t2, const char* pFile);

		virtual ~BSDF() {} 

		bool MatchesTypes(ScatterType flags) const { return (m_ScatterType & flags) == m_ScatterType;  }
		bool IsSpecular() const { return (ScatterType(BSDF_SPECULAR | BSDF_DIFFUSE | BSDF_GLOSSY) & m_ScatterType) == ScatterType(BSDF_SPECULAR);  }
		
		virtual Color Eval(const Vector3& vOut, const Vector3& vIn, const DifferentialGeom& diffGeom, ScatterType types = BSDF_ALL) const; 
		virtual flaot Pdf(const Vector3& vOut, const Vector3& vIn, const DifferentialGeom& diffGeom, ScatterType types = BSDF_ALL) const; 
		virtual Color SampleScattered(const Vector3& vOut, const Sample& sample, const DifferentialGeom& diffGeom, Vector3* pvIn, float* pPdf,
			ScatterType types = BSDF_ALL, ScatterType* pSampledTypes = nullptr) const = 0;

		template<typename T> 
		__forceinline const T GetValue(const Texture2D<T>* pTex,
			const DifferentialGeom& diffGeom,
			const TextureFilter filter = TextureFilter::TriLinear) const
		{
			Vector2 differential[2] = {
				(diffGeom.mDudx, diffGeom.mDvdx),
				(diffGeom.mDudy, diffGeom.mDvdy)
			}; 
			return pTex->Sample(diffGeom.mTexcoord, differential, filter);
		}

		const ScatterType GetScatterType() const { return m_ScatterType;  }
		const BSDFType GetBSDFType() const { return m_BSDFType;  } 

		const Texture2D<Color>* GetTexture() const { return m_pTexture.Get();  }
		const Texture2D<Color>* GetNormalMap() const { return m_pNormalMap.Get();  }

		UniquePtr<Texture2D<Color>> MoveTexture() { return Move(m_pTexture);  }
		UniquePtr<Texture2D<Color>> MoveNormalMap() { return Move(m_pNormalMap); }


	protected:
		virtual float EvalInner(const Vector3& vOut, const Vector3& vIn, const DifferentialGeom& diffGeom, ScatterType types = BSDF_ALL) const = 0; 
		virtual float PdfInner(const Vector3& vOut, const Vector3& vIn, const DifferentialGeom& diffGeom, ScatterType types = BSDF_ALL) const = 0; 

	public:
		virtual int GetParameterCount() const
		{
			return 2; 
		}

	};


	class LambertianDiffuse : public BSDF
	{
	public:
		LambertianDiffuse();
	};

	class Mirror : public BSDF
	{
	public: 
		Mirror();
	};

	class Glass : public BSDF
	{
		Glass(); 
	};

	namespace BSDFUtilities
	{
		inline float CosTheta() {} 
	}



}