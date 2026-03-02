import json
from itertools import cycle
from collections import Counter

RESOURCES = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

LEVEL_REQUIREMENTS = {
    1: {"linemate": 1},
    2: {"linemate": 1, "deraumere": 1, "sibur": 1},
    3: {"linemate": 2, "sibur": 1, "phiras": 2},
    4: {"linemate": 1, "deraumere": 1, "sibur": 2, "phiras": 1},
    5: {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3},
    6: {"linemate": 1, "deraumere": 2, "sibur": 3, "phiras": 1},
    7: {"linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1},
}

def xor(s1, s2):
    return ''.join(chr(ord(c) ^ ord(k)) for c, k in zip(s2, cycle(s1)))

def parse_inventory(response):
    response = response.strip("[] \n")
    items = response.split(",")
    inventory = {res: 0 for res in RESOURCES}
    for item in items:
        parts = item.strip().split()
        if len(parts) != 2:
            continue
        key, value = parts
        try:
            inventory[key] = int(value)
        except ValueError:
            continue
    return inventory

def update_shared_inventory(client_id, inventory, shared_inventory):
    shared_inventory[client_id] = inventory
    c = Counter()
    for k, v in shared_inventory.items():
        if k != "total":
            c.update(v)
    shared_inventory["total"] = dict(c)

def check_can_incant(level, shared_inventory):
    required = LEVEL_REQUIREMENTS.get(level, {})
    inventory = shared_inventory.get("total", {}).copy()
    for item in required:
        if inventory.get(item, 0) < required[item]:
            return False
    return True
