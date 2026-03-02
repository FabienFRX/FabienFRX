#!/usr/bin/python3
from ia.src.Network.Socket import Socket
from ia.src.Player.Player import Player
import sys

def usage(exit_code):
    print("USAGE: ./zappy_ai -p port -n name -h machine")
    print("\t-p port: port number of the server")
    print("\t-n name: name of the team")
    print("\t-h machine: name of the machine; localhost by default")
    sys.exit(exit_code)

def getPort():
    try:
        return int(sys.argv[2])
    except:
        print("Invalid port number")
        sys.exit(84)

def getMachine():
    if len(sys.argv) > 6:
        return sys.argv[6]
    return "127.0.0.1"
        
def manageWelcome(name, client_socket, debug=False):
    try:
        data = client_socket.receive()
        if data.strip() == "WELCOME":
            if debug:
                print("[ZappyAI]: Received: WELCOME")
            client_socket.send(f"{name}\n")

        data = client_socket.receive()
        lines = data.strip().split('\n')
        slots = x = y = None

        for line in lines:
            if line.isdigit():
                slots = int(line)
                if debug:
                    print(f"[ZappyAI]: Available slots: {slots}")
            elif " " in line:
                x, y = map(int, line.split())
                try:
                    if debug:
                        print(f"[ZappyAI]: Map dimensions: {x} x {y}")
                except ValueError:
                    print("Error : invalid map dimensions.")

        return slots, (x, y)
    except:
        print("Error: Failed to manage welcome message.")
        sys.exit(84)        

def manageData(name, client_socket, debug=False):
    try:
        player = Player(name, client_socket, debug)
        player.run()
    except KeyboardInterrupt:
        print("Client interrupted.")
    finally:
        client_socket.close()
        print("Connection closed.")


def main():
    if len(sys.argv) < 5 or (sys.argv[1] != "-p" or sys.argv[3] != "-n" or sys.argv[5] != "-h"):
        usage(84)
    port = getPort()
    name = sys.argv[4]
    host = getMachine()
    debug = False
    if ("-d" in sys.argv or "--debug" in sys.argv):
        debug = True
    client_socket = Socket(host, port)
    client_socket.connect()
    manageData(name, client_socket, debug)

main()