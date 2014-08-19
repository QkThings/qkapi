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
		if wait or show_result:
			self.wait()
		if show_result:
			print self.json_str
		
	def _call(self, method, params=None):
		self._ack = QkAPI._NACK
		return json.dumps({'method':method,'params':params})

    
