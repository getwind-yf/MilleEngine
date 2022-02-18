#pragma once

struct CameraParams
{
	Vector3 Pos; 
	Vector3 Target; 
	Vector3 Up; 

	float NearClip, FarClip; 
	float FocusPlanetDist; 

	static const int FullFrameSensorSize = 24; 
	int FocalLengthMilliMeters; 
	float FStop; 
	float Vignette; 

	float CalcFieldOfView() const; 
	float CalCCircleOfConfustionRadius() const; 


};