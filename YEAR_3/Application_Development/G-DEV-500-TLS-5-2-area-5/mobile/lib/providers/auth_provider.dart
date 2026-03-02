import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart' show kIsWeb;
import 'package:google_sign_in/google_sign_in.dart';
import '../services/api_service.dart';
import '../config/secrets.dart';

class AuthProvider with ChangeNotifier {
  String? _token;
  Map<String, dynamic>? _user;
  bool _isAuthenticated = false;

  final GoogleSignIn _googleSignIn = GoogleSignIn(
    scopes: [
      'email',
      'profile',
      'https://www.googleapis.com/auth/userinfo.profile',
      'https://www.googleapis.com/auth/gmail.readonly',
      'https://www.googleapis.com/auth/gmail.send',
      'https://www.googleapis.com/auth/gmail.modify',
      'https://www.googleapis.com/auth/calendar',
      'https://www.googleapis.com/auth/drive',
    ],
    serverClientId: kIsWeb ? null : Secrets.googleClientId,
    clientId: Secrets.googleClientId,
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

      final String? authCode = googleUser.serverAuthCode;
      
      if (authCode != null) {
        // Mobile / Server Auth Code flow
        final result = await apiService.googleLogin(authCode);
        await login(result['token'], result['user']);
        return result;
      } else {
        // Web / Implicit flow fallback
        final GoogleSignInAuthentication googleAuth = await googleUser.authentication;
        final String? accessToken = googleAuth.accessToken;
        
        if (accessToken != null) {
          final result = await apiService.googleLogin(null, accessToken: accessToken);
          await login(result['token'], result['user']);
          return result;
        } else {
          debugPrint('Google Sign In Error: Both serverAuthCode and accessToken are null');
        }
      }
    } catch (error) {
      debugPrint('Google Sign In Error: $error');
      rethrow;
    }
    return null;
  }

  // LinkedIn Sign In
  Future<Map<String, dynamic>?> signInWithLinkedIn(ApiService apiService, String code, String redirectUri) async {
    try {
      final result = await apiService.linkedinLogin(code, redirectUri);
      await login(result['token'], result['user']);
      return result;
    } catch (error) {
      debugPrint('LinkedIn Sign In Error: $error');
      rethrow;
    }
  }

  // GitHub Sign In
  Future<Map<String, dynamic>?> signInWithGitHub(ApiService apiService, String code, String redirectUri) async {
    try {
      final result = await apiService.githubLogin(code, redirectUri);
      await login(result['token'], result['user']);
      return result;
    } catch (error) {
      debugPrint('GitHub Sign In Error: $error');
      rethrow;
    }
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
      debugPrint('Connect Google Error: $error');
      rethrow;
    }
  }

  // Connect LinkedIn Account
  Future<void> connectLinkedIn(ApiService apiService, String code, String redirectUri) async {
    try {
      if (_token != null) {
        await apiService.connectLinkedInService(_token!, code, redirectUri);
        notifyListeners();
      }
    } catch (error) {
      debugPrint('Connect LinkedIn Error: $error');
      rethrow;
    }
  }

  // Connect Dropbox Account
  Future<void> connectDropbox(ApiService apiService, String code, String redirectUri) async {
    try {
      if (_token != null) {
        await apiService.connectDropboxService(_token!, code, redirectUri);
        notifyListeners();
      }
    } catch (error) {
      debugPrint('Connect Dropbox Error: $error');
      rethrow;
    }
  }

  // Connect GitHub Account
  Future<void> connectGitHub(ApiService apiService, String code, String redirectUri) async {
    try {
      if (_token != null) {
        await apiService.connectGitHubService(_token!, code, redirectUri);
        notifyListeners();
      }
    } catch (error) {
      debugPrint('Connect GitHub Error: $error');
      rethrow;
    }
  }

  // GitLab Sign In
  Future<Map<String, dynamic>?> signInWithGitLab(ApiService apiService, String code, String redirectUri) async {
    try {
      final result = await apiService.gitlabLogin(code, redirectUri);
      await login(result['token'], result['user']);
      return result;
    } catch (error) {
      debugPrint('GitLab Sign In Error: $error');
      rethrow;
    }
  }

  // Connect GitLab Account
  Future<void> connectGitLab(ApiService apiService, String code, String redirectUri) async {
    try {
      if (_token != null) {
        await apiService.connectGitLabService(_token!, code, redirectUri);
        notifyListeners();
      }
    } catch (error) {
      debugPrint('Connect GitLab Error: $error');
      rethrow;
    }
  }

  Future<void> logout(ApiService apiService) async {
    // Try to logout from server
    if (_token != null) {
      try {
        await apiService.logout(_token!);
      } catch (e) {
        debugPrint('Server logout error: $e');
      }
    }
    
    // Google Sign Out
    try {
      if (await _googleSignIn.isSignedIn()) {
        await _googleSignIn.signOut();
      }
    } catch (e) {
      debugPrint('Google sign out error: $e');
    }

    _token = null;
    _user = null;
    _isAuthenticated = false;
    notifyListeners();
  }
}
