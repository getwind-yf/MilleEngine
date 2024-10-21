#Author: dong frank

file(GLOB ENGINE_SOURCE "*.cpp" "*.hpp" "*.h" "../external/imgui/*.cpp")
file(GLOB ENIGNE_HEADERS "*.hpp" "*.h")

#ktx
set(KTX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../external/ktx) 
set(KTX_SOURCES     
    ${KTX_DIR}/lib/texture.c
    ${KTX_DIR}/lib/hashlist.c
    ${KTX_DIR}/lib/checkheader.c
    ${KTX_DIR}/lib/swap.c
    ${KTX_DIR}/lib/memstream.c
    ${KTX_DIR}/lib/filestream.c)

add_library(VentEngine STATIC ${ENGINE_SOURCE} ${KTX_SOURCES}) 

target_link_libraries(VentEngine ${Vulkan_LIBRARY})
