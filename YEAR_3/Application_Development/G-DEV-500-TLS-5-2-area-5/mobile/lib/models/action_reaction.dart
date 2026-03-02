class Action {
  final int id;
  final String name;
  final String description;
  final String serviceName;

  Action({
    required this.id,
    required this.name,
    required this.description,
    required this.serviceName,
  });

  factory Action.fromJson(Map<String, dynamic> json, String serviceName) {
    return Action(
      id: json['id'],
      name: json['name'],
      description: json['description'],
      serviceName: serviceName,
    );
  }
}

class Reaction {
  final int id;
  final String name;
  final String description;
  final String serviceName;

  Reaction({
    required this.id,
    required this.name,
    required this.description,
    required this.serviceName,
  });

  factory Reaction.fromJson(Map<String, dynamic> json, String serviceName) {
    return Reaction(
      id: json['id'],
      name: json['name'],
      description: json['description'],
      serviceName: serviceName,
    );
  }
}
