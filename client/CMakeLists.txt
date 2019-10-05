add_executable(client MidiClient.cpp)

target_link_libraries(client -L/usr/local/lib/)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(client -lcrypt -lrtmidi)
target_link_libraries(client -lzmq czmq)
