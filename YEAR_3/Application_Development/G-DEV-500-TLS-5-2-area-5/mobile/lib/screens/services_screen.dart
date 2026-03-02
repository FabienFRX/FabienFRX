import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/server_provider.dart';
import '../providers/auth_provider.dart';
import '../providers/area_provider.dart';
import '../services/api_service.dart';
import '../config/secrets.dart';
import '../utils/action_reaction_mapper.dart';
import 'dropbox_auth_screen.dart';
import 'linkedin_auth_screen.dart';

import 'github_auth_screen.dart';
import 'gitlab_auth_screen.dart';

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
        SnackBar(content: Text('Error: ${e.toString()}')),
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
    // Dark Theme Colors
    const backgroundColor = Color(0xFF111827); // Gray 900
    const textColor = Colors.white;
    const subtitleColor = Color(0xFF9CA3AF); // Gray 400

    // Prepare Services
    final googleService = _getServiceOrEmpty('google');
    final isGoogleConnected = _isServiceConnected('google');

    final githubService = _getServiceOrEmpty('github');
    final isGithubConnected = _isServiceConnected('github');

    final gitlabService = _getServiceOrEmpty('gitlab');
    final isGitlabConnected = _isServiceConnected('gitlab');

    final linkedinService = _getServiceOrEmpty('linkedin');
    final isLinkedinConnected = _isServiceConnected('linkedin');

    final dropboxService = _getServiceOrEmpty('dropbox');
    final isDropboxConnected = _isServiceConnected('dropbox');

    return Scaffold(
      backgroundColor: backgroundColor,
      appBar: AppBar(
        title: const Text('Services'),
        backgroundColor: backgroundColor,
        elevation: 0,
        titleTextStyle: const TextStyle(
            color: textColor, fontSize: 20, fontWeight: FontWeight.bold),
        iconTheme: const IconThemeData(color: textColor),
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
                          color: textColor,
                        ),
                  ),
                  const SizedBox(height: 8),
                  Text(
                    'Connect services to unlock Actions and REActions',
                    style: Theme.of(context).textTheme.bodyMedium?.copyWith(
                          color: subtitleColor,
                        ),
                  ),
                  const SizedBox(height: 24),

                  // Info Mode Développement
                  Container(
                    padding: const EdgeInsets.all(16),
                    decoration: BoxDecoration(
                      color: const Color(0xFF1E3A8A).withOpacity(0.2), // Dark Blue bg
                      borderRadius: BorderRadius.circular(8),
                      border: Border.all(color: const Color(0xFF1E40AF)), // Blue border
                    ),
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Row(
                          children: [
                            const Icon(Icons.info_outline, color: Color(0xFF60A5FA)),
                            const SizedBox(width: 8),
                            const Text(
                              'Mode Développement',
                              style: TextStyle(
                                fontWeight: FontWeight.bold,
                                color: Color(0xFF60A5FA),
                              ),
                            ),
                          ],
                        ),
                        const SizedBox(height: 8),
                        Text(
                          'Actuellement, seuls les scopes de base Google sont activés. '
                          'Pour Gmail/Calendar en production, l\'app doit être validée par Google.',
                          style: TextStyle(color: Colors.blue[100], fontSize: 13),
                        ),
                      ],
                    ),
                  ),
                  const SizedBox(height: 24),

                  // Services Grid - Responsive
                  LayoutBuilder(
                    builder: (context, constraints) {
                      // Adjust grid based on width
                      int crossAxisCount = constraints.maxWidth > 900 ? 3 : (constraints.maxWidth > 600 ? 2 : 1);
                      double ratio = constraints.maxWidth > 600 ? 0.8 : 1.1; // Tweak ratio for mobile cards

                      if (crossAxisCount > 1) {
                         return GridView.count(
                            crossAxisCount: crossAxisCount,
                            crossAxisSpacing: 16,
                            mainAxisSpacing: 16,
                            shrinkWrap: true,
                            physics: const NeverScrollableScrollPhysics(),
                            childAspectRatio: ratio,
                            children: _buildServiceList(
                                googleService, isGoogleConnected,
                                githubService, isGithubConnected,
                                gitlabService, isGitlabConnected,
                                linkedinService, isLinkedinConnected,
                                dropboxService, isDropboxConnected
                            ),
                         );
                      } else {
                        // Mobile List
                        return Column(
                          children: _buildServiceList(
                                googleService, isGoogleConnected,
                                githubService, isGithubConnected,
                                gitlabService, isGitlabConnected,
                                linkedinService, isLinkedinConnected,
                                dropboxService, isDropboxConnected
                            ).map((widget) => Padding(
                              padding: const EdgeInsets.only(bottom: 16.0),
                              child: widget,
                            )).toList(),
                        );
                      }
                    },
                  ),
                  
                  // Connected Services List (Bottom Summary)
                  if (_connectedServices.isNotEmpty) ...[
                    const SizedBox(height: 32),
                    const Text(
                      'Connected Services',
                      style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold, color: textColor),
                    ),
                    const SizedBox(height: 16),
                    Container(
                      decoration: BoxDecoration(
                        color: const Color(0xFF1F2937),
                        borderRadius: BorderRadius.circular(8),
                      ),
                      child: ListView.separated(
                        shrinkWrap: true,
                        physics: const NeverScrollableScrollPhysics(),
                        itemCount: _connectedServices.length,
                        separatorBuilder: (context, index) => const Divider(color: Colors.grey),
                        itemBuilder: (context, index) {
                          final service = _connectedServices[index];
                          return ListTile(
                            title: Text(
                              service['service_name'] ?? 'Unknown',
                              style: const TextStyle(fontWeight: FontWeight.w500, color: textColor),
                            ),
                            subtitle: Text(
                              'Connected as: ${service['service_username']}',
                              style: TextStyle(color: subtitleColor),
                            ),
                            trailing: Text(
                              service['is_active'] ? 'Active' : 'Inactive',
                              style: TextStyle(fontSize: 12, color: subtitleColor),
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

  Map<String, dynamic> _getServiceOrEmpty(String name) {
    return _availableServices.firstWhere(
      (s) => s['name'] == name,
      orElse: () => {'name': name, 'actions': [], 'reactions': []},
    );
  }

  List<Widget> _buildServiceList(
      Map<String, dynamic> google, bool googleConnected,
      Map<String, dynamic> github, bool githubConnected,
      Map<String, dynamic> gitlab, bool gitlabConnected,
      Map<String, dynamic> linkedin, bool linkedinConnected,
      Map<String, dynamic> dropbox, bool dropboxConnected
  ) {
    return [
      _buildServiceCard(google, googleConnected, 'google'),
      _buildServiceCard(github, githubConnected, 'github'),
      _buildServiceCard(gitlab, gitlabConnected, 'gitlab'),
      _buildServiceCard(linkedin, linkedinConnected, 'linkedin'),
      _buildServiceCard(dropbox, dropboxConnected, 'dropbox'),
      _buildComingSoonCard(),
    ];
  }

  Widget _buildServiceCard(Map<String, dynamic> service, bool isConnected, String serviceName) {
    final actions = service['actions'] as List? ?? [];
    final reactions = service['reactions'] as List? ?? [];

    Widget icon;
    String description;
    String displayName;

    switch (serviceName) {
      case 'google':
        icon = const Icon(Icons.g_mobiledata, color: Colors.white, size: 32);
        description = 'Access Gmail, Calendar and more for powerful automation';
        displayName = 'Google';
        break;
      case 'github':
        icon = const Icon(Icons.code, color: Colors.white, size: 28); // GitHub icon replacement
        description = 'Manage repositories, issues, and pull requests';
        displayName = 'GitHub';
        break;
      case 'gitlab':
        icon = const Icon(Icons.code, color: Colors.white, size: 28); // GitLab icon replacement
        description = 'DevOps platform for source code management';
        displayName = 'GitLab';
        break;
      case 'linkedin':
        icon = const Icon(Icons.business, color: Colors.blueAccent, size: 28);
        description = 'Connect to your professional network and share updates';
        displayName = 'LinkedIn';
        break;
      case 'dropbox':
        icon = const Icon(Icons.cloud, color: Colors.blue, size: 28);
        description = 'Manage and automate your Dropbox files and folders';
        displayName = 'Dropbox';
        break;
      default:
        icon = const Icon(Icons.help);
        description = 'Service description';
        displayName = serviceName;
    }

    return Container(
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: const Color(0xFF1F2937), // Card BG
        borderRadius: BorderRadius.circular(12),
        border: Border.all(color: const Color(0xFF374151)), // Border color
      ),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Row(
            children: [
              // Icon Container (Simplified)
              Container(
                width: 48, height: 48,
                decoration: BoxDecoration(
                   color: _getIconBgColor(serviceName), // Use helper for BG
                   borderRadius: BorderRadius.circular(8),
                ),
                child: Center(child: icon),
              ),
              const SizedBox(width: 12),
              Expanded(
                child: Column(
                   crossAxisAlignment: CrossAxisAlignment.start,
                   children: [
                     Text(
                        displayName,
                        style: const TextStyle(
                          fontSize: 18,
                          fontWeight: FontWeight.bold,
                          color: Colors.white,
                        ),
                      ),
                      if (isConnected) 
                        Container(
                          padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 2),
                          decoration: BoxDecoration(
                            color: Colors.green.withOpacity(0.2),
                            borderRadius: BorderRadius.circular(12),
                          ),
                          child: const Text('Connected', style: TextStyle(color: Colors.greenAccent, fontSize: 10)),
                        )
                   ],
                )
              ),
            ],
          ),
          const SizedBox(height: 12),
          Text(
            description,
            style: const TextStyle(color: Color(0xFF9CA3AF), fontSize: 13, height: 1.4),
          ),
          const SizedBox(height: 16),
          
          // Actions/Reactions List (Compact)
          if (actions.isNotEmpty || reactions.isNotEmpty) ...[
              const Text('AVAILABLE ACTIONS:', style: TextStyle(fontSize: 10, fontWeight: FontWeight.bold, color: Color(0xFF6B7280))),
              const SizedBox(height: 4),
              ...actions.take(2).map((action) => Text(
                    '• ${action['description'] ?? action['name']}',
                    style: const TextStyle(fontSize: 12, color: Color(0xFFD1D5DB)),
                    maxLines: 1, overflow: TextOverflow.ellipsis,
              )),
              if (reactions.isNotEmpty) const SizedBox(height: 8),
              if (reactions.isNotEmpty) const Text('AVAILABLE REACTIONS:', style: TextStyle(fontSize: 10, fontWeight: FontWeight.bold, color: Color(0xFF6B7280))),
              if (reactions.isNotEmpty) const SizedBox(height: 4),
               ...reactions.take(2).map((reaction) => Text(
                    '• ${reaction['description'] ?? reaction['name']}',
                    style: const TextStyle(fontSize: 12, color: Color(0xFFD1D5DB)),
                    maxLines: 1, overflow: TextOverflow.ellipsis,
              )),
             const SizedBox(height: 16),
          ],
          const SizedBox(height: 24),
          // Connect/Disconnect Button
          SizedBox(
            width: double.infinity,
            child: ElevatedButton(
              onPressed: () => _handleServiceAction(isConnected, serviceName),
              style: ElevatedButton.styleFrom(
                backgroundColor: isConnected 
                    ? Colors.red.withOpacity(0.2)
                    : _getButtonColor(serviceName),
                foregroundColor: isConnected ? Colors.redAccent : Colors.white,
                padding: const EdgeInsets.symmetric(vertical: 12),
                shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(6)),
                elevation: 0,
              ),
              child: Text(
                isConnected ? 'Disconnect' : 'Connect $displayName',
                style: const TextStyle(fontSize: 14, fontWeight: FontWeight.w600),
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildComingSoonCard() {
    return Container(
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: const Color(0xFF1F2937).withOpacity(0.5),
        borderRadius: BorderRadius.circular(12),
        border: Border.all(color: const Color(0xFF374151), style: BorderStyle.solid), // Dashed hard in basic flutter containers
      ),
      child: const Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(Icons.more_horiz, size: 48, color: Color(0xFF4B5563)),
            SizedBox(height: 8),
            Text(
              'More services coming soon...',
              textAlign: TextAlign.center,
              style: TextStyle(color: Color(0xFF6B7280), fontSize: 14),
            ),
          ],
        ),
      ),
    );
  }
  
  Color _getIconBgColor(String serviceName) {
     switch (serviceName) {
       case 'google': return Colors.red; // Or gradient
       case 'github': return const Color(0xFF111827); // Dark
       case 'gitlab': return const Color(0xFFFC6D26); // GitLab Orange
       case 'linkedin': return Colors.blue[800]!;
       case 'dropbox': return Colors.blue[400]!;
       default: return Colors.grey;
     }
  }

  Color _getButtonColor(String serviceName) {
     switch (serviceName) {
       case 'google': return const Color(0xFF4F46E5); // Indigo

       case 'github': return const Color(0xFF1F2937); // Dark Grey
       case 'gitlab': return const Color(0xFFFC6D26); // GitLab Orange
       case 'linkedin': return const Color(0xFF1D4ED8); // Blue
       case 'dropbox': return const Color(0xFF2563EB); // Blue
       default: return const Color(0xFF4F46E5);
     }
  }

  Future<void> _handleServiceAction(bool isConnected, String serviceName) async {
    final authProvider = Provider.of<AuthProvider>(context, listen: false);
    final serverProvider = Provider.of<ServerProvider>(context, listen: false);
    final apiService = ApiService(serverProvider.serverUrl);

    setState(() => _isLoading = true);

    try {
      if (isConnected) {
        final service = _connectedServices.firstWhere(
            (s) => s['service_name'] == serviceName,
            orElse: () => null);
        
        if (service != null) {
           await apiService.disconnectService(authProvider.token!, service['service_id']); 
             if (mounted) {
               ScaffoldMessenger.of(context).showSnackBar(
                 SnackBar(content: Text('Service $serviceName déconnecté')),
               );
             }
        }
      } else {
        if (serviceName == 'google') {
          await authProvider.connectGoogleAccount(apiService);
        } else {
           // Check for Web Platform regarding WebView
          if (const bool.fromEnvironment('dart.library.js_util')) {
             if (mounted) {
               ScaffoldMessenger.of(context).showSnackBar(
                 const SnackBar(content: Text('This service connection is not supported on Web version yet (Requires WebView). Please use the Mobile App.')),
               );
             }
             setState(() => _isLoading = false);
             return;
          }

          if (serviceName == 'linkedin') {
            const String clientId = Secrets.linkedinClientId;
            const String redirectUri = Secrets.linkedinRedirectUri;

            final code = await Navigator.push<String>(
              context,
              MaterialPageRoute(
                builder: (context) => const LinkedInAuthScreen(
                  clientId: clientId,
                  redirectUri: redirectUri,
                ),
              ),
            );

            if (code != null) {
              await authProvider.connectLinkedIn(apiService, code, redirectUri);
            }
          } else if (serviceName == 'dropbox') {
            const String appKey = Secrets.dropboxAppKey;
            const String redirectUri = Secrets.dropboxRedirectUri;

            final code = await Navigator.push<String>(
              context,
              MaterialPageRoute(
                builder: (context) => const DropboxAuthScreen(
                  appKey: appKey,
                  redirectUri: redirectUri,
                ),
              ),
            );

            if (code != null) {
              await authProvider.connectDropbox(apiService, code, redirectUri);
            }
          } else if (serviceName == 'github') {
            const String clientId = Secrets.githubClientId;
            const String redirectUri = Secrets.githubRedirectUri;
            
            final code = await Navigator.push<String>(
              context,
              MaterialPageRoute(
                builder: (context) => const GitHubAuthScreen(
                  clientId: clientId,
                  redirectUri: redirectUri,
                ),
              ),
            );
            
            if (code != null) {
              await authProvider.connectGitHub(apiService, code, redirectUri);
            }
          } else if (serviceName == 'gitlab') {
            const String clientId = Secrets.gitlabClientId;
            const String redirectUri = Secrets.gitlabRedirectUri;
            
            final code = await Navigator.push<String>(
              context,
              MaterialPageRoute(
                builder: (context) => const GitLabAuthScreen(
                  clientId: clientId,
                  redirectUri: redirectUri,
                ),
              ),
            );
            
            if (code != null) {
              await authProvider.connectGitLab(apiService, code, redirectUri);
            }
          }
        }
        
        if (mounted) {
             ScaffoldMessenger.of(context).showSnackBar(
               SnackBar(content: Text('Service $serviceName connected')),
             );
        }
      }
      await _loadData();
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('Error: $e')),
        );
      }
    } finally {
      if (mounted) {
        setState(() => _isLoading = false);
      }
    }
  }
}
