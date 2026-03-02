/// Helper to map action/reaction names to their IDs
/// Based on the current database structure
class ActionReactionMapper {
  // Map of action names to their IDs
  static final Map<String, int> _actionNameToId = {
    // Dropbox actions (service_id: 1)
    'new_file_in_folder': 1,
    'file_modified': 2,
    // Google actions (service_id: 2)
    'new_email_received': 4,
    'calendar_event_starts': 5,
    // Timer actions (service_id: 3)
    'specific_time': 6,
    'specific_date': 7,
    'days_until_event': 8,
    'every_hour': 9,
    'every_day': 10,
    'every_week': 11,
  };

  // Map of reaction names to their IDs
  static final Map<String, int> _reactionNameToId = {
    // Dropbox reactions (service_id: 1)
    'upload_file': 1,
    'create_folder': 4,
    'move_file': 7,
    'copy_file': 8,
    'delete_file': 9,
    'share_file': 10,
    // Google reactions (service_id: 2)
    'send_email': 2,
    'create_calendar_event': 3,
    // Timer reactions (service_id: 3)
    'send_notification': 5,
    'log_event': 6,
  };

  /// Get action ID from action name
  static int? getActionId(String actionName) {
    return _actionNameToId[actionName];
  }

  /// Get reaction ID from reaction name
  static int? getReactionId(String reactionName) {
    return _reactionNameToId[reactionName];
  }

  /// Add ID field to action map
  static Map<String, dynamic> enrichAction(Map<String, dynamic> action) {
    final enriched = Map<String, dynamic>.from(action);
    final id = getActionId(action['name']);
    if (id != null) {
      enriched['id'] = id;
    }
    return enriched;
  }

  /// Add ID field to reaction map
  static Map<String, dynamic> enrichReaction(Map<String, dynamic> reaction) {
    final enriched = Map<String, dynamic>.from(reaction);
    final id = getReactionId(reaction['name']);
    if (id != null) {
      enriched['id'] = id;
    }
    return enriched;
  }

  /// Enrich all services with IDs
  static List<Map<String, dynamic>> enrichServices(List<dynamic> services) {
    return services.map((service) {
      final enrichedService = Map<String, dynamic>.from(service);

      if (enrichedService['actions'] != null) {
        enrichedService['actions'] = (enrichedService['actions'] as List)
            .map((action) => enrichAction(Map<String, dynamic>.from(action)))
            .toList();
      }

      if (enrichedService['reactions'] != null) {
        enrichedService['reactions'] = (enrichedService['reactions'] as List)
            .map((reaction) =>
                enrichReaction(Map<String, dynamic>.from(reaction)))
            .toList();
      }

      return enrichedService;
    }).toList();
  }
}
