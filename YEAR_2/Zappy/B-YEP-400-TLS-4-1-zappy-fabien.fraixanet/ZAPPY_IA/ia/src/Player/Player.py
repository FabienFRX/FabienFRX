##
## EPITECH PROJECT, 2025
## ZAPPY_IA
## File description:
## Player
##

import enum
import sys
from ..Network.Command import Command
import random
import re
import time

class Player:
    def __init__(self, name, client_socket, debug=False):
        self.name = name
        self.client_socket = client_socket
        self.debug = debug
        self.slots = None
        self.level = 0
        self.map_size = (None, None)
        self.last_command = None
        self.current_incantation_call = None
        self.player_id = hash(name) % 1000000
        self.is_broadcasting = False
        self.mode = "DEFAULT"
        self.broadcast_direction = None
        self.broadcast_target_level = None
        self.broadcast_leader_id = None
        self.direction = None
        self.level_requirements = {
            1: {"players": 1, "food": 0, "linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0},
            2: {"players": 2, "food": 0, "linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0},
            3: {"players": 2, "food": 0, "linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0},
            4: {"players": 4, "food": 0, "linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0},
            5: {"players": 4, "food": 0, "linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0},
            6: {"players": 6, "food": 0, "linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0},
            7: {"players": 6, "food": 0, "linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1},
            8: {"players": 8, "food": 0, "linemate": 3, "deraumere": 3, "sibur": 3, "mendiane": 3, "phiras": 3, "thystame": 2}
        }
        self.total_requirements = {
            "players": 4,
            "food": 0,
            "linemate": 3,
            "deraumere": 2,
            "sibur": 3,
            "mendiane": 0,
            "phiras": 3,
            "thystame": 0
        }

    def respond_to_broadcast(self):
        if self.broadcast_direction is None:
            return

        if self.debug:
            print(f"[ZappyAI]: Mode JOIN actif, déplacement vers direction {self.broadcast_direction}")

        if self.broadcast_direction == 0:
            if self.debug:
                print("[ZappyAI]: Déjà sur la case du broadcast")
            self.mode = "DEFAULT"
            self.broadcast_direction = None
            return

        look = self.look_around()
        if look:
            objects_tile = self.parse_tile_objects(look)
            current_players = objects_tile.get(0, []).count("player")
            if self.level < 8:
                next_level = self.level + 1
                needed_players = self.level_requirements[next_level]["players"]
                if current_players >= needed_players:
                    if self.debug:
                        print(f"[ZappyAI]: Found enough players ({current_players}/{needed_players}), stopping JOIN mode")
                    self.mode = "DEFAULT"
                    self.broadcast_direction = None
                    return

        look = self.look_around()
        if look:
            objects_tile = self.parse_tile_objects(look)
            if "player" in objects_tile.get(0, []):
                self.mode = "INCANTATION_READY"
                self.broadcast_direction = None
                if self.debug:
                    print("[ZappyAI]: Arrived on broadcast tile, ready for incantation.")
                return
            for tile_pos, objects in objects_tile.items():
                if "player" in objects and tile_pos != 0:
                    self.move_to_tile(tile_pos)
                    return

        self.move_towards_broadcast(self.broadcast_direction)

    def welcome(self):
        self.slots, self.map_size = self.manageWelcome(self.name, self.client_socket, self.debug)
        if self.slots is None or self.map_size == (None, None):
            print("Error: No available slots or invalid map size.")
            sys.exit(84)

    def manageWelcome(self, name, client_socket, debug=False):
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
        except Exception as e:
            print(f"Error: Failed to manage welcome message. {e}")
            sys.exit(84)

    def run(self):
        try:
            self.welcome()
            self.game_loop()
        except KeyboardInterrupt:
            print("Player interrupted.")
            sys.exit(0)

    def receive_message(self):
        message = self.client_socket.receive()
        if message is not None and len(message.strip()) > 0:
            message = message.strip()
            if self.debug:
                print(f"[ZappyAI]: Received message: {message}")
            lines = message.split('\n')
            for line in lines:
                line = line.strip()
                if line.startswith("message"):
                    match = re.match(r"message (\d+), (.+)", line)
                    if match:
                        direction = int(match.group(1))
                        content = match.group(2).strip()
                        self.direction = direction
                        self.broadcast_direction = direction
                        self.handle_broadcast_message(content)
                    else:
                        if self.debug:
                            print(f"[ZappyAI]: Failed to parse broadcast message: {line}")
            for line in lines:
                line = line.strip()
                if line and not line.startswith("message"):
                    return line
            return message
        return None

    def handle_broadcast_message(self, message):
        if message.startswith("INCANTATION_CALL:"):
            parts = message.split(":")
            if len(parts) >= 3:  # Changed from == 4 to >= 3 for compatibility
                _, level_str, needed_players_str = parts[:3]
                leader_id_str = parts[3] if len(parts) > 3 else str(self.player_id)
                
                level = int(level_str)
                needed_players = int(needed_players_str)
                leader_id = int(leader_id_str)
                if level == self.level and self.mode != "JOIN":
                    inventory = self.check_inventory()
                    if inventory and inventory.get("food", 0) >= 10:
                        if (self.current_incantation_call is None or 
                            leader_id < self.current_incantation_call):
                        
                            self.current_incantation_call = leader_id
                            self.broadcast_leader_id = leader_id
                            
                            if self.debug:
                                print(f"[ZappyAI]: Received incantation call from leader {leader_id} for level {level} needing {needed_players} players")
                            
                            if leader_id == self.player_id:
                                self.is_broadcasting = True
                            else:
                                self.is_broadcasting = False
                                self.mode = "JOIN"
                                if self.debug:
                                    print(f"[ZappyAI]: Joining incantation call for level {level}")

    def move_towards_broadcast(self, direction):
        if self.debug:
            print(f"[ZappyAI]: Moving towards broadcast direction {direction}")
        time.sleep(0.5)

        if direction == 0:
            if self.debug:
                print("[ZappyAI]: Broadcast comes from current tile. No movement needed.")
            return True

        if direction == 1:
            return self.move_forward()
        elif direction == 2:
            self.move_forward()
            self.turn_left()
            self.move_forward()
            self.broadcast_direction = 8
        elif direction == 3:
            self.turn_left()
            self.move_forward()
            self.broadcast_direction = 1
        elif direction == 4:
            self.turn_left()
            self.move_forward()
            self.turn_left()
            self.move_forward()
            self.broadcast_direction = 8
        elif direction == 5:
            self.turn_right()
            self.turn_right()
            self.move_forward()
            self.broadcast_direction = 1
        elif direction == 6:
            self.turn_right()
            self.move_forward()
            self.turn_right()
            self.move_forward()
            self.broadcast_direction = 2
        elif direction == 7:
            self.turn_right()
            self.move_forward()
            self.broadcast_direction = 1
        elif direction == 8:
            self.move_forward()
            self.turn_right()
            self.move_forward()
            self.broadcast_direction = 2
        else:
            if self.debug:
                print(f"[ZappyAI]: Unknown broadcast direction: {direction}")
            return False
        
        if self.debug:
            print(f"[ZappyAI]: Moved towards broadcast direction: {direction}")
        return True

    def game_loop(self):
        while True:
            if self.mode == "JOIN":
                if self.broadcast_direction is not None:
                    self.respond_to_broadcast()
                    continue
                else:
                    self.mode = "DEFAULT"
                    self.broadcast_direction = None
                
            if self.mode == "INCANTATION_READY":
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                requirements = self.level_requirements[self.level + 1]
                for resource, needed in requirements.items():
                    if resource != "players" and needed > 0:
                        for _ in range(needed):
                            self.set_resource(resource)

                while True:
                    response = self.receive_message()
                    if response and ("Elevation underway" in response or "Current level:" in response):
                        self.level += 1
                        print(f"[ZappyAI]: Incantation complete! New level: {self.level}")
                        self.mode = "DEFAULT"
                        self.reset_incantation_call()
                        break
                    time.sleep(0.1)
                continue
            if self.current_incantation_call is not None and not self.is_broadcasting:
                if self.debug:
                    print("[ZappyAI]: Following broadcast direction...")
                self.move_towards_broadcast(self.broadcast_direction)
                continue
            inventory = self.check_inventory()
            if inventory is None:
                continue
            if inventory["food"] < 10:
                self.get40Food()
                continue
            if self.current_incantation_call is not None and self.current_incantation_call != self.player_id:
                if self.debug:
                    print(f"[ZappyAI]: Following leader {self.current_incantation_call}, moving to them")
                self.move_towards_broadcast(self.broadcast_direction)
                continue

            level_check = self.check_level_requirements()
            print(f"[ZappyAI]: Current inventory: {inventory}")
            print(f"[ZappyAI]: {level_check['message']}")

            if level_check["can_level_up"]:
                print("[ZappyAI]: Ready to level up! Preparing incantation...")
                next_level = self.level + 1
                requirements = self.level_requirements[next_level]
                needed_players = requirements["players"]
                actual_players = self.count_players_on_tile()

                if actual_players < needed_players:
                    print(f"[ZappyAI]: Not enough players on tile ({actual_players}/{needed_players})")
                    missing_players = needed_players - actual_players
                    look = self.look_around()
                    if look:
                        objects_tile = self.parse_tile_objects(look)
                        for tile_pos, objects in objects_tile.items():
                            if tile_pos != 0 and "player" in objects:
                                if self.debug:
                                    print(f"[ZappyAI]: Found player(s) on tile {tile_pos}, moving there before broadcasting")
                                self.move_to_tile(tile_pos)
                                actual_players = self.count_players_on_tile()
                                if actual_players >= needed_players:
                                    print(f"[ZappyAI]: Found enough players after moving! ({actual_players}/{needed_players})")
                                    break
                    if actual_players < needed_players:
                        look = self.look_around()
                        other_players_nearby = False
                        if look:
                            objects_tile = self.parse_tile_objects(look)
                            for tile_pos, objects in objects_tile.items():
                                if tile_pos != 0 and "player" in objects:
                                    other_players_nearby = True
                                    break
                        if self.current_incantation_call is None and not other_players_nearby:
                            if self.debug:
                                print(f"[ZappyAI]: No other players nearby, starting broadcast with ID {self.player_id}")
                            self.is_broadcasting = True
                            self.current_incantation_call = self.player_id
                            self.call_players_for_incantation_continuously(missing_players)
                        else:
                            if self.debug:
                                if self.current_incantation_call is not None:
                                    print("[ZappyAI]: Already responding to another player's call")
                                else:
                                    print("[ZappyAI]: Other players nearby, waiting for higher priority player to broadcast")
                else:
                    print(f"[ZappyAI]: Enough players on tile ({actual_players}/{needed_players})")
                    for resource, needed in requirements.items():
                        if resource == "players":
                            continue
                        if needed > 0:
                            for _ in range(needed):
                                if self.set_resource(resource):
                                    if self.debug:
                                        print(f"[ZappyAI]: Set {resource} on ground")
                                else:
                                    if self.debug:
                                        print(f"[ZappyAI]: Failed to set {resource}")
                                    break
                    if self.check_tile_requirements():
                        print("[ZappyAI]: All requirements met on tile. Starting incantation...")
                        time.sleep(1.5)
                        print("[ZappyAI]: Starting incantation now...")
                        if self.start_incantation():
                            response = self.receive_message()
                            if response:
                                print(f"[ZappyAI]: Incantation response: {response}")
                                if "Elevation underway" in response or "Current level:" in response:
                                    self.level += 1
                                    self.reset_incantation_call()
                                    print(f"[ZappyAI]: Level up successful! Now level {self.level}")
                                elif "ko" in response:
                                    print("[ZappyAI]: Incantation failed")
                                    self.reset_incantation_call()
                                else:
                                    print(f"[ZappyAI]: Unknown incantation response: {response}")
                                    self.reset_incantation_call()
                        else:
                            print("[ZappyAI]: Failed to start incantation")
                    else:
                        print("[ZappyAI]: Cannot start incantation - missing elements on tile")
            else:
                missing = level_check["missing"]
                if missing:
                    print(f"[ZappyAI]: Need to collect: {missing}")
                    for resource, quantity in missing.items():
                        if resource != "players":
                            print(f"[ZappyAI]: Collecting {quantity} {resource}...")
                            self.collect_resource(resource, quantity)
                    if "players" in missing:
                        needed_players = missing["players"]
                        print(f"[ZappyAI]: Need {needed_players} more players for incantation")
                        look = self.look_around()
                        other_players_nearby = False
                        if look:
                            objects_tile = self.parse_tile_objects(look)
                            for tile_pos, objects in objects_tile.items():
                                if tile_pos != 0 and "player" in objects:
                                    other_players_nearby = True
                                    break
                        if not other_players_nearby and self.current_incantation_call is None:
                            if self.debug:
                                print(f"[ZappyAI]: No other players nearby, broadcasting with ID {self.player_id}")
                            self.is_broadcasting = True
                            self.current_incantation_call = self.player_id
                            self.call_players_for_incantation_continuously(needed_players)
                        else:
                            if self.debug:
                                print("[ZappyAI]: Other players nearby or already responding to call, waiting")

    def collect_additional_needed_resources(self, resources_on_tile):
        inventory = self.check_inventory()

        if inventory is None:
            return
        if "food" in resources_on_tile:
            food_count_on_tile = resources_on_tile.count("food")
            for _ in range(food_count_on_tile):
                if self.take_resource("food"):
                    if self.debug:
                        print(f"[ZappyAI]: Always taking food from tile")
                else:
                    break
        for needed_resource, needed_total in self.total_requirements.items():
            if needed_resource == "players" or needed_total == 0 or needed_resource == "food":
                continue
            if needed_resource in resources_on_tile:
                current_amount = inventory.get(needed_resource, 0)
                if current_amount < needed_total:
                    resource_count_on_tile = resources_on_tile.count(needed_resource)
                    needed_amount = needed_total - current_amount
                    to_take = min(resource_count_on_tile, needed_amount)

                    if to_take > 0:
                        for _ in range(to_take):
                            if self.take_resource(needed_resource):
                                if self.debug:
                                    print(f"[ZappyAI]: Opportunistically took {needed_resource} (need {needed_total} total)")
                            else:
                                break

    def collect_resource(self, resource, target_quantity=1):
        if self.debug:
            print(f"[ZappyAI]: Starting to collect {target_quantity} {resource}")
        
        collected = 0
        while collected < target_quantity:
            inventory = self.check_inventory()
            if inventory is None:
                continue
            
            current_amount = inventory.get(resource, 0)
            if current_amount >= target_quantity:
                if self.debug:
                    print(f"[ZappyAI]: Resource target reached: {current_amount}/{target_quantity} {resource}")
                break
            
            look = self.look_around()
            if look is None:
                if random.randint(1, 2) == 1:
                    self.turn_left()
                else:
                    self.turn_right()
                self.move_forward()
                response = self.receive_message()
                continue
            
            objects_tile = self.parse_tile_objects(look)
            resource_found = False
            
            for position, ressources in objects_tile.items():
                if resource in ressources:
                    self.move_to_tile(position)
                    resource_count = ressources.count(resource)
                    resources_taken = 0
                    
                    for _ in range(resource_count):
                        if self.take_resource(resource):
                            resources_taken += 1
                            collected += 1
                            if self.debug:
                                print(f"[ZappyAI]: Took {resource} {resources_taken}/{resource_count} from tile {position}")
                            if collected >= target_quantity:
                                break
                        else:
                            break
                    if resources_taken > 0:
                        self.collect_additional_needed_resources(ressources)
                        resource_found = True
                        if self.debug:
                            print(f"[ZappyAI]: Successfully took {resources_taken} {resource} from tile {position}")
                    break
            
            if not resource_found:
                if random.randint(1, 2) == 1:
                    self.turn_left()
                else:
                    self.turn_right()
                self.move_forward()
                response = self.receive_message()
        
        if self.debug:
            print(f"[ZappyAI]: Finished collecting {resource}. Collected: {collected}")
        return collected >= target_quantity


    def get40Food(self):
        return self.collect_resource(Command.Resources.FOOD, 40)

    def send_command(self, command):
        if self.debug:
            print(f"[ZappyAI]: Sending command: {command}")
        self.client_socket.send(f"{command}\n")
    
    def parse_tile_objects(self, objects_dict):
        tile_elements = {}
        
        for objects_str, tile_indices in objects_dict.items():
            items = objects_str.strip().split()
            for tile_index in tile_indices:
                if tile_index not in tile_elements:
                    tile_elements[tile_index] = []
                
                for item in items:
                    tile_elements[tile_index].append(item)
        
        return tile_elements
    
    def move_forward(self):
        self.send_command(Command.forward())
        message = self.receive_message()
        if message is not None and message.strip() == "ok":
            return True
        else:
            if self.debug:
                print(f"[ZappyAI]: Move forward failed: {message}")
            return False
    
    def turn_right(self):
        self.send_command(Command.turn_right())
        message = self.receive_message()
        if message is not None and message.strip() == "ok":
            return True
        else:
            if self.debug:
                print(f"[ZappyAI]: Turn right failed: {message}")
            return False
    
    def turn_left(self):
        self.send_command(Command.turn_left())
        message = self.receive_message()
        if message is not None and message.strip() == "ok":
            return True
        else:
            if self.debug:
                print(f"[ZappyAI]: Turn left failed: {message}")
            return False
    
    def look_around(self):
        self.send_command(Command.look())
        message = self.receive_message()
        if message is not None and message.startswith("[") and message.endswith("]"):
            return self.extract_objects_from_look(message)
        else:
            if self.debug:
                print(f"[ZappyAI]: Look around failed: {message}")
            return None
        
    def extract_objects_from_look(self, vision_str):
        vision_str = vision_str.strip("[]")
        raw_tiles = vision_str.split(",")
        objects = {}
        for i, item in enumerate(raw_tiles):
            item = item.strip()
            if item:
                if item not in objects:
                    objects[item] = []
                objects[item].append(i)
        return objects
    
    def check_inventory(self):
        self.send_command(Command.inventory())
        message = self.receive_message()
        if message is not None and message.startswith("[ food ") and message.endswith(" ]"):
            inventory_dict = {}
            content = message.strip()[1:-1].strip()
            items = content.split(', ')
            
            for item in items:
                parts = item.strip().split(' ')
                if len(parts) == 2:
                    resource_name = parts[0]
                    quantity = int(parts[1])
                    inventory_dict[resource_name] = quantity
            return inventory_dict
        else:
            if self.debug:
                print(f"[ZappyAI]: Check inventory failed: {message}")
            return None

    def take_resource(self, resource):
        self.send_command(Command.take_object(resource))
        message = self.receive_message()
        if message is not None and message.strip() == "ok":
            return True
        else:
            if self.debug:
                print(f"[ZappyAI]: Take resource failed: {message}")
            return False
    
    def set_resource(self, resource):
        self.send_command(Command.set_object(resource))
        message = self.receive_message()
        if message is not None and message.strip() == "ok":
            return True
        else:
            if self.debug:
                print(f"[ZappyAI]: Set resource failed: {message}")
            return False
    
    def call_players_for_incantation_continuously(self, needed_players):
        if not self.is_broadcasting:
            return
        next_level = self.level + 1
        if next_level not in self.level_requirements:
            return
            
        requirements = self.level_requirements[next_level]
        
        # Set required resources first
        for resource, needed in requirements.items():
            if resource == "players" or needed == 0:
                continue
            for _ in range(needed):
                if self.set_resource(resource):
                    if self.debug:
                        print(f"[ZappyAI]: Dropped {resource} for incantation")
                else:
                    if self.debug:
                        print(f"[ZappyAI]: Failed to drop {resource}")
                    break
        
        broadcast_count = 0
        max_broadcasts = 20  # Increased for level 4 coordination
        
        while self.is_broadcasting and broadcast_count < max_broadcasts:
            inventory = self.check_inventory()
            if inventory is not None and inventory.get("food", 0) < 10:
                if self.debug:
                    print("[ZappyAI]: Not enough food to continue broadcasting.")
                break
            look = self.look_around()
            if look:
                objects_tile = self.parse_tile_objects(look)
                player_count = 0
                if 0 in objects_tile:
                    player_count = objects_tile[0].count("player")
                if self.debug:
                    print(f"[ZappyAI]: Leader sees {player_count} players on tile, needs {needed_players}")
                if player_count >= needed_players:
                    if self.debug:
                        print("[ZappyAI]: Enough players gathered, stopping broadcast")
                    self.is_broadcasting = False
                    break
            message = f"INCANTATION_CALL:{self.level}:{needed_players}:{self.player_id}"
            response = self.broadcast_message(message)
            if self.debug:
                print(f"[ZappyAI]: Broadcasting call - Response: {response}")
        self.is_broadcasting = False
        self.broadcast_leader_id = None
        self.send_command(Command.incantation())



    def reset_incantation_call(self):
        self.current_incantation_call = None
        self.is_broadcasting = False
        if self.debug:
            print("[ZappyAI]: Reset incantation call tracking")

    def call_players_for_incantation(self, needed_players):
        if self.debug:
            print(f"[ZappyAI]: Calling {needed_players} players of level {self.level} for incantation")
        print(f"PLAYER REQUIS: {needed_players}")
        for i in range(5):
            message = f"INCANTATION_CALL:{self.level}:{needed_players}"
            response = self.broadcast_message(message)
            
            if response and response.strip() == "ok":
                if self.debug:
                    print(f"[ZappyAI]: Broadcast {i+1}/5 sent successfully")
            else:
                if self.debug:
                    print(f"[ZappyAI]: Broadcast {i+1}/5 failed: {response}")
        
        if self.debug:
            print(f"[ZappyAI]: Waiting for players to join...")

    def broadcast_message(self, message):
        self.send_command(Command.broadcast(message))
        return self.receive_message()
    
    def connect_number(self):
        self.send_command(Command.connect_nbr())
    
    def fork_player(self):
        self.send_command(Command.fork())
    
    def eject_players(self):
        self.send_command(Command.eject())
    
    def count_players_on_tile(self):
        look = self.look_around()
        if look:
            objects_tile = self.parse_tile_objects(look)
            if 0 in objects_tile:
                return objects_tile[0].count("player")
        return 1

    def check_level_requirements(self):
        if self.level == 8:
            return {"can_level_up": False, "missing": {}, "message": "Already at maximum level (7)"}
        
        current_inventory = self.check_inventory()
        if current_inventory is None:
            return {"can_level_up": False, "missing": {}, "message": "Cannot check inventory"}
        
        next_level = self.level + 1
        requirements = self.level_requirements[next_level]
        missing = {}
        
        for resource, needed in requirements.items():
            if resource == "players":
                continue
            
            current_amount = current_inventory.get(resource, 0)
            if current_amount < needed:
                missing[resource] = needed - current_amount
        
        needed_players = requirements["players"]
        actual_players = self.count_players_on_tile()
        if actual_players < needed_players:
            missing["players"] = needed_players - actual_players
        
        can_level_up = len(missing) == 0
        
        if can_level_up:
            message = f"Ready to level up from {self.level} to {next_level}!"
        else:
            message = f"To level up from {self.level} to {next_level}, you need:"
            for resource, amount in missing.items():
                if resource == "players":
                    message += f" {amount} more player(s),"
                else:
                    message += f" {amount} more {resource},"
            message = message.rstrip(",")
        
        if self.debug:
            print(f"[ZappyAI]: Level check - {message}")
        
        return {
            "can_level_up": can_level_up,
            "missing": missing,
            "message": message,
            "current_level": self.level,
            "next_level": next_level,
            "requirements": requirements
        }

    def check_tile_requirements(self):
        if self.level >= 7:
            if self.debug:
                print("[ZappyAI]: Already at maximum level")
            return False
        
        next_level = self.level + 1
        requirements = self.level_requirements[next_level]
        
        look = self.look_around()
        if look is None:
            if self.debug:
                print("[ZappyAI]: Cannot look around")
            return False
        
        objects_tile = self.parse_tile_objects(look)
        current_tile_objects = objects_tile.get(0, [])
        
        if self.debug:
            print(f"[ZappyAI]: Objects on current tile: {current_tile_objects}")
        
        needed_players = requirements["players"]
        actual_players = current_tile_objects.count("player")
        
        if actual_players < needed_players:
            if self.debug:
                print(f"[ZappyAI]: Not enough players on tile: {actual_players}/{needed_players}")
            return False
        
        for resource, needed in requirements.items():
            if resource == "players":
                continue 
            
            if needed > 0:
                actual_count = current_tile_objects.count(resource)
                if actual_count < needed:
                    if self.debug:
                        print(f"[ZappyAI]: Not enough {resource} on tile: {actual_count}/{needed}")
                    return False
        
        if self.debug:
            print(f"[ZappyAI]: All requirements met on tile for level {next_level}")
        return True

    def start_incantation(self):
        if not self.check_tile_requirements():
            if self.debug:
                print("[ZappyAI]: Cannot start incantation - missing required elements on tile")
            return False
        
        self.send_command(Command.incantation())
        return True
        
    def move_to_tile(self, tile_index):
        if tile_index == None:
            if self.debug:
                print("[ZappyAI]: Tile index is None, cannot move to tile.")
            return False
        if tile_index == 0:
            return True

        level = 0
        count = 0
        while count + (2 * level + 1) <= tile_index:
            count += (2 * level + 1)
            level += 1
        pos_in_row = tile_index - count
        row_length = 2 * level + 1
        center = row_length // 2
        for _ in range(level):
            if not self.move_forward():
                return False
        lateral = pos_in_row - center
        if lateral < 0:
            if not self.turn_left():
                return False
            for _ in range(abs(lateral)):
                if not self.move_forward():
                    return False
            if not self.turn_right():
                return False
        elif lateral > 0:
            if not self.turn_right():
                return False
            for _ in range(abs(lateral)):
                if not self.move_forward():
                    return False
            if not self.turn_left():
                return False
        
        return True
