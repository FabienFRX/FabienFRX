import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:flutter/foundation.dart' show kIsWeb;

class ServerProvider with ChangeNotifier {
  final SharedPreferences _prefs;
  String _serverUrl = '';

  ServerProvider(this._prefs) {
    _loadServerUrl();
  }

  String get serverUrl => _serverUrl;

  Future<void> _loadServerUrl() async {
    String defaultUrl = 'http://localhost:8080';
    try {
       // kIsWeb is available from flutter/foundation.dart
       if (kIsWeb) {
         defaultUrl = 'http://localhost:8080';
       } else {
         // For Android Emulator, use 10.0.2.2 to access host localhost
         // For iOS Simulator, localhost usually works, but explicit IP is safer.
         // We will default to 10.0.2.2 for mobile dev
         defaultUrl = 'http://10.0.2.2:8080';
       }
    } catch (_) {}
    
    _serverUrl = _prefs.getString('server_url') ?? defaultUrl;
    notifyListeners();
  }

  Future<void> setServerUrl(String url) async {
    _serverUrl = url;
    await _prefs.setString('server_url', url);
    notifyListeners();
  }

  bool get isConfigured => _serverUrl.isNotEmpty;
}
