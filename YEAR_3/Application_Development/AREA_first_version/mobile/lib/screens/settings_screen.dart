import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/server_provider.dart';
import '../providers/auth_provider.dart';
import '../services/api_service.dart';

class SettingsScreen extends StatefulWidget {
  const SettingsScreen({super.key});

  @override
  State<SettingsScreen> createState() => _SettingsScreenState();
}

class _SettingsScreenState extends State<SettingsScreen> {
  final _formKey = GlobalKey<FormState>();
  final _urlController = TextEditingController();

  @override
  void initState() {
    super.initState();
    final serverProvider = Provider.of<ServerProvider>(context, listen: false);
    _urlController.text = serverProvider.serverUrl;
  }

  @override
  void dispose() {
    _urlController.dispose();
    super.dispose();
  }

  Future<void> _saveSettings() async {
    if (_formKey.currentState!.validate()) {
      final serverProvider =
          Provider.of<ServerProvider>(context, listen: false);
      await serverProvider.setServerUrl(_urlController.text.trim());

      if (!mounted) return;

      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Configuration sauvegardée')),
      );

      Navigator.pushReplacementNamed(context, '/login');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Configuration'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Form(
          key: _formKey,
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              const Icon(
                Icons.settings,
                size: 80,
                color: Colors.deepPurple,
              ),
              const SizedBox(height: 32),
              Text(
                'Configuration du serveur',
                style: Theme.of(context).textTheme.headlineSmall,
                textAlign: TextAlign.center,
              ),
              const SizedBox(height: 32),
              TextFormField(
                controller: _urlController,
                decoration: const InputDecoration(
                  labelText: 'URL du serveur',
                  hintText: 'http://10.0.2.2:8082',
                  prefixIcon: Icon(Icons.link),
                  border: OutlineInputBorder(),
                ),
                keyboardType: TextInputType.url,
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return 'Veuillez entrer l\'URL du serveur';
                  }
                  if (!value.startsWith('http://') &&
                      !value.startsWith('https://')) {
                    return 'L\'URL doit commencer par http:// ou https://';
                  }
                  return null;
                },
              ),
              const SizedBox(height: 24),
              ElevatedButton(
                onPressed: _saveSettings,
                style: ElevatedButton.styleFrom(
                  padding: const EdgeInsets.symmetric(vertical: 16),
                ),
                child: const Text('Sauvegarder'),
              ),
              const SizedBox(height: 32),
              
              // Connected Services Section - Only if logged in (check auth provider)
              Consumer<AuthProvider>(
                builder: (context, auth, _) {
                  if (!auth.isAuthenticated) return const SizedBox.shrink();
                  
                  return Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                       Text(
                        'Services Connectés',
                        style: Theme.of(context).textTheme.titleMedium,
                      ),
                      const SizedBox(height: 16),
                      Card(
                        child: ListTile(
                          leading: const Icon(Icons.g_mobiledata, size: 40, color: Colors.blue),
                          title: const Text('Google'),
                          subtitle: const Text('Pour les actions/réactions Google'),
                          trailing: ElevatedButton(
                            onPressed: () async {
                                 final serverProvider = Provider.of<ServerProvider>(context, listen: false);
                                 final apiService = ApiService(serverProvider.serverUrl);
                                 
                                 try {
                                   await auth.connectGoogleAccount(apiService);
                                   if (context.mounted) {
                                      ScaffoldMessenger.of(context).showSnackBar(
                                        const SnackBar(content: Text('Compte Google connecté')),
                                      );
                                   }
                                 } catch (e) {
                                   if (context.mounted) {
                                      ScaffoldMessenger.of(context).showSnackBar(
                                        SnackBar(content: Text('Erreur: $e')),
                                      );
                                   }
                                 }
                            },
                            child: const Text('Connecter'),
                          ),
                        ),
                      ),
                    ],
                  );
                },
              ),
            ],
          ),
        ),
      ),
    );
  }
}
