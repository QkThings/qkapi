#!/usr/bin/python

import sys
import json
import socket
import threading
import select
import time

class QkAPI(object):

	_NACK = 0
	_ACK = 1
	_TIMEOUT = 254
	_ERROR = 255
	_subscription_names = ["data", "event", "debug"]

	def __init__(self):
		self._client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self._alive = False
		self._ack = QkAPI._NACK
		self._registered_callbacks = {}
		self._parse_level = 0
		self._rx_str_buf = ""
		self.json_str = ""
		self.json = {}

	def _register_callback(self, id, callback):
		self._registered_callbacks[id] = callback

	def subscribe(self, subname, callback):
		self.set("/qk/subscriptions/" + subname, 1)
		self._register_callback(subname, callback)

	def unsubscribe(self, subname, callback):
		self.set("/qk/subscriptions/" + subname, 0)
		self._register_callback(subname, None)
	

	def connect(self, hostname, port):
		self._client.connect((hostname, port))
		self._thread_listener = threading.Thread(target = self._listener)
		self._thread_listener.setDaemon(True)
		self._alive = True
		self._thread_listener.start()
		
	def disconnect(self):
		self._alive = False;
		self._client.close()

	def _parse(self, data):
		parsing_done = False
		bytes = list(data)
		for c in bytes:
			if c is '{':
				if self._parse_level == 0:
					self._rx_str_buf = ""
				self._parse_level = self._parse_level + 1
			elif c is '}':
				self._parse_level = self._parse_level - 1
				if self._parse_level == 0:
					parsing_done = True
			self._rx_str_buf += str(c)
			if parsing_done is True:
				self._process()
				parsing_done = False

	def _process(self):
		self.json_str = self._rx_str_buf
		self.json = json.loads(self.json_str)
		self._ack = QkAPI._ACK
		#print self.json
		if "data" in self.json.keys() and self._registered_callbacks["data"] is not None:
			self._registered_callbacks["data"](self)
		if "event" in self.json.keys() and self._registered_callbacks["event"] is not None:
			self._registered_callbacks["event"](self)
		if "debug" in self.json.keys() and self._registered_callbacks["debug"] is not None:
			self._registered_callbacks["debug"](self)
					
	def _listener(self):
		while self._alive:
			input,output,err = select.select([self._client],[],[]) 
			for rd in input: 
				if self._alive is True:
					data = rd.recv(1024)
#					print "data received:", data
					self._parse(data)
					
	def wait(self):
		while self._ack is QkAPI._NACK:
			continue

	def wait_and_print(self):
		self.wait()
		print self.json_str

	def get(self, resource, params=None,  wait=True, show_result=False):
		self._client.send(self._call("get",[resource, params]))
		if wait or show_result:
			self.wait()
		if show_result:
			print self.json_str
	
	def set(self, resource, params=None, wait=True, show_result=False):
		self._client.send(self._call("set",[resource, params]))
#		if "cmds" in resource:
#			wait = False
		if wait or show_result:
			self.wait()
		if show_result:
			print self.json_str
		
	def _call(self, method, params=None):
		self._ack = QkAPI._NACK
		return json.dumps({'method':method,'params':params})

# ------------------------------------------------------------------------------------------------

def handleDataJson(api):
	connID = int(api.json["data"]["connID"])
	addr = int(api.json["data"]["address"])
	print "-----------------------------------------------"
	print " %-8s connID=%d address=%04x %s" % ("DATA", connID, addr, time.strftime("%H:%M:%S"))
	values = api.json["data"]["values"]
	for obj in values:
		print "%-9s %s" % (obj["label"],obj["value"])	

def handleEventJson(api):
	connID = int(api.json["event"]["connID"])
	addr = int(api.json["event"]["address"])
	print "-----------------------------------------------"
	print " %-8s connID=%d address=%04x %s" % ("EVENT", connID, addr, time.strftime("%H:%M:%S"))
	print "label:   ",api.json["event"]["label"]
	print "message: ",api.json["event"]["message"]
	print "args:    ",api.json["event"]["args"]	

def handleDebugJson(api):
	connID = int(api.json["debug"]["connID"])
	addr = int(api.json["debug"]["address"])
	print "-----------------------------------------------"
	print " %-8s connID=%d address=%04x %s" % ("DEBUG", connID, addr, time.strftime("%H:%M:%S"))
	print "message: ",api.json["debug"]["message"]


if __name__ == "__main__":
	print "QkAPI test"
	HOST = "192.168.1.64"
	PORT = 1234
	api = QkAPI()
	api.connect(HOST, PORT)
	print "Connected"
	api.get("/qk/conns", show_result=True)
	api.get("/qk/nodes", show_result=True)
	api.get("/qk/nodes/0", show_result=True)
	api.get("/qk/nodes/0/device/sampling", show_result=True)
	api.set("/qk/nodes/0/device/sampling/mode", "continuous", show_result=True)
	api.set("/qk/nodes/0/device/sampling/frequency", 10, show_result=True)
	api.set("/qk/nodes/0/device/cmds/update", show_result=True)
	api.get("/qk/nodes/0/device/sampling", show_result=True)
	api.set("/qk/nodes/123/device/sampling/frequency", 1, show_result=True)
	api.set("/qk/nodes/0/device/sampling/frequency", "asd", show_result=True)

	api.subscribe("data", handleDataJson)
	api.subscribe("event", handleEventJson)
	api.subscribe("debug", handleDebugJson)

	api.set("/qk/cmds/start")
	print "Press any key to exit..."
	raw_input()
	api.set("/qk/cmds/stop")
	api.disconnect()
	print "Done"

    
