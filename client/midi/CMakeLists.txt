add_executable(midiclient MidiClient.cpp)

target_link_libraries(midiclient -L/usr/local/lib/)
target_link_libraries(midiclient ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(midiclient -lcrypt -lrtmidi)
target_link_libraries(midiclient -lzmq czmq)
