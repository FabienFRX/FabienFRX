##
## EPITECH PROJECT, 2025
## ZAPPY_IA
## File description:
## Command
##

class Command:
    @staticmethod
    def forward():
        return "Forward"
    
    @staticmethod
    def turn_right():
        return "Right"
    
    @staticmethod
    def turn_left():
        return "Left"

    @staticmethod
    def look():
        return "Look"
    
    @staticmethod
    def inventory():
        return "Inventory"
    
    @staticmethod
    def take_object(object_name):
        return f"Take {object_name}"
    
    @staticmethod
    def set_object(object_name):
        return f"Set {object_name}"
    
    @staticmethod
    def broadcast(message):
        return f"Broadcast {message}"
    
    @staticmethod
    def connect_nbr():
        return "Connect_nbr"
    
    @staticmethod
    def fork():
        return "Fork"
    
    @staticmethod
    def eject():
        return "Eject"
    
    @staticmethod
    def incantation():
        return "Incantation"
    
    class Resources:
        FOOD = "food"
        LINEMATE = "linemate"
        DERAUMERE = "deraumere"
        SIBUR = "sibur"
        MENDIANE = "mendiane"
        PHIRAS = "phiras"
        THYSTAME = "thystame"
    
    @staticmethod
    def take_food():
        return Command.take_object(Command.Resources.FOOD)
    
    @staticmethod
    def take_linemate():
        return Command.take_object(Command.Resources.LINEMATE)
    
    @staticmethod
    def take_deraumere():
        return Command.take_object(Command.Resources.DERAUMERE)
    
    @staticmethod
    def take_sibur():
        return Command.take_object(Command.Resources.SIBUR)
    
    @staticmethod
    def take_mendiane():
        return Command.take_object(Command.Resources.MENDIANE)
    
    @staticmethod
    def take_phiras():
        return Command.take_object(Command.Resources.PHIRAS)
    
    @staticmethod
    def take_thystame():
        return Command.take_object(Command.Resources.THYSTAME)
    
    @staticmethod
    def set_food():
        return Command.set_object(Command.Resources.FOOD)
    
    @staticmethod
    def set_linemate():
        return Command.set_object(Command.Resources.LINEMATE)
    
    @staticmethod
    def set_deraumere():
        return Command.set_object(Command.Resources.DERAUMERE)
    
    @staticmethod
    def set_sibur():
        return Command.set_object(Command.Resources.SIBUR)
    
    @staticmethod
    def set_mendiane():
        return Command.set_object(Command.Resources.MENDIANE)
    
    @staticmethod
    def set_phiras():
        return Command.set_object(Command.Resources.PHIRAS)
    
    @staticmethod
    def set_thystame():
        return Command.set_object(Command.Resources.THYSTAME)
    
    @staticmethod
    def get_all_commands():
        return [
            "Forward", "Right", "Left", "Look", "Inventory",
            "Take", "Set", "Broadcast", "Connect_nbr",
            "Fork", "Eject", "Incantation"
        ]
    
    @staticmethod
    def get_all_resources():
        return [
            Command.Resources.FOOD, Command.Resources.LINEMATE,
            Command.Resources.DERAUMERE, Command.Resources.SIBUR,
            Command.Resources.MENDIANE, Command.Resources.PHIRAS,
            Command.Resources.THYSTAME
        ]
