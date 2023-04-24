cmake_minimum_required(VERSION 3.13)
project(MailServer)

if(WIN32)
else()
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

endif()

include_directories(
	${PROJECT_NAME}
	PUBLIC
	C:/sdk/asio-1.26.0/include
)


add_executable(
	main
	main.cpp
)

add_executable(
	client
	client.cpp
)

if (WIN32)
	target_link_libraries(client -static)
	target_link_libraries(main -static)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set(CMAKE_MFC_FLAG 1)
else()
target_link_libraries(client -static)
	target_link_libraries(main -static)
endif()