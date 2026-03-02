import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/auth_provider.dart';
import '../providers/server_provider.dart';
import '../providers/area_provider.dart';
import '../services/api_service.dart';
import '../models/area.dart';

class MyAreasScreen extends StatefulWidget {
  const MyAreasScreen({super.key});

  @override
  State<MyAreasScreen> createState() => _MyAreasScreenState();
}

class _MyAreasScreenState extends State<MyAreasScreen> {
  bool _isLoading = false;

  @override
  void initState() {
    super.initState();
    _loadAreas();
  }

  Future<void> _loadAreas() async {
    setState(() => _isLoading = true);

    try {
      final serverProvider =
          Provider.of<ServerProvider>(context, listen: false);
      final authProvider = Provider.of<AuthProvider>(context, listen: false);
      final areaProvider = Provider.of<AreaProvider>(context, listen: false);
      final apiService = ApiService(serverProvider.serverUrl);

      final response = await apiService.getAreas(authProvider.token!);
      final areasData = response['areas'] as List;
      final areas = areasData.map((data) => Area.fromJson(data)).toList();
      areaProvider.setAreas(areas);
    } catch (e) {
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Erreur: ${e.toString()}')),
      );
    } finally {
      if (mounted) {
        setState(() => _isLoading = false);
      }
    }
  }

  Future<void> _toggleArea(String id) async {
    try {
      final serverProvider =
          Provider.of<ServerProvider>(context, listen: false);
      final authProvider = Provider.of<AuthProvider>(context, listen: false);
      final areaProvider = Provider.of<AreaProvider>(context, listen: false);
      final apiService = ApiService(serverProvider.serverUrl);

      // Récupère l'AREA actuelle pour inverser son état
      final area = areaProvider.areas.firstWhere((a) => a.id == id);
      final newEnabled = !area.isActive;

      await apiService.updateArea(
          authProvider.token!, int.parse(id), {'enabled': newEnabled});
      areaProvider.toggleArea(id);

      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('AREA mise à jour')),
      );
    } catch (e) {
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Erreur: ${e.toString()}')),
      );
    }
  }

  Future<void> _deleteArea(String id) async {
    final confirm = await showDialog<bool>(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Confirmer la suppression'),
        content: const Text('Voulez-vous vraiment supprimer cette AREA ?'),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context, false),
            child: const Text('Annuler'),
          ),
          TextButton(
            onPressed: () => Navigator.pop(context, true),
            style: TextButton.styleFrom(foregroundColor: Colors.red),
            child: const Text('Supprimer'),
          ),
        ],
      ),
    );

    if (confirm != true || !mounted) return;

    final serverProvider = Provider.of<ServerProvider>(context, listen: false);
    final authProvider = Provider.of<AuthProvider>(context, listen: false);
    final areaProvider = Provider.of<AreaProvider>(context, listen: false);
    final apiService = ApiService(serverProvider.serverUrl);

    try {
      await apiService.deleteArea(authProvider.token!, int.parse(id));
      areaProvider.removeArea(id);

      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('AREA supprimée')),
      );
    } catch (e) {
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Erreur: ${e.toString()}')),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    final areaProvider = Provider.of<AreaProvider>(context);
    final areas = areaProvider.areas;

    return Scaffold(
      appBar: AppBar(
        title: const Text('Mes AREAs'),
        actions: [
          IconButton(
            icon: const Icon(Icons.refresh),
            onPressed: _loadAreas,
          ),
        ],
      ),
      body: _isLoading
          ? const Center(child: CircularProgressIndicator())
          : areas.isEmpty
              ? Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      const Icon(Icons.inbox, size: 64, color: Colors.grey),
                      const SizedBox(height: 16),
                      const Text('Aucune AREA créée'),
                      const SizedBox(height: 16),
                      ElevatedButton.icon(
                        onPressed: () =>
                            Navigator.pushNamed(context, '/area-builder'),
                        icon: const Icon(Icons.add),
                        label: const Text('Créer une AREA'),
                      ),
                    ],
                  ),
                )
              : ListView.builder(
                  padding: const EdgeInsets.all(16),
                  itemCount: areas.length,
                  itemBuilder: (context, index) {
                    final area = areas[index];
                    return Card(
                      margin: const EdgeInsets.only(bottom: 16),
                      child: Padding(
                        padding: const EdgeInsets.all(16),
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Row(
                              children: [
                                Expanded(
                                  child: Column(
                                    crossAxisAlignment:
                                        CrossAxisAlignment.start,
                                    children: [
                                      Text(
                                        area.name,
                                        style: Theme.of(context)
                                            .textTheme
                                            .titleMedium
                                            ?.copyWith(
                                              fontWeight: FontWeight.bold,
                                            ),
                                      ),
                                      const SizedBox(height: 4),
                                      Text(
                                        '${area.actionService} → ${area.reactionService}',
                                        style: const TextStyle(
                                          fontSize: 12,
                                          color: Colors.grey,
                                        ),
                                      ),
                                    ],
                                  ),
                                ),
                                Chip(
                                  label: Text(
                                    area.isActive ? 'Actif' : 'Inactif',
                                    style: TextStyle(
                                      color: area.isActive
                                          ? Colors.green
                                          : Colors.grey,
                                      fontWeight: FontWeight.bold,
                                    ),
                                  ),
                                  backgroundColor: area.isActive
                                      ? Colors.green.withOpacity(0.1)
                                      : Colors.grey.withOpacity(0.1),
                                ),
                              ],
                            ),
                            const SizedBox(height: 12),
                            Row(
                              children: [
                                const Icon(Icons.input, size: 18),
                                const SizedBox(width: 8),
                                Expanded(
                                  child: Text(
                                    'SI: ${area.actionName}',
                                    style: const TextStyle(fontSize: 14),
                                  ),
                                ),
                              ],
                            ),
                            const SizedBox(height: 8),
                            Row(
                              children: [
                                const Icon(Icons.output, size: 18),
                                const SizedBox(width: 8),
                                Expanded(
                                  child: Text(
                                    'ALORS: ${area.reactionName}',
                                    style: const TextStyle(fontSize: 14),
                                  ),
                                ),
                              ],
                            ),
                            const SizedBox(height: 12),
                            Row(
                              mainAxisAlignment: MainAxisAlignment.end,
                              children: [
                                TextButton.icon(
                                  onPressed: () => _toggleArea(area.id),
                                  icon: Icon(
                                    area.isActive
                                        ? Icons.pause
                                        : Icons.play_arrow,
                                  ),
                                  label: Text(
                                    area.isActive ? 'Désactiver' : 'Activer',
                                  ),
                                ),
                                TextButton.icon(
                                  onPressed: () => _deleteArea(area.id),
                                  icon: const Icon(Icons.delete),
                                  label: const Text('Supprimer'),
                                  style: TextButton.styleFrom(
                                    foregroundColor: Colors.red,
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
      floatingActionButton: FloatingActionButton(
        onPressed: () => Navigator.pushNamed(context, '/area-builder'),
        child: const Icon(Icons.add),
      ),
    );
  }
}
