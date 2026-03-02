import random
import re
import math
import json
import time

RESOURCES = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

LEVEL_REQUIREMENTS = {
    1: {"linemate": 1, "players": 1},
    2: {"linemate": 1, "deraumere": 1, "sibur": 1, "players": 2},
    3: {"linemate": 2, "sibur": 1, "phiras": 2, "players": 2},
    4: {"linemate": 1, "deraumere": 1, "sibur": 2, "phiras": 1, "players": 4},
    5: {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "players": 4},
    6: {"linemate": 1, "deraumere": 2, "sibur": 3, "phiras": 1, "players": 6},
    7: {"linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1, "players": 6},
}

class AutonomousAI:
    def __init__(self, team, port=4242, host="127.0.0.1"):
        self.team = team
        self.port = port
        self.host = host
        
        self.inventory = {res: 0 for res in RESOURCES}
        self.level = 1
        self.step = -1
        self.commands_list = []
        
        self.look_data = ""
        self.last_look_parsed = []
        
        self.first_inventory_received = False
        self.client_num = 0
        self.slots_available = 0
        
        self.current_target = "food"
        
        self.waiting_for_response = False
        self.last_command_time = 0
        self.command_cooldown = 0.1

    def parse_inventory(self, data):
        try:
            clean_data = data.replace("[", "").replace("]", "")
            items = clean_data.split(",")
            
            new_inventory = {res: 0 for res in RESOURCES}
            
            for item in items:
                item = item.strip()
                if item:
                    parts = item.split()
                    if len(parts) >= 2:
                        resource = parts[0]
                        amount = int(parts[1])
                        if resource in RESOURCES:
                            new_inventory[resource] = amount
            
            self.inventory = new_inventory
            self.first_inventory_received = True
            return True
            
        except Exception as e:
            return False

    def parse_look(self, data, target_object):
        try:
            clean_data = data.replace("[", "").replace("]", "")
            tiles = clean_data.split(",")
            
            tile_contents = []
            for tile in tiles:
                tile = tile.strip()
                if tile:
                    objects = tile.split()
                    tile_contents.append(objects)
                else:
                    tile_contents.append([])
            
            self.last_look_parsed = tile_contents
            
            if len(tile_contents) > 0 and target_object in tile_contents[0]:
                return [f"Take {target_object}"]
            
            for i, tile in enumerate(tile_contents[1:], 1):
                if target_object in tile:
                    moves = ["Forward"] * i
                    moves.append(f"Take {target_object}")
                    return moves
            
            return [random.choice(["Forward", "Right", "Left"])]
            
        except Exception as e:
            return ["Look"]

    def get_priority_resource(self):
        current_food = self.inventory.get("food", 0)
        
        if current_food < 20:
            return "food"
        
        if self.level in LEVEL_REQUIREMENTS:
            required = LEVEL_REQUIREMENTS[self.level]
            for resource, needed in required.items():
                if resource == "players":
                    continue
                if self.inventory.get(resource, 0) < needed:
                    return resource
        
        return "food"

    def next_action(self, last_response):
        current_time = time.time()
        
        if last_response:
            self.handle_response(last_response)
        
        if current_time - self.last_command_time < self.command_cooldown:
            return None
        
        if self.commands_list:
            command = self.commands_list.pop(0)
            self.last_command_time = current_time
            return command
        
        if self.step == -1:
            self.step = 0
            return "Inventory"
            
        elif self.step == 0:
            if not self.first_inventory_received:
                return "Inventory"
            
            self.step = 1
            return "Connect_nbr"
            
        elif self.step == 1:
            self.step = 2
            return "Look"
            
        elif self.step == 2:
            priority = self.get_priority_resource()
            self.current_target = priority
            
            if self.look_data:
                self.commands_list = self.parse_look(self.look_data, priority)
                self.commands_list.append("Inventory")
            else:
                self.commands_list = ["Look"]
            
            self.step = 1
            
            if self.commands_list:
                return self.commands_list.pop(0)
            
        return "Look"

    def handle_response(self, response):
        if "[" in response and "]" in response:
            if "food" in response and any(resource in response for resource in RESOURCES[1:]):
                self.parse_inventory(response)
            else:
                self.look_data = response
        elif "ok" in response.lower():
            pass
        elif "ko" in response.lower():
            pass
        elif "dead" in response.lower():
            return False
        elif "Current level:" in response:
            try:
                new_level = int(response.split(":")[1].strip())
                if new_level > self.level:
                    self.level = new_level
            except:
                pass
        
        return True

    def can_evolve(self):
        if self.level not in LEVEL_REQUIREMENTS:
            return False
        
        required = LEVEL_REQUIREMENTS[self.level]
        for resource, needed in required.items():
            if resource == "players":
                continue
            if self.inventory.get(resource, 0) < needed:
                return False
        
        return True