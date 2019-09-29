
set(SOURCE_FILES 
    main.cpp
    Color.cpp
    Stripe.cpp
		device/LaunchpadIn.cpp
		device/LaunchpadOut.cpp
    device/ServerIn.cpp
		mapping/Mapping.cpp
		mapping/DirectMapping.cpp
		mapping/SequencerMapping.cpp
		mapping/page/Page.cpp
		mapping/page/ColorPage.cpp
		mapping/page/SequencerPage.cpp
		mapping/page/SnakePage.cpp)

add_executable(lights ${SOURCE_FILES})

target_link_libraries(lights -L/usr/local/lib/)
TARGET_LINK_LIBRARIES(lights ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(lights ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(lights ${wiringPi_LIB})
TARGET_LINK_LIBRARIES(lights -lcrypt -lrtmidi)
target_link_libraries(lights Ableton::Link)
target_link_libraries(lights czmq)
