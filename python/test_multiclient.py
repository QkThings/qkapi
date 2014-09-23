#!/usr/bin/python

from qkthings import QkAPI
import time
import argparse
import os

dat_cnt = []
evt_cnt = []
dbg_cnt = []
HOST = "localhost"
PORT = 1234

class Client:
	def __init__(self, id):
		self.id = id
		self.dat_cnt = 0
		self.evt_cnt = 0
		self.dbg_cnt = 0
		self.api = QkAPI()
	
	def run(self):
		self.api.connect(HOST, PORT)
		self.api.subscribe("data", self.handle_data)
		self.api.subscribe("event", self.handle_event)
		self.api.subscribe("debug", self.handle_debug)

	def quit(self):
		self.api.disconnect()

	def handle_data(self, api):
		self.dat_cnt += 1

	def handle_event(self, api):
		self.evt_cnt += 1

	def handle_debug(self, api):
		self.dbg_cnt += 1

'''
def handleDataJson(api):
	connID = int(api.json["data"]["conn_id"])
	addr = int(api.json["data"]["address"])
	print "-----------------------------------------------"
	print " %-8s connID=%d address=%04x %s" % ("DATA", connID, addr, time.strftime("%H:%M:%S"))
	values = api.json["data"]["values"]
	for obj in values:
		print "%-9s %s" % (obj["label"],obj["value"])	

def handleEventJson(api):
	connID = int(api.json["event"]["conn_id"])
	addr = int(api.json["event"]["address"])
	print "-----------------------------------------------"
	print " %-8s connID=%d address=%04x %s" % ("EVENT", connID, addr, time.strftime("%H:%M:%S"))
	print "label:   ",api.json["event"]["label"]
	print "message: ",api.json["event"]["message"]
	print "args:    ",api.json["event"]["args"]	

def handleDebugJson(api):
	connID = int(api.json["debug"]["conn_id"])
	addr = int(api.json["debug"]["address"])
	print "-----------------------------------------------"
	print " %-8s connID=%d address=%04x %s" % ("DEBUG", connID, addr, time.strftime("%H:%M:%S"))
	print "message: ",api.json["debug"]["message"]
'''
clients = []
num_clients = 0
quit = False



def handle_rx(api):
	if quit:
		return
	os.system('clear')
	header = "CLIENT "
	data =   "Data   "
	events = "Events "
	debug =  "Debug  "
	for i in range(0, num_clients):
		header += "\t%d" % i
		data += "\t%d" % clients[i].dat_cnt
		events += "\t%d" % clients[i].evt_cnt
		debug += "\t%d" % clients[i].dbg_cnt
	print header
	print "-------"
	print data
	print events
	print debug
	printing = False


if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('--n')
	args = parser.parse_args()

	print "Create %s clients" % args.n

	num_clients = int(args.n)


	for i in range(0, num_clients):
		client = Client(i)
		client.run()
		if i == 0:
			client.api.register_callback("rx", handle_rx)
		clients.append( client )


	api = QkAPI()
	api.connect(HOST, PORT)
	api.set("/qk/conns/0/nodes/0/device/sampling/frequency", 2, show_result=True)
	api.set("/qk/conns/0/cmds/start")

	api.register_callback("rx", handle_rx)
		

	print "Type ENTER to exit."
	raw_input()
	quit = True







	api.set("/qk/conns/0/cmds/stop")
	api.disconnect()

	for i in range(0, num_clients):
		clients[i].quit()

	print "Done"

    
