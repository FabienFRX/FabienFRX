import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/server_provider.dart';
import '../providers/auth_provider.dart';
import '../providers/area_provider.dart';
import '../services/api_service.dart';
import '../utils/action_reaction_mapper.dart';

class ServicesScreen extends StatefulWidget {
  const ServicesScreen({super.key});

  @override
  State<ServicesScreen> createState() => _ServicesScreenState();
}

class _ServicesScreenState extends State<ServicesScreen> {
  bool _isLoading = false;
  List<dynamic> _connectedServices = [];
  List<dynamic> _availableServices = [];

  @override
  void initState() {
    super.initState();
    _loadData();
  }

  Future<void> _loadData() async {
    setState(() => _isLoading = true);

    try {
      final serverProvider =
          Provider.of<ServerProvider>(context, listen: false);
      final authProvider = Provider.of<AuthProvider>(context, listen: false);
      final areaProvider = Provider.of<AreaProvider>(context, listen: false);
      final apiService = ApiService(serverProvider.serverUrl);

      // Charger les services connectés
      final connectedData =
          await apiService.getUserServices(authProvider.token!);
      _connectedServices = connectedData['services'] ?? [];

      // Charger les services disponibles
      final servicesData = await apiService.getServices(authProvider.token!);
      if (servicesData.containsKey('services')) {
        final services = servicesData['services'] as List;
        final enrichedServices = ActionReactionMapper.enrichServices(services);
        _availableServices = enrichedServices;
        areaProvider.setServices(enrichedServices);
      }
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

  bool _isServiceConnected(String serviceName) {
    return _connectedServices.any((s) => s['service_name'] == serviceName);
  }

  @override
  Widget build(BuildContext context) {
    final googleService = _availableServices.firstWhere(
      (s) => s['name'] == 'google',
      orElse: () => {'name': 'google', 'actions': [], 'reactions': []},
    );
    final isGoogleConnected = _isServiceConnected('google');

    return Scaffold(
      appBar: AppBar(
        title: const Text('Services'),
        actions: [
          IconButton(
            icon: const Icon(Icons.refresh),
            onPressed: _loadData,
          ),
        ],
      ),
      body: _isLoading
          ? const Center(child: CircularProgressIndicator())
          : SingleChildScrollView(
              padding: const EdgeInsets.all(16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    'Services',
                    style: Theme.of(context).textTheme.headlineMedium?.copyWith(
                          fontWeight: FontWeight.bold,
                        ),
                  ),
                  const SizedBox(height: 8),
                  Text(
                    'Connectez des services pour débloquer Actions et REActions',
                    style: Theme.of(context).textTheme.bodyMedium?.copyWith(
                          color: Colors.grey[600],
                        ),
                  ),
                  const SizedBox(height: 24),

                  // Info Mode Développement
                  Card(
                    color: Colors.blue[50],
                    child: Padding(
                      padding: const EdgeInsets.all(16),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Row(
                            children: [
                              Icon(Icons.info, color: Colors.blue[800]),
                              const SizedBox(width: 8),
                              Text(
                                'Mode Développement',
                                style: TextStyle(
                                  fontWeight: FontWeight.bold,
                                  color: Colors.blue[800],
                                ),
                              ),
                            ],
                          ),
                          const SizedBox(height: 8),
                          Text(
                            'Actuellement, seuls les scopes de base Google sont activés. '
                            'Pour Gmail/Calendar en production, l\'app doit être validée par Google.',
                            style: TextStyle(
                                color: Colors.blue[900], fontSize: 13),
                          ),
                        ],
                      ),
                    ),
                  ),
                  const SizedBox(height: 24),

                  // Google Service Card
                  _buildServiceCard(context, googleService, isGoogleConnected),

                  const SizedBox(height: 16),

                  // Placeholder autres services
                  Card(
                    child: Container(
                      padding: const EdgeInsets.all(24),
                      decoration: BoxDecoration(
                        border: Border.all(color: Colors.grey[300]!, width: 2),
                        borderRadius: BorderRadius.circular(12),
                      ),
                      child: Center(
                        child: Column(
                          children: [
                            Icon(Icons.more_horiz,
                                size: 48, color: Colors.grey[400]),
                            const SizedBox(height: 8),
                            Text(
                              'Plus de services bientôt...',
                              style: TextStyle(
                                color: Colors.grey[600],
                                fontSize: 16,
                                fontWeight: FontWeight.w500,
                              ),
                            ),
                          ],
                        ),
                      ),
                    ),
                  ),

                  // Liste des services connectés
                  if (_connectedServices.isNotEmpty) ...[
                    const SizedBox(height: 32),
                    Text(
                      'Services Connectés',
                      style: Theme.of(context).textTheme.titleLarge?.copyWith(
                            fontWeight: FontWeight.bold,
                          ),
                    ),
                    const SizedBox(height: 16),
                    Card(
                      child: ListView.separated(
                        shrinkWrap: true,
                        physics: const NeverScrollableScrollPhysics(),
                        itemCount: _connectedServices.length,
                        separatorBuilder: (context, index) => const Divider(),
                        itemBuilder: (context, index) {
                          final service = _connectedServices[index];
                          return ListTile(
                            title: Text(
                              service['service_name'] ?? 'Unknown',
                              style: const TextStyle(
                                fontWeight: FontWeight.w500,
                                textBaseline: TextBaseline.alphabetic,
                              ),
                            ),
                            subtitle: Text(
                              'Connecté en tant que: ${service['service_username']}',
                            ),
                            trailing: Text(
                              service['is_active'] ? 'Actif' : 'Inactif',
                              style: TextStyle(
                                fontSize: 12,
                                color: Colors.grey[600],
                              ),
                            ),
                          );
                        },
                      ),
                    ),
                  ],
                ],
              ),
            ),
    );
  }

  Widget _buildServiceCard(
      BuildContext context, Map<String, dynamic> service, bool isConnected) {
    final actions = service['actions'] as List? ?? [];
    final reactions = service['reactions'] as List? ?? [];

    return Card(
      elevation: 4,
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                Container(
                  width: 48,
                  height: 48,
                  decoration: BoxDecoration(
                    gradient: const LinearGradient(
                      colors: [Colors.blue, Colors.red],
                    ),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  child: const Icon(Icons.g_mobiledata,
                      color: Colors.white, size: 32),
                ),
                const SizedBox(width: 12),
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      const Text(
                        'Google',
                        style: TextStyle(
                          fontSize: 18,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                      if (isConnected)
                        Container(
                          padding: const EdgeInsets.symmetric(
                            horizontal: 8,
                            vertical: 2,
                          ),
                          decoration: BoxDecoration(
                            color: Colors.green[100],
                            borderRadius: BorderRadius.circular(12),
                          ),
                          child: Text(
                            'Connecté',
                            style: TextStyle(
                              color: Colors.green[800],
                              fontSize: 12,
                              fontWeight: FontWeight.w600,
                            ),
                          ),
                        ),
                    ],
                  ),
                ),
              ],
            ),
            const SizedBox(height: 12),
            Text(
              'Accédez à Gmail, Calendar et plus pour une automatisation puissante',
              style: TextStyle(color: Colors.grey[700], fontSize: 14),
            ),
            if (actions.isNotEmpty) ...[
              const SizedBox(height: 16),
              Text(
                'ACTIONS DISPONIBLES:',
                style: TextStyle(
                  fontSize: 11,
                  fontWeight: FontWeight.bold,
                  color: Colors.grey[600],
                ),
              ),
              const SizedBox(height: 8),
              ...actions.map((action) => Padding(
                    padding: const EdgeInsets.symmetric(vertical: 4),
                    child: Row(
                      children: [
                        const Icon(Icons.circle, size: 6, color: Colors.grey),
                        const SizedBox(width: 8),
                        Expanded(
                          child: Text(
                            action['description'] ?? action['name'],
                            style: TextStyle(
                              fontSize: 13,
                              color: Colors.grey[800],
                            ),
                          ),
                        ),
                      ],
                    ),
                  )),
            ],
            if (reactions.isNotEmpty) ...[
              const SizedBox(height: 16),
              Text(
                'REACTIONS DISPONIBLES:',
                style: TextStyle(
                  fontSize: 11,
                  fontWeight: FontWeight.bold,
                  color: Colors.grey[600],
                ),
              ),
              const SizedBox(height: 8),
              ...reactions.map((reaction) => Padding(
                    padding: const EdgeInsets.symmetric(vertical: 4),
                    child: Row(
                      children: [
                        const Icon(Icons.circle, size: 6, color: Colors.grey),
                        const SizedBox(width: 8),
                        Expanded(
                          child: Text(
                            reaction['description'] ?? reaction['name'],
                            style: TextStyle(
                              fontSize: 13,
                              color: Colors.grey[800],
                            ),
                          ),
                        ),
                      ],
                    ),
                  )),
            ],
            const SizedBox(height: 16),
            SizedBox(
              width: double.infinity,
              child: ElevatedButton(
                onPressed: () => _handleServiceAction(isConnected, 'google'),
                style: ElevatedButton.styleFrom(
                  backgroundColor: isConnected ? Colors.red : Colors.indigo,
                  padding: const EdgeInsets.symmetric(vertical: 12),
                ),
                child: Text(
                  isConnected ? 'Déconnecter' : 'Connecter Google',
                  style: const TextStyle(fontSize: 16),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  Future<void> _handleServiceAction(bool isConnected, String serviceName) async {
    if (serviceName != 'google') return;

    final authProvider = Provider.of<AuthProvider>(context, listen: false);
    final serverProvider = Provider.of<ServerProvider>(context, listen: false);
    final apiService = ApiService(serverProvider.serverUrl);

    setState(() => _isLoading = true);

    try {
      if (isConnected) {
        // Find service ID for Google
        final service = _connectedServices.firstWhere(
            (s) => s['service_name'] == 'google',
            orElse: () => null);
        
        if (service != null) {
           await apiService.disconnectService(authProvider.token!, service['service_id']); 
           if (mounted) {
             ScaffoldMessenger.of(context).showSnackBar(
               const SnackBar(content: Text('Service Google déconnecté')),
             );
           }
        }
      } else {
        await authProvider.connectGoogleAccount(apiService);
        if (mounted) {
             ScaffoldMessenger.of(context).showSnackBar(
               const SnackBar(content: Text('Service Google connecté')),
             );
        }
      }
      // Refresh list
      await _loadData();
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('Erreur: $e')),
        );
      }
    } finally {
      if (mounted) {
        setState(() => _isLoading = false);
      }
    }
  }

}
