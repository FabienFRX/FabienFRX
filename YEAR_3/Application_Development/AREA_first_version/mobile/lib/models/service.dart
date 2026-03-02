class Service {
  final String name;
  final String icon;
  final List<ServiceAction> actions;
  final List<ServiceReaction> reactions;

  Service({
    required this.name,
    required this.icon,
    required this.actions,
    required this.reactions,
  });

  factory Service.fromJson(Map<String, dynamic> json) {
    return Service(
      name: json['name'] ?? '',
      icon: json['icon'] ?? '📦',
      actions: (json['actions'] as List?)
              ?.map((a) => ServiceAction.fromJson(a))
              .toList() ??
          [],
      reactions: (json['reactions'] as List?)
              ?.map((r) => ServiceReaction.fromJson(r))
              .toList() ??
          [],
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'name': name,
      'icon': icon,
      'actions': actions.map((a) => a.toJson()).toList(),
      'reactions': reactions.map((r) => r.toJson()).toList(),
    };
  }
}

class ServiceAction {
  final String name;
  final String description;

  ServiceAction({
    required this.name,
    required this.description,
  });

  factory ServiceAction.fromJson(Map<String, dynamic> json) {
    return ServiceAction(
      name: json['name'] ?? '',
      description: json['description'] ?? '',
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'name': name,
      'description': description,
    };
  }
}

class ServiceReaction {
  final String name;
  final String description;

  ServiceReaction({
    required this.name,
    required this.description,
  });

  factory ServiceReaction.fromJson(Map<String, dynamic> json) {
    return ServiceReaction(
      name: json['name'] ?? '',
      description: json['description'] ?? '',
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'name': name,
      'description': description,
    };
  }
}
