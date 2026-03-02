class Area {
  final String id;
  final String name;
  final String actionService;
  final String actionName;
  final String actionDescription;
  final String reactionService;
  final String reactionName;
  final String reactionDescription;
  bool isActive;

  Area({
    required this.id,
    required this.name,
    required this.actionService,
    required this.actionName,
    required this.actionDescription,
    required this.reactionService,
    required this.reactionName,
    required this.reactionDescription,
    this.isActive = true,
  });

  factory Area.fromJson(Map<String, dynamic> json) {
    return Area(
      id: json['id']?.toString() ?? '',
      name: json['name'] ?? 'AREA',
      actionService: json['action_service'] ?? '',
      actionName: json['action_name'] ?? '',
      actionDescription: json['action_description'] ?? '',
      reactionService: json['reaction_service'] ?? '',
      reactionName: json['reaction_name'] ?? '',
      reactionDescription: json['reaction_description'] ?? '',
      isActive: json['enabled'] ?? true,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'id': id,
      'name': name,
      'actionService': actionService,
      'actionName': actionName,
      'actionDescription': actionDescription,
      'reactionService': reactionService,
      'reactionName': reactionName,
      'reactionDescription': reactionDescription,
      'isActive': isActive,
    };
  }

  Area copyWith({bool? isActive}) {
    return Area(
      id: id,
      name: name,
      actionService: actionService,
      actionName: actionName,
      actionDescription: actionDescription,
      reactionService: reactionService,
      reactionName: reactionName,
      reactionDescription: reactionDescription,
      isActive: isActive ?? this.isActive,
    );
  }
}
