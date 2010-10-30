
import struct
import json
import socket


s = socket.create_connection (("127.0.0.1", 10808))

xid = 87717
call = 0
js = 3
prog = 10808
vers = 1
proc = 1
hdr = struct.pack (">" + "L" * 6, xid, call, js, prog, vers, proc);

# make an XDR "Auth" Field
hdr += struct.pack ("x" * 4 * 4) 

date = { "date" : { "on" : True, 
                    "date" : { "mday" : 27, "mon" : 11, "year" : 1977 } },
         "time" : { "on" : False} }
data = { "d" : date, "i" : 3000, "pk" : 4004, "d2" : date}
put_arg = { "key" : "test-key-1",
            "data" : data }

packet = hdr + json.dumps (put_arg)
plen = len(packet)
rem = plen % 4
if rem != 0:
    packet += struct.pack ("x" * (4 - rem))

full_packet = struct.pack (">L", len (packet) + 0x80000000) + packet

print full_packet
s.send (full_packet)

packlen_struct = s.recv (4)
print (len (packlen_struct))
print packlen_struct
packlen = struct.unpack (">L", packlen_struct)
packlen = packlen[0] & 0x7ffffff
print packlen
stuff = s.recv (packlen)
(xid,reply) = struct.unpack(">LL", stuff[0:8])
stuff = stuff[24:]
print "XID: " + str (xid)
print "Reply bit: " + str (reply)
print stuff
