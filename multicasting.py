import platform, time, socket, select

addr = ("239.255.2.9", 1600)

sendSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sendSock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 24)
sendSock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_IF,
                    socket.inet_aton("127.0.0.1"))

recvSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
recvSock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)
if hasattr(socket, 'SO_REUSEPORT'):
    recvSock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, True)

recvSock.bind(("0.0.0.0", addr[1]))




status = recvSock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP,
                             socket.inet_aton(addr[0]) + socket.inet_aton("127.0.0.1"));

while 1:
    pkt = "Hello host: {1} time: {0}".format(time.clock(), platform.node())
    print "SEND to: {0} data: {1}".format(addr, pkt)
    r = sendSock.sendto(pkt, addr)

    while select.select([recvSock], [], [], 0)[0]:
        data, fromAddr = recvSock.recvfrom(1024)
        print "RECV from: {0} data: {1}".format(fromAddr, data)

    time.sleep(1)