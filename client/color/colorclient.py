#!/usr/bin/python

import sys, time
import signal
import threading
import zmq
import wiringpi
import json
from flask import Flask, render_template, session, request
from flask_socketio import SocketIO, emit, join_room, leave_room, \
close_room, rooms, disconnect

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

context = zmq.Context()
zmq_socket = context.socket(zmq.PUB)
zmq_socket.bind("tcp://*:7050")



class Color(dict):
    def __init__(self, red, green, blue):
        dict.__init__(self, red=red, green=green, blue=blue)

class Stripe():
    color_period = 1
    i_color = 0
    
    is_dynamic = False
    is_fade = False
    is_enabled = True
    
    colors = [Color(0, 0, 0)]

    def enable(self, enable):
        self.is_enabled = enable
        if enable:
            zmq_socket.send("Colors %s" % (json.dumps(self.colors)))
        else:
            zmq_socket.send("Colors %s" % (json.dumps([Color(0, 0, 0)])))

    def setColorRGB(self, red, green, blue):
        self.colors = [Color(red, green, blue)]
        if self.is_enabled:
            zmq_socket.send("Colors %s" % (json.dumps(self.colors)))

    def setColors(self, newColors):
        colors = []
        for color in newColors:
            lv = len(color) - 1
            if lv is 6:
                red = int(color[1:3],16)
                green = int(color[3:5],16)
                blue = int(color[5:7],16)
            else:
                red = int(color[1:2],16) * 16
                green = int(color[2:3],16) * 16
                blue = int(color[3:4],16) * 16
                
            colors.append(Color(red, green, blue))
        self.colors = colors
        if self.is_enabled:
            zmq_socket.send("Colors %s" % (json.dumps(colors)))

stripe = Stripe()

def handler(signal, frame):
  print('CTRL-C pressed!')
  sys.exit(0)

   
@app.route("/")
def main():
   templateData = {
      'dummy' : 0
      }
   # Pass the template data into the template main.html and return it to the user
   return render_template('index.html', **templateData)
   
   
@socketio.on('my_event')
def test_message(message):
    print("Connected")
    session['receive_count'] = session.get('receive_count', 0) + 1
    emit('my_response',
         {'data': message['data'], 'count': session['receive_count']})


@socketio.on('color_changed')
def set_colors(colors):
    print(colors)
    stripe.setColors(colors)

@socketio.on('toggle_fade')
def toggle_fade(fade):
    stripe.is_fade = fade
    
@socketio.on('toggle_led')
def toggle_led(enable):
    stripe.enable(enable)

if __name__ == "__main__":
    print("Running")
    socketio.run(app, host='0.0.0.0', port=80)


