import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/auth_provider.dart';
import '../providers/server_provider.dart';
import '../services/api_service.dart';
import '../widgets/action_config_form.dart';
import '../widgets/reaction_config_form.dart';

class AREABuilderScreen extends StatefulWidget {
  const AREABuilderScreen({super.key});

  @override
  State<AREABuilderScreen> createState() => _AREABuilderScreenState();
}

class _AREABuilderScreenState extends State<AREABuilderScreen> {
  bool _isLoading = false;

  // Form Data
  String _areaName = '';
  String _description = '';

  // Action Data
  Map<String, dynamic>? _selectedActionService;
  Map<String, dynamic>? _selectedAction;
  Map<String, dynamic> _actionParams = {};

  // Reaction Data
  Map<String, dynamic>? _selectedReactionService;
  Map<String, dynamic>? _selectedReaction;
  Map<String, dynamic> _reactionParams = {};

  List<dynamic> _availableServices = [];

  // Theme Colors (Dark Theme)
  static const backgroundColor = Color(0xFF111827); // gray-900
  static const cardColor = Color(0xFF1F2937); // gray-800
  static const textColor = Color(0xFFE5E7EB); // gray-200
  static const secondaryTextColor = Color(0xFF9CA3AF); // gray-400
  static const inputFillColor = Color(0xFF374151); // gray-700
  static const borderColor = Color(0xFF4B5563); // gray-600
  static const primaryColor = Color(0xFF6366F1); // indigo-500
  static const errorColor = Color(0xFFEF4444); // red-500

  @override
  void initState() {
    super.initState();
    _loadServices();
  }

  Future<void> _loadServices() async {
    setState(() => _isLoading = true);
    try {
      final authProvider = Provider.of<AuthProvider>(context, listen: false);
      final serverProvider =
          Provider.of<ServerProvider>(context, listen: false);
      final apiService = ApiService(serverProvider.serverUrl);

      final data = await apiService.getServices(authProvider.token!);
      if (data.containsKey('services')) {
        setState(() {
          _availableServices = data['services'];
        });
      }
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('Error loading services: $e')),
        );
      }
    } finally {
      if (mounted) setState(() => _isLoading = false);
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: backgroundColor,
      appBar: AppBar(
        title: const Text('My AREAs'),
        backgroundColor: backgroundColor, // Match background
        elevation: 0,
        titleTextStyle: const TextStyle(
          color: textColor,
          fontSize: 20,
          fontWeight: FontWeight.bold,
        ),
        iconTheme: const IconThemeData(color: textColor),
        actions: [
          Padding(
            padding: const EdgeInsets.only(right: 16.0),
            child: TextButton(
              onPressed: () => Navigator.pop(context),
              style: TextButton.styleFrom(
                backgroundColor: primaryColor,
                foregroundColor: Colors.white,
                padding:
                    const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(6)),
              ),
              child: const Text('Cancel',
                  style: TextStyle(fontWeight: FontWeight.w600)),
            ),
          ),
        ],
        automaticallyImplyLeading: false, // Removed default back button
      ),
      body: _isLoading
          ? const Center(child: CircularProgressIndicator())
          : SingleChildScrollView(
              padding: const EdgeInsets.all(24.0),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  const Text(
                    'Create New AREA',
                    style: TextStyle(
                      fontSize: 24,
                      fontWeight: FontWeight.bold,
                      color: textColor,
                    ),
                  ),
                  const SizedBox(height: 24),

                  // 1. General Info Section
                  Row(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Expanded(
                        child: _buildTextField(
                          label: 'AREA Name',
                          placeholder: 'Your automation name',
                          onChanged: (val) => setState(() => _areaName = val),
                        ),
                      ),
                      const SizedBox(width: 16),
                      Expanded(
                        child: _buildTextField(
                          label: 'Description',
                          placeholder: 'Describe what this AREA does',
                          onChanged: (val) =>
                              setState(() => _description = val),
                        ),
                      ),
                    ],
                  ),
                  const SizedBox(height: 32),

                  // 2. ACTION Section
                  _buildSectionHeader('ACTION (Trigger)'),
                  const SizedBox(height: 16),
                  _buildSelectionRow(
                    serviceValue: _selectedActionService,
                    serviceLabel: 'Service',
                    typeValue: _selectedAction,
                    typeLabel: 'Action Type',
                    services: _availableServices,
                    onServiceChanged: (val) {
                      setState(() {
                        _selectedActionService = val;
                        _selectedAction = null;
                        _actionParams = {};
                      });
                    },
                    onTypeChanged: (val) {
                      setState(() {
                        _selectedAction = val;
                        _actionParams = {};
                      });
                    },
                    getTypes: (service) => service['actions'] as List,
                  ),
                  
                  // Action Configuration Panel
                  if (_selectedAction != null) ...[
                    const SizedBox(height: 16),
                    Container(
                      padding: const EdgeInsets.all(20),
                      decoration: BoxDecoration(
                        color: const Color(0xFF1E3A8A).withOpacity(0.2), // Dark Blue Tint
                        borderRadius: BorderRadius.circular(8),
                        border: Border.all(color: const Color(0xFF1E40AF).withOpacity(0.5)),
                      ),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                           Text(
                            'Configure Action: ${_selectedAction!['name']}',
                            style: const TextStyle(
                              color: Color(0xFF93C5FD), // Light Blue
                              fontWeight: FontWeight.bold,
                              fontSize: 14,
                            ),
                          ),
                          const SizedBox(height: 12),
                          ActionConfigForm(
                            actionType: _selectedAction!['name'],
                            initialValues: _actionParams,
                            onChanged: (val) => _actionParams = val,
                          ),
                        ],
                      ),
                    ),
                  ],

                  const SizedBox(height: 32),

                  // 3. REACTION Section
                  _buildSectionHeader('REACTION (Response)'),
                  const SizedBox(height: 16),
                  _buildSelectionRow(
                    serviceValue: _selectedReactionService,
                    serviceLabel: 'Service',
                    typeValue: _selectedReaction,
                    typeLabel: 'REAction Type',
                    services: _availableServices,
                    onServiceChanged: (val) {
                      setState(() {
                        _selectedReactionService = val;
                        _selectedReaction = null;
                        _reactionParams = {};
                      });
                    },
                    onTypeChanged: (val) {
                      setState(() {
                        _selectedReaction = val;
                        _reactionParams = {};
                      });
                    },
                    getTypes: (service) => service['reactions'] as List,
                  ),

                  // Reaction Configuration Panel
                  if (_selectedReaction != null) ...[
                    const SizedBox(height: 16),
                     Container(
                      padding: const EdgeInsets.all(20),
                      decoration: BoxDecoration(
                        color: const Color(0xFF064E3B).withOpacity(0.4), // Dark Green Tint
                        borderRadius: BorderRadius.circular(8),
                        border: Border.all(color: const Color(0xFF059669).withOpacity(0.5)),
                      ),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                           Text(
                            'Configure Reaction: ${_selectedReaction!['name']}',
                            style: const TextStyle(
                              color: Color(0xFF6EE7B7), // Light Green
                              fontWeight: FontWeight.bold,
                              fontSize: 14,
                            ),
                          ),
                          const SizedBox(height: 12),
                          ReactionConfigForm(
                            reactionType: _selectedReaction!['name'],
                            initialValues: _reactionParams,
                            onChanged: (val) => _reactionParams = val,
                          ),
                        ],
                      ),
                    ),
                  ],

                  const SizedBox(height: 48),

                  // Create Button
                  SizedBox(
                    width: double.infinity,
                    height: 50,
                    child: ElevatedButton(
                      onPressed: _createArea,
                      style: ElevatedButton.styleFrom(
                        backgroundColor: primaryColor,
                        foregroundColor: Colors.white,
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(8),
                        ),
                        elevation: 0,
                      ),
                      child: const Text(
                        'Create AREA',
                        style: TextStyle(
                            fontSize: 16, fontWeight: FontWeight.bold),
                      ),
                    ),
                  ),
                  const SizedBox(height: 24),
                ],
              ),
            ),
    );
  }

  Widget _buildSectionHeader(String title) {
    return Text(
      title,
      style: const TextStyle(
        fontSize: 14,
        fontWeight: FontWeight.bold,
        color: secondaryTextColor,
        letterSpacing: 0.5,
      ),
    );
  }

  Widget _buildTextField({
    required String label,
    required String placeholder,
    required Function(String) onChanged,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          label,
          style: const TextStyle(
            color: secondaryTextColor,
            fontSize: 12,
            fontWeight: FontWeight.w500,
          ),
        ),
        const SizedBox(height: 8),
        TextFormField(
          style: const TextStyle(color: textColor),
          decoration: InputDecoration(
            hintText: placeholder,
            hintStyle: TextStyle(color: secondaryTextColor.withOpacity(0.5)),
            filled: true,
            fillColor: inputFillColor,
            contentPadding:
                const EdgeInsets.symmetric(horizontal: 16, vertical: 14),
            border: OutlineInputBorder(
              borderRadius: BorderRadius.circular(6),
              borderSide: const BorderSide(color: borderColor),
            ),
            enabledBorder: OutlineInputBorder(
              borderRadius: BorderRadius.circular(6),
              borderSide: const BorderSide(color: borderColor),
            ),
            focusedBorder: OutlineInputBorder(
              borderRadius: BorderRadius.circular(6),
              borderSide: const BorderSide(color: primaryColor),
            ),
          ),
          onChanged: onChanged,
        ),
      ],
    );
  }

  Widget _buildSelectionRow({
    required Map<String, dynamic>? serviceValue,
    required String serviceLabel,
    required Map<String, dynamic>? typeValue,
    required String typeLabel,
    required List<dynamic> services,
    required Function(Map<String, dynamic>?) onServiceChanged,
    required Function(Map<String, dynamic>?) onTypeChanged,
    required List Function(Map<String, dynamic>) getTypes,
  }) {
    // For mobile, we might want to stack them if screen is narrow, 
    // but Row with Expanded works well for landscape/tablets or concise labels.
    // Let's use Column for mobile-friendliness or Row if space permits.
    // Given the screenshots, they are side-by-side. 
    // We'll use a LayoutBuilder or just Row for now.
    
    return Row(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        // Service Dropdown
        Expanded(
          flex: 1,
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                serviceLabel,
                style: const TextStyle(
                  color: secondaryTextColor,
                  fontSize: 12,
                  fontWeight: FontWeight.w500,
                ),
              ),
              const SizedBox(height: 8),
              Container(
                padding: const EdgeInsets.symmetric(horizontal: 12),
                decoration: BoxDecoration(
                  color: inputFillColor,
                  borderRadius: BorderRadius.circular(6),
                  border: Border.all(color: borderColor),
                ),
                child: DropdownButtonHideUnderline(
                  child: DropdownButton<Map<String, dynamic>>(
                    value: serviceValue,
                    hint: Text('Select Service',
                        style: TextStyle(
                            color: secondaryTextColor.withOpacity(0.5))),
                    dropdownColor: cardColor,
                    style: const TextStyle(color: textColor),
                    isExpanded: true,
                    icon: const Icon(Icons.arrow_drop_down,
                        color: secondaryTextColor),
                    items: services
                        .map<DropdownMenuItem<Map<String, dynamic>>>((s) {
                      return DropdownMenuItem(
                        value: s,
                        child: Row(
                          children: [
                            Icon(_getServiceIcon(s['name']),
                                color: textColor, size: 18),
                            const SizedBox(width: 8),
                            Text(
                              s['name'].toString().toUpperCase(),
                              overflow: TextOverflow.ellipsis,
                            ),
                          ],
                        ),
                      );
                    }).toList(),
                    onChanged: onServiceChanged,
                  ),
                ),
              ),
            ],
          ),
        ),
        const SizedBox(width: 16),
        // Type Dropdown
        Expanded(
          flex: 1,
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                typeLabel,
                style: const TextStyle(
                  color: secondaryTextColor,
                  fontSize: 12,
                  fontWeight: FontWeight.w500,
                ),
              ),
              const SizedBox(height: 8),
              Container(
                padding: const EdgeInsets.symmetric(horizontal: 12),
                decoration: BoxDecoration(
                  color: inputFillColor,
                  borderRadius: BorderRadius.circular(6),
                  border: Border.all(color: borderColor),
                ),
                child: DropdownButtonHideUnderline(
                  child: DropdownButton<Map<String, dynamic>>(
                    value: typeValue,
                    hint: Text('Select Type',
                        style: TextStyle(
                            color: secondaryTextColor.withOpacity(0.5))),
                    dropdownColor: cardColor,
                    style: const TextStyle(color: textColor),
                    isExpanded: true,
                    icon: const Icon(Icons.arrow_drop_down,
                        color: secondaryTextColor),
                    items: serviceValue == null
                        ? []
                        : getTypes(serviceValue!)
                            .map<DropdownMenuItem<Map<String, dynamic>>>((t) {
                            return DropdownMenuItem(
                              value: t,
                              child: Text(
                                t['description'] ?? t['name'],
                                overflow: TextOverflow.ellipsis,
                              ),
                            );
                          }).toList(),
                    onChanged: onTypeChanged,
                  ),
                ),
              ),
            ],
          ),
        ),
      ],
    );
  }

  Future<void> _createArea() async {
    if (_selectedAction == null || _selectedReaction == null) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
            content: Text('Please select both Action and Reaction')),
      );
      return;
    }

    if (_areaName.isEmpty) {
      _areaName =
          '${_selectedAction!['name']} -> ${_selectedReaction!['name']}';
    }

    setState(() => _isLoading = true);

    try {
      final authProvider =
          Provider.of<AuthProvider>(context, listen: false);
      final serverProvider =
          Provider.of<ServerProvider>(context, listen: false);
      final apiService = ApiService(serverProvider.serverUrl);

      final payload = {
        'name': _areaName,
        'description': _description,
        'action_service': _selectedActionService!['name'],
        'action_type': _selectedAction!['name'],
        'reaction_service': _selectedReactionService!['name'],
        'reaction_type': _selectedReaction!['name'],
        'action_config': _actionParams,
        'reaction_config': _reactionParams,
      };

      await apiService.createArea(authProvider.token!, payload);

      if (mounted) {
        Navigator.pop(context, true);
        ScaffoldMessenger.of(context)
            .showSnackBar(const SnackBar(content: Text('AREA Created!')));
      }
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context)
            .showSnackBar(SnackBar(content: Text('Error: $e')));
      }
    } finally {
      if (mounted) setState(() => _isLoading = false);
    }
  }

  IconData _getServiceIcon(String name) {
    switch (name.toLowerCase()) {
      case 'google':
        return Icons.g_mobiledata;
      case 'linkedin':
        return Icons.business;
      case 'dropbox':
        return Icons.cloud;
      case 'github':
        return Icons.code;
      case 'timer':
        return Icons.timer;
      default:
        return Icons.extension;
    }
  }
}
