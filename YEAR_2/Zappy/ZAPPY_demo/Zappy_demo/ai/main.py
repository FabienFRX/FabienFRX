import socket
import argparse
import selectors
import time
import re
import json

class WorkingEvolutionAI:
    def __init__(self, hostname, port, team_name):
        self.hostname = hostname
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.selectors = selectors.DefaultSelector()
        
        self.handshake_step = 0
        self.pending_send = ""
        
        self.inventory = {
            "food": 10, "linemate": 0, "deraumere": 0, "sibur": 0, 
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        self.level = 1
        self.last_command_time = 0
        self.command_queue = []
        self.waiting_for_response = False
        self.current_command = ""
        
        self.current_x = 0
        self.current_y = 0
        self.orientation = 1
        
        self.phase = "COLLECT"
        self.is_evolution_master = False
        self.evolution_participants = 1
        self.shared_inventory = {}
        self.target_evolution_x = None
        self.target_evolution_y = None
        self.waiting_start_time = None
        self.stagnation_counter = 0
        self.last_broadcast_time = 0
        
        print(f"Working Evolution AI - Level: {self.level}")

    def get_evolution_requirements(self):
        requirements = {
            1: {"linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0, "players": 1},
            2: {"linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0, "players": 2},
            3: {"linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0, "players": 2},
            4: {"linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0, "players": 4},
            5: {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0, "players": 4},
            6: {"linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0, "players": 6},
            7: {"linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1, "players": 6},
        }
        return requirements.get(self.level, {})

    def get_evolution_requirements_for_level(self, level):
        requirements = {
            1: {"linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0, "players": 1},
            2: {"linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0, "players": 2},
            3: {"linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0, "players": 2},
            4: {"linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0, "players": 4},
            5: {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0, "players": 4},
            6: {"linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0, "players": 6},
            7: {"linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1, "players": 6},
        }
        return requirements.get(level, {})

    def can_evolve_alone(self):
        required = self.get_evolution_requirements()
        players_needed = required.get("players", 1)
        
        if players_needed > 1:
            return False
            
        for resource, needed in required.items():
            if resource == "players":
                continue
            if self.inventory.get(resource, 0) < needed:
                return False
        
        return True

    def has_evolution_resources(self):
        required = self.get_evolution_requirements()
        
        for resource, needed in required.items():
            if resource == "players":
                continue
            if self.inventory.get(resource, 0) < needed:
                return False
        
        return True

    def get_missing_resource(self):
        required = self.get_evolution_requirements()
        
        for resource, needed in required.items():
            if resource == "players":
                continue
            current = self.inventory.get(resource, 0)
            if current < needed:
                return resource
        
        return None

    def connect_to_server(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setblocking(False)
        result = self.socket.connect_ex((self.hostname, int(self.port)))
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.selectors.register(self.socket, events)
        print(f"Connecting to {self.hostname}:{self.port}...")

    def handle_handshake(self, message):
        print(f"Handshake step {self.handshake_step}: {message}")
        
        if self.handshake_step == 0 and "WELCOME" in message:
            self.pending_send = self.team_name + "\n"
            self.handshake_step = 1
            
        elif self.handshake_step == 1 and message.strip().isdigit():
            self.handshake_step = 2
            
        elif self.handshake_step == 2 and len(message.split()) == 2:
            self.handshake_step = 3
            print("Handshake complete! Starting working evolution AI...")
            self.command_queue = ["Inventory", "Look"]
            
        return self.handshake_step >= 3

    def parse_inventory_simple(self, data):
        try:
            resources = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
            
            for resource in resources:
                match = re.search(rf'{resource}\s+(\d+)', data)
                if match:
                    old_val = self.inventory.get(resource, 0)
                    new_val = int(match.group(1))
                    self.inventory[resource] = new_val
                    if old_val != new_val and resource != "food":
                        print(f"{resource}: {old_val} -> {new_val}")
            
            print(f"Level {self.level} | food={self.inventory['food']} | Phase: {self.phase}")
            self.update_phase()
            
        except Exception as e:
            print(f"Parse error: {e}")

    def update_phase(self):
        food = self.inventory["food"]
        old_phase = self.phase
        
        if food < 3:
            self.phase = "COLLECT"
            self.is_evolution_master = False
            return
        
        current_required = self.get_evolution_requirements_for_level(self.level)
        has_all_resources = True
        if current_required:
            for resource, needed in current_required.items():
                if resource == "players":
                    continue
                if self.inventory.get(resource, 0) < needed:
                    has_all_resources = False
                    break
        else:
            has_all_resources = False
        
        if has_all_resources:
            players_needed = current_required.get("players", 1)
            if players_needed == 1:
                self.phase = "READY_TO_EVOLVE"
            else:
                if not self.is_evolution_master and self.phase != "JOINING_EVOLUTION":
                    self.phase = "READY_TO_EVOLVE"
                    self.is_evolution_master = True
                    print(f"Becoming evolution master for level {self.level}->{self.level+1}")
        else:
            self.phase = "COLLECT"
            self.is_evolution_master = False
        
        if old_phase != self.phase:
            print(f"Phase: {old_phase} -> {self.phase}")

    def count_players_in_look(self, tiles):
        current_tile = tiles[0].strip() if tiles else ""
        player_count = current_tile.count("player")
        return max(1, player_count)

    def calculate_direction_to_target(self, target_x, target_y):
        """Calcule la direction vers la cible et retourne les commandes nécessaires"""
        dx = target_x - self.current_x
        dy = target_y - self.current_y
        
        if abs(dx) > 5:
            dx = dx - 10 if dx > 0 else dx + 10
        if abs(dy) > 5:
            dy = dy - 10 if dy > 0 else dy + 10
        
        if dx == 0 and dy == 0:
            return []
        
        if dx > 0:  # Est
            target_orientation = 2
        elif dx < 0:  # Ouest
            target_orientation = 4
        elif dy > 0:  # Sud
            target_orientation = 3
        else:  # Nord
            target_orientation = 1
        
        commands = []
        
        while self.orientation != target_orientation:
            if (self.orientation == 1 and target_orientation == 2) or \
               (self.orientation == 2 and target_orientation == 3) or \
               (self.orientation == 3 and target_orientation == 4) or \
               (self.orientation == 4 and target_orientation == 1):
                commands.append("Right")
            else:
                commands.append("Left")
        
        # Avancer
        commands.append("Forward")
        return commands

    def _handle_resource_collection(self, tiles, target_resource):
        current_tile = tiles[0].strip() if tiles else ""
        
        if target_resource in current_tile:
            print(f"Taking {target_resource}!")
            self.command_queue = [f"Take {target_resource}", "Inventory"]
        else:
            for i, tile in enumerate(tiles[1:], 1):
                if target_resource in tile:
                    print(f"Moving toward {target_resource} in tile {i}")
                    self.command_queue = ["Forward", "Look"]
                    return
            self.command_queue = ["Forward", "Look"]

    def handle_look_working(self, look_data):
        try:
            clean_data = look_data.replace("[", "").replace("]", "")
            tiles = clean_data.split(",")
            current_tile = tiles[0].strip() if tiles else ""
            
            players_on_tile = self.count_players_in_look(tiles)
            
            print(f"Phase: {self.phase} | Tile: '{current_tile}' | Players: {players_on_tile}")
            
            if self.inventory["food"] < 10:
                print("[SURVIE] Collecte exclusive de nourriture!")
                self._handle_resource_collection(tiles, "food")
                self.stagnation_counter = 0
                return
            if self.phase == "COLLECT" or self.inventory["food"] < 5:
                if self.inventory["food"] < 5:
                    print("[SURVIE] Food trop basse, retour en mode survie!")
                missing_resource = self.get_missing_resource()
                if missing_resource:
                    print(f"Collecting missing resource: {missing_resource}")
                    self._handle_resource_collection(tiles, missing_resource)
                    self.stagnation_counter += 1
                    if self.stagnation_counter > 10:
                        message = json.dumps({"type": "evolution_call", "level": self.level, "x": self.current_x, "y": self.current_y, "players_needed": self.get_evolution_requirements().get("players", 1)})
                        print(f"[STAGNATION] Broadcasting for help: {message}")
                        self.command_queue = [f"Broadcast {message}"]
                        self.stagnation_counter = 0
                    return
                else:
                    self.stagnation_counter = 0
                    self.update_phase()
                    if self.phase == "COLLECT":
                        self._handle_resource_collection(tiles, "food")
                    else:
                        self.command_queue = ["Look"]
                return
            # PHASES D'EVOLUTION ET SYNCHRO : inchangées
            if self.phase == "READY_TO_EVOLVE":
                required = self.get_evolution_requirements()
                players_needed = required.get("players", 1)
                
                if players_needed == 1:
                    print("Solo evolution!")
                    commands = []
                    for resource, needed in required.items():
                        if resource == "players":
                            continue
                        current = self.inventory.get(resource, 0)
                        for _ in range(min(needed, current)):
                            commands.append(f"Set {resource}")
                    commands.append("Incantation")
                    self.command_queue = commands
                    self.phase = "EVOLVING"
                    
                else:
                    if not self.is_evolution_master:
                        self.is_evolution_master = True
                        print(f"Becoming evolution master for level {self.level}->{self.level+1}")
                    self.target_evolution_x = self.current_x
                    self.target_evolution_y = self.current_y
                    self.waiting_start_time = time.time()
                    evolution_call = {
                        "type": "evolution_call",
                        "level": self.level,
                        "x": self.current_x,
                        "y": self.current_y,
                        "players_needed": players_needed
                    }
                    message = json.dumps(evolution_call)
                    print(f"Broadcasting evolution call: {message}")
                    self.command_queue = [f"Broadcast {message}"]
                    self.last_broadcast_time = time.time()
                    self.phase = "WAITING_FOR_PLAYERS"
            elif self.phase == "WAITING_FOR_PLAYERS":
                required = self.get_evolution_requirements()
                players_needed = required.get("players", 1)
                print(f"Waiting for players: {players_on_tile}/{players_needed}")
                if self.inventory["food"] < 5:
                    print("Food is low, switching to survival mode!")
                    self.phase = "COLLECT"
                    self.command_queue = ["Look"]
                    return
                if self.waiting_start_time and (time.time() - self.waiting_start_time) > 30:
                    print("Timeout waiting for players, going back to collect mode!")
                    self.phase = "COLLECT"
                    self.is_evolution_master = False
                    self.waiting_start_time = None
                    self.command_queue = ["Look"]
                    return
                if players_on_tile >= players_needed:
                    print("Enough players! Starting evolution!")
                    commands = []
                    for resource, needed in required.items():
                        if resource == "players":
                            continue
                        current = self.inventory.get(resource, 0)
                        for _ in range(min(needed, current)):
                            commands.append(f"Set {resource}")
                    commands.append("Incantation")
                    self.command_queue = commands
                    self.phase = "EVOLVING"
                else:
                    if time.time() - self.last_broadcast_time > 5:
                        message = json.dumps({"type": "evolution_call", "level": self.level, "x": self.current_x, "y": self.current_y, "players_needed": players_needed})
                        print(f"[WAITING] Broadcasting again: {message}")
                        self.command_queue.append(f"Broadcast {message}")
                        self.last_broadcast_time = time.time()
                    if self.inventory["food"] < 8:
                        print("Waiting for players but food is getting low, collecting food...")
                        self._handle_resource_collection(tiles, "food")
                    else:
                        self.command_queue.append("Look")
            elif self.phase == "JOINING_EVOLUTION":
                if (self.current_x == self.target_evolution_x and 
                    self.current_y == self.target_evolution_y):
                    print("Arrived at evolution site!")
                    self.phase = "WAITING_FOR_PLAYERS"
                    self.waiting_start_time = time.time()
                    self.command_queue = ["Look"]
                else:
                    print(f"Moving to evolution site ({self.target_evolution_x}, {self.target_evolution_y})")
                    navigation_commands = self.calculate_direction_to_target(self.target_evolution_x, self.target_evolution_y)
                    if navigation_commands:
                        self.command_queue = navigation_commands + ["Look"]
                    else:
                        self.command_queue = ["Look"]
            elif self.phase == "EVOLVING":
                print("Currently evolving, waiting for completion...")
                self.command_queue = ["Look"]
            else:
                self.command_queue = ["Look"]
        except Exception as e:
            print(f"Look error: {e}")
            self.command_queue = ["Look"]

    def handle_broadcast_message(self, message):
        try:
            if "message " in message:
                parts = message.split(", ", 1)
                if len(parts) == 2:
                    direction = int(parts[0].split()[1])
                    content = parts[1]
                    
                    try:
                        data = json.loads(content)
                        
                        if data.get("type") == "evolution_call":
                            level = data.get("level")
                            x = data.get("x")
                            y = data.get("y")
                            players_needed = data.get("players_needed")
                            
                            print(f"Evolution call for level {level} at ({x},{y}), need {players_needed} players")
                            
                            if (level == self.level and 
                                self.has_evolution_resources() and 
                                self.phase in ["COLLECT", "READY_TO_EVOLVE"] and
                                not self.is_evolution_master):
                                
                                print(f"Joining evolution at ({x},{y})")
                                self.target_evolution_x = x
                                self.target_evolution_y = y
                                self.waiting_start_time = time.time()
                                self.phase = "JOINING_EVOLUTION"
                                self.command_queue = ["Look"]
                                
                    except json.JSONDecodeError:
                        pass
                        
        except Exception as e:
            print(f"Broadcast parse error: {e}")

    def get_next_command(self):
        current_time = time.time()
        
        if current_time - self.last_command_time < 0.3:
            return None
        
        if self.command_queue:
            cmd = self.command_queue.pop(0)
            print(f"Executing: {cmd}")
            return cmd
        
        return "Look"

    def handle_game_response(self, message):
        print(f"Response to '{self.current_command}': {message}")
        
        if "dead" in message.lower():
            print("Player died!")
            return False
        
        if message.startswith("message "):
            self.handle_broadcast_message(message)
            return True
        
        if "Elevation underway" in message:
            print("Evolution started!")
            return True
        elif "Current level:" in message:
            try:
                new_level = int(''.join(filter(str.isdigit, message)))
                old_level = self.level
                self.level = new_level
                print(f"LEVEL UP! {old_level} -> {new_level}")
                
                self.phase = "COLLECT"
                self.is_evolution_master = False
                self.evolution_participants = 1
                self.target_evolution_x = None
                self.target_evolution_y = None
                
                required = self.get_evolution_requirements()
                if required:
                    print(f"Next level needs: {required}")
                    players_needed = required.get("players", 1)
                    if players_needed > 1:
                        print(f"Will need {players_needed} players for next evolution!")
                else:
                    print("MAX LEVEL REACHED!")
                
                self.command_queue = ["Inventory", "Look"]
                print("Continuing game at new level...")
            except:
                pass
            return True
        
        if self.current_command == "Inventory":
            self.parse_inventory_simple(message)
            self.command_queue.append("Look")
            
        elif self.current_command == "Look":
            self.handle_look_working(message)
            
        elif self.current_command.startswith("Take"):
            if "ok" in message:
                print("Successfully took item!")
                self.command_queue.append("Inventory")
            else:
                print("Failed to take item")
        
        elif self.current_command.startswith("Set"):
            if "ok" in message:
                print("Successfully dropped item!")
            else:
                print("Failed to drop item")
                
        elif self.current_command.startswith("Broadcast"):
            if "ok" in message:
                print("Broadcast sent successfully!")
            else:
                print("Broadcast failed")
                
        elif self.current_command == "Incantation":
            if "ko" in message:
                print("Incantation failed!")
                self.phase = "COLLECT"
                self.is_evolution_master = False
                
        elif self.current_command == "Forward":
            if "ok" in message:
                if self.orientation == 1:
                    self.current_y = (self.current_y - 1) % 10
                elif self.orientation == 2:
                    self.current_x = (self.current_x + 1) % 10
                elif self.orientation == 3:
                    self.current_y = (self.current_y + 1) % 10
                elif self.orientation == 4:
                    self.current_x = (self.current_x - 1) % 10
                print(f"Position: ({self.current_x}, {self.current_y})")
                
        elif self.current_command in ["Right", "Left"]:
            if "ok" in message:
                if self.current_command == "Right":
                    self.orientation = (self.orientation % 4) + 1
                else:
                    self.orientation = ((self.orientation - 2) % 4) + 1
                    if self.orientation <= 0:
                        self.orientation += 4
                print(f"New orientation: {self.orientation}")
                
        self.waiting_for_response = False
        self.current_command = ""
        
        return True

    def launch_client(self):
        message_buffer = ""
        game_started = False
        
        while True:
            try:
                events = self.selectors.select(timeout=1.0)
                
                for key, mask in events:
                    if mask & selectors.EVENT_READ:
                        try:
                            data = self.socket.recv(1024).decode("utf-8")
                            if not data:
                                print("Connection closed by server")
                                return
                                
                            message_buffer += data
                            
                        except Exception as e:
                            print(f"Receive error: {e}")
                            continue
                        
                        lines = message_buffer.split("\n")
                        message_buffer = lines[-1]
                        
                        for line in lines[:-1]:
                            if not line.strip():
                                continue
                                
                            print(f"Received: {line}")
                            
                            if not game_started:
                                game_started = self.handle_handshake(line)
                            else:
                                if not self.handle_game_response(line):
                                    return

                    if mask & selectors.EVENT_WRITE:
                        if self.pending_send:
                            try:
                                sent = self.socket.send(self.pending_send.encode())
                                print(f"Sent: {repr(self.pending_send[:sent])}")
                                self.pending_send = self.pending_send[sent:]
                                
                            except socket.error as e:
                                print(f"Send error: {e}")
                        
                        elif game_started and not self.waiting_for_response:
                            next_cmd = self.get_next_command()
                            if next_cmd:
                                self.pending_send = next_cmd + "\n"
                                self.current_command = next_cmd
                                self.waiting_for_response = True
                                self.last_command_time = time.time()
                                print(f"AI command: {next_cmd}")
                            
            except KeyboardInterrupt:
                print("\nInterrupted by user")
                break
            except Exception as e:
                print(f"Main loop error: {e}")
                break

def main():
    parser = argparse.ArgumentParser(description="Working Evolution Zappy AI")
    parser.add_argument("-p", "--port", type=int, required=True, help="Server port")
    parser.add_argument("-n", "--team", type=str, required=True, help="Team name")
    parser.add_argument("--host", type=str, default="127.0.0.1", help="Server host")
    args = parser.parse_args()

    client = WorkingEvolutionAI(args.host, args.port, args.team)
    
    try:
        client.connect_to_server()
        print(f"Connected to {args.host}:{args.port} as team {args.team}")
        client.launch_client()
        
    except Exception as e:
        print(f"Client error: {e}")
    finally:
        if client.socket:
            client.socket.close()
            print("Socket closed")

if __name__ == "__main__":
    main()
