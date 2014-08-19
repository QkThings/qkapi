from qkthings import QkAPI
import time

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
	print "qkapi test"
	HOST = "localhost"
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
