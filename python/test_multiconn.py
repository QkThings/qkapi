from qkthings import QkAPI
import time

def handle_data(api):
	connID = int(api.json["data"]["conn_id"])
	addr = int(api.json["data"]["address"])
	print "-----------------------------------------------"
	print " %-8s conn_id=%d address=%04x %s" % ("DATA", connID, addr, time.strftime("%H:%M:%S"))
	values = api.json["data"]["values"]
	for obj in values:
		print "%-9s %s" % (obj["label"],obj["value"])	


if __name__ == "__main__":
	HOST = "localhost"
	PORT = 1234
	api = QkAPI()
	api.connect(HOST, PORT)

	api.get("/qk/conns/0/nodes", show_result=True)
	api.get("/qk/conns/1/nodes", show_result=True)

	api.subscribe("data", handle_data, 0)
	api.subscribe("data", handle_data, 1)

	print "Press any key to start..."
	raw_input()
	api.set("/qk/conns/0/cmds/start")
	api.set("/qk/conns/1/cmds/start")

	raw_input()
	api.set("/qk/conns/0/cmds/stop")
	api.set("/qk/conns/1/cmds/stop")

	api.disconnect()
	print "Done"
