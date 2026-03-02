import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/auth_provider.dart';
import '../providers/server_provider.dart';
import '../services/api_service.dart';

class HomeScreen extends StatelessWidget {
  const HomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final authProvider = Provider.of<AuthProvider>(context);

    // Explicit Dark Theme Colors (matching Web / RegisterScreen / ServicesScreen)
    const textColor = Colors.white;
    const subtitleColor = Color(0xFF9CA3AF); // Gray 400

    return Scaffold(
      // Background handled by main.dart Theme (ScaffoldBackgroundColor)
      appBar: AppBar(
        title: const Text('AREA'),
        actions: [
          IconButton(
            icon: const Icon(Icons.settings),
            onPressed: () => Navigator.pushNamed(context, '/settings'),
          ),
          IconButton(
            icon: const Icon(Icons.logout),
            onPressed: () async {
              final serverProvider =
                  Provider.of<ServerProvider>(context, listen: false);
              final apiService = ApiService(serverProvider.serverUrl);

              await authProvider.logout(apiService);
              if (context.mounted) {
                Navigator.pushReplacementNamed(context, '/login');
              }
            },
          ),
        ],
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              // Welcome Card
              Card(
                elevation: 0,
                // Color handled by Theme (CardTheme), but can enforce if needed
                child: Padding(
                  padding: const EdgeInsets.all(20.0),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        'Welcome to AREA',
                        style: Theme.of(context).textTheme.titleLarge?.copyWith(
                              fontWeight: FontWeight.w600,
                              color: textColor,
                            ),
                      ),
                      const SizedBox(height: 8),
                      Text(
                        'Your automation platform is ready. Start connecting services and creating automations.',
                        style: Theme.of(context).textTheme.bodyMedium?.copyWith(
                              color: subtitleColor,
                            ),
                      ),
                    ],
                  ),
                ),
              ),
              const SizedBox(height: 24),

              // Stats Cards
              LayoutBuilder(
                builder: (context, constraints) {
                  return Row(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Expanded(
                        child: _buildStatCard(
                          context,
                          title: 'Active AREAs',
                          value: '0', 
                          onTap: () => Navigator.pushNamed(context, '/my-areas'),
                          textColor: textColor,
                          subtitleColor: subtitleColor,
                        ),
                      ),
                      const SizedBox(width: 12),
                      Expanded(
                        child: _buildStatCard(
                          context,
                          title: 'Connected Services',
                          value: _getConnectedServicesCount(context),
                          onTap: () => Navigator.pushNamed(context, '/services'),
                          textColor: textColor,
                          subtitleColor: subtitleColor,
                        ),
                      ),
                      const SizedBox(width: 12),
                      Expanded(
                        child: _buildStatCard(
                          context,
                          title: 'Executions Today',
                          value: '0',
                          onTap: () {},
                          textColor: textColor,
                          subtitleColor: subtitleColor,
                        ),
                      ),
                    ],
                  );
                },
              ),
              const SizedBox(height: 24),

              // Call to Action Cards
              Row(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Expanded(
                    child: _buildCTACard(
                    context,
                    title: 'Connect Services',
                    description:
                        'Connect your Google account to unlock email automation.',
                    buttonText: 'Manage Services',
                    color: const Color(0xFF4F46E5), // Indigo
                    lightColor: const Color(0xFF312E81), // Darker Indigo for Dark Mode
                    onTap: () => Navigator.pushNamed(context, '/services'),
                    textColor: textColor,
                    subtitleColor: subtitleColor,
                  ),
                  ),
                  const SizedBox(width: 16),
                  Expanded(
                    child: _buildCTACard(
                    context,
                    title: 'Create AREA',
                    description:
                        'Build your first automation by connecting an Action and a REAction.',
                    buttonText: 'Create Automation',
                     color: const Color(0xFF10B981), // Emerald
                    lightColor: const Color(0xFF064E3B), // Darker Emerald
                    onTap: () => Navigator.pushNamed(context, '/area-builder'),
                    textColor: textColor,
                    subtitleColor: subtitleColor,
                  ),
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }

  String _getConnectedServicesCount(BuildContext context) {
    return '0'; 
  }

  Widget _buildStatCard(
    BuildContext context, {
    required String title,
    required String value,
    required VoidCallback onTap,
    required Color textColor,
    required Color subtitleColor,
  }) {
    return Card(
      elevation: 0,
      child: InkWell(
        onTap: onTap,
        borderRadius: BorderRadius.circular(8),
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                title,
                style: TextStyle(
                  fontSize: 12,
                  fontWeight: FontWeight.w500,
                  color: subtitleColor,
                ),
                maxLines: 1,
                overflow: TextOverflow.ellipsis,
              ),
              const SizedBox(height: 8),
              Text(
                value,
                style: TextStyle(
                  fontSize: 24,
                  fontWeight: FontWeight.bold,
                  color: textColor,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildCTACard(
    BuildContext context, {
    required String title,
    required String description,
    required String buttonText,
    required Color color,
    required Color lightColor,
    required VoidCallback onTap,
    required Color textColor,
    required Color subtitleColor,
  }) {
    return Card(
      elevation: 0,
      color: lightColor.withOpacity(0.4),
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(8),
        side: BorderSide(color: color.withOpacity(0.3)),
      ),
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              title,
              style: TextStyle(
                fontSize: 16,
                fontWeight: FontWeight.bold,
                color: textColor,
              ),
            ),
            const SizedBox(height: 8),
            Text(
              description,
              style: TextStyle(
                fontSize: 12,
                color: Colors.grey[300], // Keep lighter grey for CTA description
                height: 1.4,
              ),
            ),
            const SizedBox(height: 16),
            SizedBox(
              width: double.infinity,
              child: ElevatedButton(
                onPressed: onTap,
                style: ElevatedButton.styleFrom(
                  backgroundColor: color,
                  foregroundColor: Colors.white,
                  padding: const EdgeInsets.symmetric(
                    horizontal: 12,
                    vertical: 10,
                  ),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(6),
                  ),
                  elevation: 0,
                ),
                child: Text(
                  buttonText,
                  style: const TextStyle(
                    fontSize: 13,
                    fontWeight: FontWeight.w600,
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
