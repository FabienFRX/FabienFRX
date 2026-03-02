import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class ServerProvider with ChangeNotifier {
  final SharedPreferences _prefs;
  String _serverUrl = '';

  ServerProvider(this._prefs) {
    _loadServerUrl();
  }

  String get serverUrl => _serverUrl;

  Future<void> _loadServerUrl() async {
    _serverUrl = _prefs.getString('server_url') ?? 'http://10.0.2.2:8080';
    notifyListeners();
  }

  Future<void> setServerUrl(String url) async {
    _serverUrl = url;
    await _prefs.setString('server_url', url);
    notifyListeners();
  }

  bool get isConfigured => _serverUrl.isNotEmpty;
}
