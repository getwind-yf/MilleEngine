#pragma once

// 定义类型种类 

// Define 
#pragma warning (disable: 4244)  
#pragma warning (disable: 4267) 
#pragma warning (disable: 4018) 
#pragma warning (disable: 4800) 
#pragma warning (disable: 4838) 

// C++ support  
#include <cstdlib> 
#include <crtdbg.h> 
#include <new> 
#include <malloc.h> 
#include <cstdio> 
#include <cmath> 
#include <cfloat> 

#define USE_UNICODE 0 


namespace MilleRenderer 
{
	typedef unsigned int uint; 
	typedef unsigned char _byte; 
	typedef unsigned long dword; 

	typedef unsigned char uint8; 
	typedef unsigned short int uint16; 


#if USE_UNICODE 


#endif

	


	typedef size_t SIZE_T; 


} 