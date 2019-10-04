
set(SOURCE_FILES 
    main.cpp
    Stripe.cpp
    device/Device.cpp
    device/Launchpad.cpp
    mapping/DirectMapping.cpp
    mapping/SequencerMapping.cpp
    mapping/page/ColorPage.cpp
    mapping/page/SequencerPage.cpp
    mapping/page/SnakePage.cpp
    transport/ColorServer.cpp
    transport/MidiLocal.cpp
    transport/Transport.cpp
    util/Color.cpp)

add_executable(lights ${SOURCE_FILES})

target_link_libraries(lights -L/usr/local/lib/)
target_link_libraries(lights ${Boost_LIBRARIES})
target_link_libraries(lights ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(lights ${wiringPi_LIB})
target_link_libraries(lights -lcrypt -lrtmidi)
target_link_libraries(lights Ableton::Link)
target_link_libraries(lights czmq)
