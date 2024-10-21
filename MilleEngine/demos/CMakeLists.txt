# Author Dong Frank  

function(buidDemo DEMO_NAME)
	SET(DEMO_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/${DEMO_NAME})
	message(STATUS "generating project files: ${DEMO_FOLDER}")

    file(GLOB SOURCE *.cpp ${ENIGNE_HEADERS} ${DEMO_FOLDER}/*.cpp)
	SET(MAIN_CPP ${DEMO_FOLDER}/${DEMO_NAME}.cpp)
	if(EXISTS ${DEMO_FOLDER}/${DEMO_NAME}.h)
		SET(MAIN_HEADER ${DEMO_FOLDER}/${DEMO_NAME}.h)
	ENDIF()

	# Shaders
	set(SHADER_DIR_GLSL "./shaders/glsl/${DEMO_NAME}")
	file(GLOB SHADERS_GLSL "${SHADER_DIR_GLSL}/*.vert" "${SHADER_DIR_GLSL}/*.frag" "${SHADER_DIR_GLSL}/*.comp" "${SHADER_DIR_GLSL}/*.geom" "${SHADER_DIR_GLSL}/*.tesc" "${SHADER_DIR_GLSL}/*.tese" "${SHADER_DIR_GLSL}/*.mesh" "${SHADER_DIR_GLSL}/*.task" "${SHADER_DIR_GLSL}/*.rgen" "${SHADER_DIR_GLSL}/*.rchit" "${SHADER_DIR_GLSL}/*.rmiss" "${SHADER_DIR_GLSL}/*.rcall" "${SHADER_DIR_GLSL}/*.rahit" "${SHADER_DIR_GLSL}/*.rint" "${SHADER_DIR_GLSL}/*.glsl")
	set(SHADER_DIR_HLSL "./shaders/hlsl/${DEMO_NAME}")
	file(GLOB SHADERS_HLSL "${SHADER_DIR_HLSL}/*.vert" "${SHADER_DIR_HLSL}/*.frag" "${SHADER_DIR_HLSL}/*.comp" "${SHADER_DIR_HLSL}/*.geom" "${SHADER_DIR_HLSL}/*.tesc" "${SHADER_DIR_HLSL}/*.tese" "${SHADER_DIR_HLSL}/*.mesh" "${SHADER_DIR_HLSL}/*.task" "${SHADER_DIR_HLSL}/*.rgen" "${SHADER_DIR_HLSL}/*.rchit" "${SHADER_DIR_HLSL}/*.rmiss" "${SHADER_DIR_HLSL}/*.rcall" "${SHADER_DIR_HLSL}/*.rahit" "${SHADER_DIR_HLSL}/*.rint")
	source_group("Shaders\\GLSL" FILES ${SHADERS_GLSL})
	source_group("Shaders\\HLSL" FILES ${SHADERS_HLSL})
	
	#win32
	add_executable(${DEMO_NAME} WIN32 ${MAIN_CPP} ${SOURCE} ${MAIN_HEADER} ${SHADERS_GLSL} ${SHADERS_HLSL})
	target_link_libraries(${DEMO_NAME} VentEngine ${Vulkan_LIBRARY} ${WINLIBS})

	file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
	set_target_properties(${DEMO_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

endfunction(buidDemo)

function(buildDemos)
	foreach(Demo ${Demos})
		buidDemo(${Demo})
	endforeach(Demo)
endfunction(buildDemos)

set(Demos
	Triangle
	ShadowMapping
	CSM
)

buildDemos()