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

    def send_structured_broadcast(self, msg_type, **kwargs):
        try:
            data = {"type": msg_type, **kwargs}
            message = json.dumps(data)
            print(f"[BROADCAST] Sending: {message}")
            self.command_queue.append(f"Broadcast {message}")
        except Exception as e:
            print(f"Failed to build broadcast: {e}")

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

    def get_missing_tile_resources(self, tile_str):
        required = self.get_evolution_requirements()
        if not required:
            return {}
        present = {k: 0 for k in required if k != "players"}
        for res in present:
            present[res] = tile_str.count(res)
        missing = {}
        for res, needed in required.items():
            if res == "players":
                continue
            if present[res] < needed:
                missing[res] = needed - present[res]
        return missing

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

    def has_all_resources_for_next_level(self):
        next_level = self.level + 1
        requirements = self.get_evolution_requirements_for_level(next_level)
        if not requirements:
            return False
        for resource, needed in requirements.items():
            if resource == "players":
                continue
            if self.inventory.get(resource, 0) < needed:
                return False
        return True

    def has_all_resources_for_current_level(self):
        requirements = self.get_evolution_requirements()
        if not requirements:
            return False
        for resource, needed in requirements.items():
            if resource == "players":
                continue
            if self.inventory.get(resource, 0) < needed:
                return False
        return True

    def update_phase(self):
        food = self.inventory["food"]
        old_phase = self.phase
        if food < 3:
            self.phase = "COLLECT"
            self.is_evolution_master = False
            return
        
        if food < 20:
            if self.phase != "COLLECT":
                print(f"[FOOD LOW] Staying in COLLECT phase (food={food})")
            self.phase = "COLLECT"
            self.is_evolution_master = False
            return
        if not self.has_all_resources_for_current_level():
            self.phase = "COLLECT"
            self.is_evolution_master = False
            return

        current_required = self.get_evolution_requirements()
        players_needed = current_required.get("players", 1) if current_required else 1
        if self.level == 1 and players_needed == 1:
            if food < 8:
                if self.phase != "COLLECT":
                    print(f"[LEVEL 1->2] Not enough food for incantation (food={food} < 20), staying in COLLECT")
                self.phase = "COLLECT"
                self.is_evolution_master = False
                return
            else:
                self.phase = "READY_TO_EVOLVE"
        elif players_needed == 1:
            self.phase = "READY_TO_EVOLVE"
        else:
            if not self.is_evolution_master and self.phase != "JOINING_EVOLUTION":
                self.phase = "READY_TO_EVOLVE"
                self.is_evolution_master = True
                print(f"Becoming evolution master for level {self.level}->{self.level+1}")

        if old_phase != self.phase:
            print(f"Phase: {old_phase} -> {self.phase}")

    def count_players_in_look(self, tiles):
        current_tile = tiles[0].strip() if tiles else ""
        player_count = current_tile.count("player")
        return max(1, player_count)

    def calculate_direction_to_target(self, target_x, target_y):
        dx = target_x - self.current_x
        dy = target_y - self.current_y
        
        if abs(dx) > 5:
            dx = dx - 10 if dx > 0 else dx + 10
        if abs(dy) > 5:
            dy = dy - 10 if dy > 0 else dy + 10
        
        if dx == 0 and dy == 0:
            return []
        
        if dx > 0:
            target_orientation = 2
        elif dx < 0:
            target_orientation = 4
        elif dy > 0:
            target_orientation = 3
        else:
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
                    if i <= 3:
                        self.command_queue = ["Forward", "Look"]
                    elif i <= 8:
                        self.command_queue = ["Right", "Forward", "Look"]
                    elif i <= 13:
                        self.command_queue = ["Left", "Forward", "Look"]
                    else:
                        self.command_queue = ["Forward", "Forward", "Look"]
                    return
            import random
            if random.random() < 0.3:
                self.command_queue = ["Right", "Forward", "Look"]
            else:
                self.command_queue = ["Forward", "Look"]

    def handle_look_working(self, look_data):
        try:
            clean_data = look_data.replace("[", "").replace("]", "")
            tiles = clean_data.split(",")
            self.visible_tiles = tiles
            current_tile = tiles[0].strip() if tiles else ""
            
            players_on_tile = self.count_players_in_look(tiles)
            
            print(f"Phase: {self.phase} | Tile: '{current_tile}' | Players: {players_on_tile}")

            if self.phase == "COLLECT":
                missing_resource = self.get_missing_resource()
                if missing_resource and missing_resource in current_tile:
                    print(f"[PRIORITÉ] Prise immédiate de {missing_resource} sur la case !")
                    self.command_queue = [f"Take {missing_resource}", "Inventory"]
                    self.stagnation_counter = 0
                    return

                if self.inventory["food"] < 5:
                    print("[SURVIE CRITIQUE] Collecte exclusive de nourriture!")
                    self._handle_resource_collection(tiles, "food")
                    self.stagnation_counter = 0
                    return
                
                if self.inventory["food"] < 12:
                    print(f"[SURVIE] Collecte de nourriture (food={self.inventory['food']})")
                    self._handle_resource_collection(tiles, "food")
                    self.stagnation_counter = 0
                    return

                missing_resource = self.get_missing_resource()
                if missing_resource:
                    if self.inventory["food"] < 5:
                        print(f"[CRITIQUE] Collecte de nourriture prioritaire (food={self.inventory['food']})")
                        self._handle_resource_collection(tiles, "food")
                    elif self.inventory["food"] < 8:
                        import random
                        if random.random() < 0.6:
                            print(f"[BASSE] Collecte de nourriture (food={self.inventory['food']})")
                            self._handle_resource_collection(tiles, "food")
                        else:
                            print(f"[BASSE] Collecte du missing resource: {missing_resource}")
                            self._handle_resource_collection(tiles, missing_resource)
                    else:
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
                        if not self.command_queue:
                            import random
                            move = random.choice(["Forward", "Left", "Right"])
                            print(f"[EXPLORATION] No resource to collect, moving: {move}")
                            self.command_queue = [move, "Look"]
                        return
                    else:
                        print("All resources collected, looking for evolution partner...")
                        center_x, center_y = 5, 5
                        if abs(self.current_x - center_x) > 1 or abs(self.current_y - center_y) > 1:
                            print(f"Moving to center ({center_x}, {center_y}) to find partners")
                            navigation_commands = self.calculate_direction_to_target(center_x, center_y)
                            if navigation_commands:
                                self.command_queue = navigation_commands + ["Look"]
                            else:
                                self.command_queue = ["Look"]
                        else:
                            evolution_call = {
                                "type": "evolution_call",
                                "level": self.level,
                                "x": self.current_x,
                                "y": self.current_y,
                                "players_needed": self.get_evolution_requirements().get("players", 1)
                            }
                            message = json.dumps(evolution_call)
                            print(f"Broadcasting evolution call from center: {message}")
                            self.command_queue = [f"Broadcast {message}"]
                            self.last_broadcast_time = time.time()
                            self.phase = "WAITING_FOR_PLAYERS"
                return
            if self.phase in ["READY_TO_EVOLVE", "WAITING_FOR_PLAYERS"]:
                missing_tile = self.get_missing_tile_resources(current_tile)
                if missing_tile:
                    for res, nb in missing_tile.items():
                        if self.inventory.get(res, 0) > 0:
                            print(f"Posing {res} on tile for incantation")
                            self.command_queue = [f"Set {res}"]
                            return
                    print("Waiting for all resources to be on the tile before incantation.")
                    self.command_queue = ["Look"]
                    return
                required = self.get_evolution_requirements()
                players_needed = required.get("players", 1)
                if self.phase == "READY_TO_EVOLVE":
                    if players_needed == 1:
                        print("Solo evolution!")
                        self.command_queue = ["Incantation"]
                        self.phase = "EVOLVING"
                    else:
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
                    return
                elif self.phase == "WAITING_FOR_PLAYERS":
                    if players_on_tile >= players_needed:
                        print(f"Enough players on tile ({players_on_tile} >= {players_needed})! Starting evolution!")
                        self.command_queue = ["Incantation"]
                        self.phase = "EVOLVING"
                        return
                    if time.time() - self.last_broadcast_time > 5:
                        message = json.dumps({"type": "evolution_call", "level": self.level, "x": self.current_x, "y": self.current_y, "players_needed": players_needed})
                        print(f"[WAITING] Broadcasting again: {message}")
                        self.command_queue.append(f"Broadcast {message}")
                        self.last_broadcast_time = time.time()
                    for res, nb in missing_tile.items():
                        if self.inventory.get(res, 0) > 0:
                            print(f"Posing {res} on tile while waiting")
                            self.command_queue = [f"Set {res}"]
                            return
                    missing_resource = self.get_missing_resource()
                    if missing_resource:
                        print(f"Waiting for players but collecting missing resource: {missing_resource}")
                        self._handle_resource_collection(tiles, missing_resource)
                        return
                    self.command_queue.append("Look")
                    return

            if self.phase == "JOINING_EVOLUTION":
                if (self.current_x == self.target_evolution_x and 
                    self.current_y == self.target_evolution_y):
                    print("Arrived at evolution site!")
                    missing_tile = self.get_missing_tile_resources(current_tile)
                    for res, nb in missing_tile.items():
                        if self.inventory.get(res, 0) > 0:
                            print(f"Posing {res} on tile for incantation (join)")
                            self.command_queue = [f"Set {res}"]
                            return
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
                return
            
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
            if message.startswith("message "):
                parts = message[len("message "):].split(", ", 1)
                if len(parts) != 2:
                    return
                direction_part, content = parts
                try:
                    direction = int(direction_part)
                except ValueError:
                    print("Invalid direction in broadcast")
                    return
                try:
                    data = json.loads(content)
                except json.JSONDecodeError:
                    print("Invalid JSON in broadcast")
                    return
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
                        response = json.dumps({
                            "type": "evolution_response",
                            "level": self.level,
                            "x": self.current_x,
                            "y": self.current_y,
                            "status": "joining"
                        })
                        self.command_queue.append(f"Broadcast {response}")
                elif data.get("type") == "evolution_response":
                    level = data.get("level")
                    status = data.get("status")
                    if level == self.level and status == "joining":
                        print("Another player is joining our evolution!")
                        self.evolution_participants += 1
        except Exception as e:
            print(f"Broadcast parse error: {e}")

    def get_next_command(self):
        current_time = time.time()
        if current_time - self.last_command_time < 0.07:
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

                self.command_queue.clear()
                self.command_queue = ["Inventory", "Look"]
                print("Continuing game at new level, searching for food...")
                self.update_phase()
                print(f"[LEVEL UP] Phase after level-up: {self.phase}")

            except Exception as e:
                print(f"Level parse error: {e}")
            return True

        if self.current_command == "Inventory":
            self.parse_inventory_simple(message)
            self.command_queue.append("Look")

        elif self.current_command == "Look":
            self.handle_look_working(message)
            self.update_phase()

        elif self.current_command.startswith("Take"):
            if "ok" in message:
                print("Successfully took item!")
                self.command_queue.append("Inventory")
            else:
                print("Failed to take item")

        elif self.current_command == "Incantation":
            if "ko" in message:
                print("Incantation failed!")
                self.phase = "COLLECT"
                self.is_evolution_master = False
                self.waiting_start_time = None
                self.target_evolution_x = None
                self.target_evolution_y = None
            elif "Elevation underway" in message:
                print("Evolution started!")

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
