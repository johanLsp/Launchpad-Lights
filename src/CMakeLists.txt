
set(SOURCE_FILES 
		main.cpp
		LaunchpadOut.cpp
		LaunchpadIn.cpp
		Stripe.cpp
		Mapping.cpp
		DirectMapping.cpp
		SequencerMapping.cpp
		Page.cpp
		ColorPage.cpp
		Color.cpp
		SequencerPage.cpp
		SnakePage.cpp)

add_executable(lights ${SOURCE_FILES})

target_link_libraries(lights -L/usr/local/lib/)
TARGET_LINK_LIBRARIES(lights ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(lights ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(lights ${wiringPi_LIB})
TARGET_LINK_LIBRARIES(lights -lcrypt -lrtmidi)
target_link_libraries(lights Ableton::Link)
