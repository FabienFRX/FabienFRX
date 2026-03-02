import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/auth_provider.dart';
import '../providers/server_provider.dart';
import '../providers/area_provider.dart';
import '../services/api_service.dart';
import '../utils/action_reaction_mapper.dart';

class AREABuilderScreen extends StatefulWidget {
  const AREABuilderScreen({super.key});

  @override
  State<AREABuilderScreen> createState() => _AREABuilderScreenState();
}

class _AREABuilderScreenState extends State<AREABuilderScreen> {
  String? _selectedService;
  Map<String, dynamic>? _selectedAction;
  Map<String, dynamic>? _selectedReaction;
  bool _isLoading = false;

  @override
  void initState() {
    super.initState();
    _loadServices();
  }

  Future<void> _loadServices() async {
    final serverProvider = Provider.of<ServerProvider>(context, listen: false);
    final authProvider = Provider.of<AuthProvider>(context, listen: false);
    final areaProvider = Provider.of<AreaProvider>(context, listen: false);

    if (areaProvider.services.isEmpty) {
      try {
        final apiService = ApiService(serverProvider.serverUrl);
        // Récupère /services/ qui contient la liste des services avec IDs
        final servicesData = await apiService.getServices(authProvider.token!);

        // Parse la structure: { "services": [...] }
        if (servicesData.containsKey('services')) {
          final services = servicesData['services'] as List;
          // Enrichir les services avec les IDs depuis le mapper
          final enrichedServices =
              ActionReactionMapper.enrichServices(services);
          areaProvider.setServices(enrichedServices);
        }
      } catch (e) {
        if (!mounted) return;
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('Erreur: ${e.toString()}')),
        );
      }
    }
  }

  Future<void> _createArea() async {
    if (_selectedAction == null || _selectedReaction == null) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('Veuillez sélectionner une action et une réaction'),
        ),
      );
      return;
    }

    setState(() => _isLoading = true);

    try {
      final serverProvider =
          Provider.of<ServerProvider>(context, listen: false);
      final authProvider = Provider.of<AuthProvider>(context, listen: false);
      final apiService = ApiService(serverProvider.serverUrl);

      // Générer un nom pour l'AREA
      final areaName =
          'Si ${_selectedAction!['name']} alors ${_selectedReaction!['name']}';

      await apiService.createArea(authProvider.token!, {
        'name': areaName,
        'action_id': _selectedAction!['id'],
        'reaction_id': _selectedReaction!['id'],
        'enabled': true,
      });

      if (!mounted) return;

      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('AREA créée avec succès!')),
      );

      Navigator.pushReplacementNamed(context, '/my-areas');
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

  @override
  Widget build(BuildContext context) {
    final areaProvider = Provider.of<AreaProvider>(context);
    final services = areaProvider.services;

    return Scaffold(
      appBar: AppBar(
        title: const Text('Créer une AREA'),
      ),
      body: services.isEmpty
          ? const Center(child: CircularProgressIndicator())
          : SingleChildScrollView(
              padding: const EdgeInsets.all(16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.stretch,
                children: [
                  Card(
                    child: Padding(
                      padding: const EdgeInsets.all(16),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Row(
                            children: [
                              const Icon(Icons.info_outline, size: 20),
                              const SizedBox(width: 8),
                              Text(
                                'Créer une automation',
                                style: Theme.of(context).textTheme.titleMedium,
                              ),
                            ],
                          ),
                          const SizedBox(height: 8),
                          const Text(
                            'Sélectionnez un service, puis une action et une réaction pour créer votre AREA.',
                          ),
                        ],
                      ),
                    ),
                  ),
                  const SizedBox(height: 24),
                  Text(
                    '1. Service',
                    style: Theme.of(context).textTheme.titleMedium,
                  ),
                  const SizedBox(height: 8),
                  DropdownButtonFormField<String>(
                    decoration: const InputDecoration(
                      border: OutlineInputBorder(),
                      prefixIcon: Icon(Icons.apps),
                      hintText: 'Choisir un service',
                    ),
                    value: _selectedService,
                    items: services.map<DropdownMenuItem<String>>((service) {
                      return DropdownMenuItem<String>(
                        value: service['name'] as String,
                        child: Text(service['name'] as String),
                      );
                    }).toList(),
                    onChanged: (value) {
                      setState(() {
                        _selectedService = value;
                        _selectedAction = null;
                        _selectedReaction = null;
                      });
                    },
                  ),
                  if (_selectedService != null) ...[
                    const SizedBox(height: 24),
                    Text(
                      '2. Action (SI)',
                      style: Theme.of(context).textTheme.titleMedium,
                    ),
                    const SizedBox(height: 8),
                    DropdownButtonFormField<Map<String, dynamic>>(
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        prefixIcon: Icon(Icons.input),
                        hintText: 'Choisir une action',
                      ),
                      value: _selectedAction,
                      items: _getActions().map((action) {
                        return DropdownMenuItem(
                          value: action,
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              Text(action['name']),
                              if (action['description'] != null)
                                Text(
                                  action['description'],
                                  style: Theme.of(context).textTheme.bodySmall,
                                ),
                            ],
                          ),
                        );
                      }).toList(),
                      onChanged: (value) {
                        setState(() => _selectedAction = value);
                      },
                    ),
                    const SizedBox(height: 24),
                    Text(
                      '3. Réaction (ALORS)',
                      style: Theme.of(context).textTheme.titleMedium,
                    ),
                    const SizedBox(height: 8),
                    DropdownButtonFormField<Map<String, dynamic>>(
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        prefixIcon: Icon(Icons.output),
                        hintText: 'Choisir une réaction',
                      ),
                      value: _selectedReaction,
                      items: _getReactions().map((reaction) {
                        return DropdownMenuItem(
                          value: reaction,
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              Text(reaction['name']),
                              if (reaction['description'] != null)
                                Text(
                                  reaction['description'],
                                  style: Theme.of(context).textTheme.bodySmall,
                                ),
                            ],
                          ),
                        );
                      }).toList(),
                      onChanged: (value) {
                        setState(() => _selectedReaction = value);
                      },
                    ),
                  ],
                  if (_selectedAction != null && _selectedReaction != null) ...[
                    const SizedBox(height: 24),
                    Card(
                      color: Theme.of(context).colorScheme.secondaryContainer,
                      child: Padding(
                        padding: const EdgeInsets.all(16),
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Text(
                              'Aperçu de l\'AREA',
                              style: Theme.of(context)
                                  .textTheme
                                  .titleMedium
                                  ?.copyWith(
                                    fontWeight: FontWeight.bold,
                                  ),
                            ),
                            const SizedBox(height: 12),
                            Row(
                              children: [
                                const Icon(Icons.input, size: 20),
                                const SizedBox(width: 8),
                                Expanded(
                                  child: Text(
                                    'SI: ${_selectedAction!['name']}',
                                    style: const TextStyle(
                                      fontWeight: FontWeight.w500,
                                    ),
                                  ),
                                ),
                              ],
                            ),
                            const SizedBox(height: 8),
                            Row(
                              children: [
                                const Icon(Icons.output, size: 20),
                                const SizedBox(width: 8),
                                Expanded(
                                  child: Text(
                                    'ALORS: ${_selectedReaction!['name']}',
                                    style: const TextStyle(
                                      fontWeight: FontWeight.w500,
                                    ),
                                  ),
                                ),
                              ],
                            ),
                          ],
                        ),
                      ),
                    ),
                    const SizedBox(height: 24),
                    ElevatedButton.icon(
                      onPressed: _isLoading ? null : _createArea,
                      icon: _isLoading
                          ? const SizedBox(
                              width: 20,
                              height: 20,
                              child: CircularProgressIndicator(strokeWidth: 2),
                            )
                          : const Icon(Icons.check),
                      label: const Text('Créer l\'AREA'),
                      style: ElevatedButton.styleFrom(
                        padding: const EdgeInsets.symmetric(vertical: 16),
                      ),
                    ),
                  ],
                ],
              ),
            ),
    );
  }

  List<Map<String, dynamic>> _getActions() {
    final areaProvider = Provider.of<AreaProvider>(context, listen: false);
    final service =
        areaProvider.services.firstWhere((s) => s['name'] == _selectedService);
    return List<Map<String, dynamic>>.from(service['actions'] ?? []);
  }

  List<Map<String, dynamic>> _getReactions() {
    final areaProvider = Provider.of<AreaProvider>(context, listen: false);
    final service =
        areaProvider.services.firstWhere((s) => s['name'] == _selectedService);
    return List<Map<String, dynamic>>.from(service['reactions'] ?? []);
  }
}
