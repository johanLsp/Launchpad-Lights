
CC= g++
CFLAGS= -g

APP= lights
SRC= main.cpp Stripe.cpp LaunchpadIn.cpp LaunchpadOut.cpp Mapping.cpp SequencerMapping.cpp DirectMapping.cpp

LD_FLAGS= -lrtmidi -lwiringPi -lboost_system -lboost_chrono -lpthread
LD_PATH=-Wl,-rpath -Wl,/usr/local/lib

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LD_PATH) $(LD_FLAGS) -o $(APP)
