#pragma once

class Camera
{
public:
	Vector3 mPos; 
	Vector3 mTarget; 
	Vector3 mUp; 
	Vector3 mDir; 

	float mMoveScaler; 
	float mRotateScaler; 
	float mYaw, mPitch; 

	float mFOV; 
	float mRatio; 
	float mNearClip; 
	float mFarClip; 

protected: 
	float mFOV_2; 

	int mFilmResX; 
	int mFilmResY; 

	// Matrices 
	Matrix mView;
	Matrix mViewInv; 
	Matrix mProj; 

	Matrix mScreenToRaster; 
	Matrix mRasterToCamera; 

public:
	Camera(); 
	Camera(const Vector3& ptPos, const Vector3& ptTar, const Vector3& vUp, int iResX, int iResY,
		float fFOV = 35.0f, float fNear = 0.1f, float fFar = 1000.0f);


};