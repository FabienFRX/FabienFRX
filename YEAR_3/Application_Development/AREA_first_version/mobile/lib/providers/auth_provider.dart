import 'package:flutter/material.dart';
import 'package:google_sign_in/google_sign_in.dart';
import '../services/api_service.dart';

class AuthProvider with ChangeNotifier {
  String? _token;
  Map<String, dynamic>? _user;
  bool _isAuthenticated = false;

  final GoogleSignIn _googleSignIn = GoogleSignIn(
    scopes: [
      'email',
      'https://www.googleapis.com/auth/userinfo.profile',
    ],
  );

  bool get isAuthenticated => _isAuthenticated;
  String? get token => _token;
  Map<String, dynamic>? get user => _user;

  Future<void> login(String token, Map<String, dynamic> user) async {
    _token = token;
    _user = user;
    _isAuthenticated = true;
    notifyListeners();
  }

  // Google Sign In
  Future<Map<String, dynamic>?> signInWithGoogle(ApiService apiService) async {
    try {
      final GoogleSignInAccount? googleUser = await _googleSignIn.signIn();
      if (googleUser == null) return null; // User canceled

      final GoogleSignInAuthentication googleAuth = await googleUser.authentication;
      final String? accessToken = googleAuth.accessToken;

      if (accessToken != null) {
        // Send to backend
        final result = await apiService.googleLogin(accessToken);
        await login(result['token'], result['user']);
        return result;
      }
    } catch (error) {
      print('Google Sign In Error: $error');
      rethrow;
    }
    return null;
  }

  // Connect Google Account (when already logged in)
  Future<void> connectGoogleAccount(ApiService apiService) async {
    try {
      // Force sign in to ensure we get a fresh token for the right account
      final GoogleSignInAccount? googleUser = await _googleSignIn.signIn();
      if (googleUser == null) return;

      final GoogleSignInAuthentication googleAuth = await googleUser.authentication;
      final String? accessToken = googleAuth.accessToken;

      if (accessToken != null && _token != null) {
        await apiService.connectGoogleService(_token!, accessToken);
        notifyListeners(); // Refresh UI if needed
      }
    } catch (error) {
      print('Connect Google Error: $error');
      rethrow;
    }
  }

  Future<void> logout(ApiService apiService) async {
    // Try to logout from server
    if (_token != null) {
      try {
        await apiService.logout(_token!);
      } catch (e) {
        print('Server logout error: $e');
      }
    }
    
    // Google Sign Out
    try {
      if (await _googleSignIn.isSignedIn()) {
        await _googleSignIn.signOut();
      }
    } catch (e) {
      print('Google sign out error: $e');
    }

    _token = null;
    _user = null;
    _isAuthenticated = false;
    notifyListeners();
  }
}
