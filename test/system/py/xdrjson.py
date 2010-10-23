
import struct
import json
import socket


s = socket.create_connection (("127.0.0.1", 10808))

xid = 87717
call = 0
js = 2
prog = 10808
vers = 1
proc = 1
hdr = struct.pack (">" + "L" * 6, xid, call, js, prog, vers, proc);
hdr += struct.pack ("x" * 4 * 8)

date = { "date" : { "on" : True, 
                    "date" : { "mday" : 27, "mon" : 11, "year" : 1977 } },
         "time" : { "on" : False} }
data = { "d" : date, "i" : 3000, "pk" : 4004, "d2" : date}
put_arg = { "key" : "test-key-1",
            "data" : data }

packet = hdr + str (data)
plen = len(packet)
rem = plen % 4
if rem != 0:
    packet += struct.pack ("x" * rem)

full_packet = struct.pack (">L", len (packet) + 0x80000000) + packet

s.send (full_packet)
