import 'package:flutter/material.dart';
import 'dart:convert';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Automation Platform - PoC Flutter',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        useMaterial3: true,
      ),
      home: const HomePage(),
    );
  }
}

// Services data
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
}

class ServiceAction {
  final String id;
  final String label;
  final String description;

  ServiceAction({
    required this.id,
    required this.label,
    required this.description,
  });
}

class ServiceReaction {
  final String id;
  final String label;
  final String description;

  ServiceReaction({
    required this.id,
    required this.label,
    required this.description,
  });
}

final Map<String, Service> availableServices = {
  'gmail': Service(
    name: 'Gmail',
    icon: '📧',
    actions: [
      ServiceAction(
        id: 'new_email',
        label: 'Nouvel email reçu',
        description: 'Déclenché quand un nouvel email arrive',
      ),
      ServiceAction(
        id: 'email_with_attachment',
        label: 'Email avec pièce jointe',
        description: 'Déclenché quand un email avec pièce jointe arrive',
      ),
    ],
    reactions: [],
  ),
  'onedrive': Service(
    name: 'OneDrive',
    icon: '☁️',
    actions: [],
    reactions: [
      ServiceReaction(
        id: 'save_file',
        label: 'Sauvegarder un fichier',
        description: 'Sauvegarde un fichier dans OneDrive',
      ),
      ServiceReaction(
        id: 'create_folder',
        label: 'Créer un dossier',
        description: 'Crée un nouveau dossier',
      ),
    ],
  ),
  'github': Service(
    name: 'GitHub',
    icon: '🐙',
    actions: [
      ServiceAction(
        id: 'new_pr',
        label: 'Nouvelle Pull Request',
        description: 'Déclenché quand une nouvelle PR est créée',
      ),
      ServiceAction(
        id: 'new_issue',
        label: 'Nouvelle Issue',
        description: 'Déclenché quand une nouvelle issue est ouverte',
      ),
    ],
    reactions: [
      ServiceReaction(
        id: 'create_issue',
        label: 'Créer une Issue',
        description: 'Crée une nouvelle issue dans un repository',
      ),
      ServiceReaction(
        id: 'star_repo',
        label: 'Starrer un repository',
        description: 'Star un repository spécifié',
      ),
    ],
  ),
  'twitter': Service(
    name: 'Twitter',
    icon: '🐦',
    actions: [
      ServiceAction(
        id: 'new_tweet',
        label: 'Nouveau tweet',
        description: 'Déclenché quand un nouveau tweet est posté',
      ),
      ServiceAction(
        id: 'new_follower',
        label: 'Nouveau follower',
        description: 'Déclenché quand vous gagnez un follower',
      ),
    ],
    reactions: [
      ServiceReaction(
        id: 'post_tweet',
        label: 'Poster un tweet',
        description: 'Poste un nouveau tweet',
      ),
      ServiceReaction(
        id: 'like_tweet',
        label: 'Liker un tweet',
        description: 'Like un tweet spécifié',
      ),
    ],
  ),
};

// Area model
class Area {
  final int id;
  final String actionService;
  final ServiceAction action;
  final ServiceReaction reaction;
  bool active;

  Area({
    required this.id,
    required this.actionService,
    required this.action,
    required this.reaction,
    this.active = true,
  });
}

class HomePage extends StatelessWidget {
  const HomePage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Automation Platform - PoC Flutter'),
        centerTitle: true,
        backgroundColor: Colors.blue,
        foregroundColor: Colors.white,
      ),
      body: Center(
        child: Padding(
          padding: const EdgeInsets.all(24.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              const Text(
                '⚡ AREA Builder',
                style: TextStyle(fontSize: 40, fontWeight: FontWeight.bold),
              ),
              const SizedBox(height: 30),
              const Card(
                elevation: 4,
                child: Padding(
                  padding: EdgeInsets.all(20.0),
                  child: Column(
                    children: [
                      Text(
                        'Automatisez vos tâches',
                        style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                      ),
                      SizedBox(height: 10),
                      Text(
                        'Créez des automatisations (AREA) en connectant vos services préférés : SI une action se produit, ALORS déclenchez une réaction.',
                        textAlign: TextAlign.center,
                        style: TextStyle(fontSize: 14),
                      ),
                    ],
                  ),
                ),
              ),
              const SizedBox(height: 40),
              ElevatedButton.icon(
                onPressed: () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => const AREABuilderPage()),
                  );
                },
                icon: const Icon(Icons.add_circle),
                label: const Text('Créer une AREA'),
                style: ElevatedButton.styleFrom(
                  padding: const EdgeInsets.symmetric(horizontal: 40, vertical: 16),
                  textStyle: const TextStyle(fontSize: 16),
                ),
              ),
              const SizedBox(height: 16),
              OutlinedButton.icon(
                onPressed: () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => const MyAreasPage()),
                  );
                },
                icon: const Icon(Icons.list),
                label: const Text('Mes AREAs'),
                style: OutlinedButton.styleFrom(
                  padding: const EdgeInsets.symmetric(horizontal: 50, vertical: 16),
                  textStyle: const TextStyle(fontSize: 16),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

// AREA Builder Page
class AREABuilderPage extends StatefulWidget {
  const AREABuilderPage({super.key});

  @override
  State<AREABuilderPage> createState() => _AREABuilderPageState();
}

class _AREABuilderPageState extends State<AREABuilderPage> {
  String? selectedActionService;
  ServiceAction? selectedAction;
  ServiceReaction? selectedReaction;
  List<ServiceReaction> availableReactions = [];

  List<MapEntry<String, ServiceAction>> getAllActions() {
    List<MapEntry<String, ServiceAction>> actions = [];
    availableServices.forEach((serviceKey, service) {
      for (var action in service.actions) {
        actions.add(MapEntry(serviceKey, action));
      }
    });
    return actions;
  }

  void updateAvailableReactions() {
    if (selectedAction != null) {
      List<ServiceReaction> reactions = [];
      availableServices.forEach((_, service) {
        reactions.addAll(service.reactions);
      });
      setState(() {
        availableReactions = reactions;
      });
    }
  }

  void createArea() {
    if (selectedAction == null || selectedReaction == null || selectedActionService == null) {
      return;
    }

    final newArea = Area(
      id: DateTime.now().millisecondsSinceEpoch,
      actionService: selectedActionService!,
      action: selectedAction!,
      reaction: selectedReaction!,
      active: true,
    );

    // Navigate to MyAreasPage and pass the new area
    Navigator.pushReplacement(
      context,
      MaterialPageRoute(
        builder: (context) => MyAreasPage(newArea: newArea),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final allActions = getAllActions();

    return Scaffold(
      appBar: AppBar(
        title: const Text('Créer une AREA'),
        backgroundColor: Colors.blue,
        foregroundColor: Colors.white,
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: ListView(
          children: [
            Card(
              elevation: 4,
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'Configuration de l\'automation',
                      style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                    ),
                    const SizedBox(height: 20),
                    const Text(
                      'QUAND (Action) :',
                      style: TextStyle(fontWeight: FontWeight.bold),
                    ),
                    const SizedBox(height: 8),
                    DropdownButtonFormField<String>(
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        hintText: 'Choisir une action...',
                      ),
                      items: allActions.map((entry) {
                        final serviceKey = entry.key;
                        final action = entry.value;
                        final service = availableServices[serviceKey]!;
                        return DropdownMenuItem(
                          value: '$serviceKey:${action.id}',
                          child: Text('${service.icon} ${service.name} - ${action.label}'),
                        );
                      }).toList(),
                      onChanged: (value) {
                        if (value != null) {
                          final parts = value.split(':');
                          final serviceKey = parts[0];
                          final actionId = parts[1];
                          final service = availableServices[serviceKey]!;
                          final action = service.actions.firstWhere((a) => a.id == actionId);
                          setState(() {
                            selectedActionService = serviceKey;
                            selectedAction = action;
                            selectedReaction = null;
                          });
                          updateAvailableReactions();
                        }
                      },
                    ),
                    const SizedBox(height: 20),
                    const Text(
                      'ALORS (REAction) :',
                      style: TextStyle(fontWeight: FontWeight.bold),
                    ),
                    const SizedBox(height: 8),
                    DropdownButtonFormField<String>(
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        hintText: 'Choisir une réaction...',
                      ),
                      items: availableReactions.map((reaction) {
                        return DropdownMenuItem(
                          value: reaction.id,
                          child: Text(reaction.label),
                        );
                      }).toList(),
                      onChanged: selectedAction == null
                          ? null
                          : (value) {
                              if (value != null) {
                                final reaction = availableReactions.firstWhere((r) => r.id == value);
                                setState(() {
                                  selectedReaction = reaction;
                                });
                              }
                            },
                    ),
                    const SizedBox(height: 30),
                    SizedBox(
                      width: double.infinity,
                      child: ElevatedButton.icon(
                        onPressed: (selectedAction != null && selectedReaction != null)
                            ? createArea
                            : null,
                        icon: const Icon(Icons.check_circle),
                        label: const Text('Créer l\'AREA'),
                        style: ElevatedButton.styleFrom(
                          padding: const EdgeInsets.symmetric(vertical: 16),
                        ),
                      ),
                    ),
                  ],
                ),
              ),
            ),
            if (selectedAction != null && selectedReaction != null)
              Card(
                margin: const EdgeInsets.only(top: 20),
                color: Colors.blue.shade50,
                child: Padding(
                  padding: const EdgeInsets.all(16.0),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      const Text(
                        'Aperçu',
                        style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                      ),
                      const SizedBox(height: 10),
                      Text('QUAND: ${selectedAction!.label}'),
                      const SizedBox(height: 5),
                      Text('ALORS: ${selectedReaction!.label}'),
                    ],
                  ),
                ),
              ),
          ],
        ),
      ),
    );
  }
}

// My Areas Page
class MyAreasPage extends StatefulWidget {
  final Area? newArea;

  const MyAreasPage({super.key, this.newArea});

  @override
  State<MyAreasPage> createState() => _MyAreasPageState();
}

class _MyAreasPageState extends State<MyAreasPage> {
  List<Area> userAreas = [];

  @override
  void initState() {
    super.initState();
    if (widget.newArea != null) {
      userAreas.add(widget.newArea!);
    }
  }

  void toggleArea(int areaId) {
    setState(() {
      final index = userAreas.indexWhere((area) => area.id == areaId);
      if (index != -1) {
        userAreas[index].active = !userAreas[index].active;
      }
    });
  }

  void deleteArea(int areaId) {
    setState(() {
      userAreas.removeWhere((area) => area.id == areaId);
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Mes AREAs (${userAreas.length})'),
        backgroundColor: Colors.blue,
        foregroundColor: Colors.white,
        actions: [
          IconButton(
            icon: const Icon(Icons.add_circle),
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const AREABuilderPage()),
              );
            },
          ),
        ],
      ),
      body: userAreas.isEmpty
          ? Center(
              child: Padding(
                padding: const EdgeInsets.all(32.0),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    const Text(
                      'Aucune AREA créée.\nCommencez par en créer une !',
                      textAlign: TextAlign.center,
                      style: TextStyle(fontSize: 18),
                    ),
                    const SizedBox(height: 30),
                    ElevatedButton.icon(
                      onPressed: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(builder: (context) => const AREABuilderPage()),
                        );
                      },
                      icon: const Icon(Icons.add_circle),
                      label: const Text('Créer une AREA'),
                    ),
                  ],
                ),
              ),
            )
          : ListView.builder(
              padding: const EdgeInsets.all(16),
              itemCount: userAreas.length,
              itemBuilder: (context, index) {
                final area = userAreas[index];
                final service = availableServices[area.actionService]!;

                return Card(
                  margin: const EdgeInsets.only(bottom: 16),
                  elevation: 4,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(12),
                    side: BorderSide(
                      color: area.active ? Colors.green : Colors.grey,
                      width: 2,
                    ),
                  ),
                  child: Padding(
                    padding: const EdgeInsets.all(16.0),
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Row(
                          mainAxisAlignment: MainAxisAlignment.spaceBetween,
                          children: [
                            Text(
                              'AREA #${area.id}',
                              style: const TextStyle(
                                fontSize: 18,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                            Chip(
                              label: Text(area.active ? 'Active' : 'Inactive'),
                              backgroundColor: area.active ? Colors.green.shade100 : Colors.grey.shade300,
                            ),
                          ],
                        ),
                        const Divider(),
                        const SizedBox(height: 8),
                        Text(
                          'QUAND:',
                          style: TextStyle(
                            fontWeight: FontWeight.bold,
                            color: Colors.grey.shade700,
                          ),
                        ),
                        Text(area.action.label),
                        const SizedBox(height: 12),
                        Text(
                          'ALORS:',
                          style: TextStyle(
                            fontWeight: FontWeight.bold,
                            color: Colors.grey.shade700,
                          ),
                        ),
                        Text(area.reaction.label),
                        const SizedBox(height: 16),
                        Row(
                          children: [
                            Expanded(
                              child: OutlinedButton.icon(
                                onPressed: () => toggleArea(area.id),
                                icon: Icon(area.active ? Icons.power_off : Icons.power),
                                label: Text(area.active ? 'Désactiver' : 'Activer'),
                                style: OutlinedButton.styleFrom(
                                  foregroundColor: area.active ? Colors.grey : Colors.green,
                                ),
                              ),
                            ),
                            const SizedBox(width: 10),
                            Expanded(
                              child: OutlinedButton.icon(
                                onPressed: () => deleteArea(area.id),
                                icon: const Icon(Icons.delete),
                                label: const Text('Supprimer'),
                                style: OutlinedButton.styleFrom(
                                  foregroundColor: Colors.red,
                                ),
                              ),
                            ),
                          ],
                        ),
                      ],
                    ),
                  ),
                );
              },
            ),
    );
  }
}
