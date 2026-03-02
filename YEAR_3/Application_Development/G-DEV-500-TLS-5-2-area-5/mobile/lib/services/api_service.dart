import 'dart:convert';
import 'package:http/http.dart' as http;

class ApiService {
  final String baseUrl;

  ApiService(this.baseUrl);

  // Get /about.json - Liste tous les services disponibles (public)
  Future<Map<String, dynamic>> getAbout() async {
    final response = await http.get(Uri.parse('$baseUrl/about.json'));
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to load about.json: ${response.statusCode}');
  }

  // Get /services/ - Liste des services disponibles avec actions/réactions (authentifié)
  Future<Map<String, dynamic>> getServices(String token) async {
    final response = await http.get(
      Uri.parse('$baseUrl/services/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to load services: ${response.statusCode}');
  }

  // Auth endpoints - Compatible avec Django backend
  Future<Map<String, dynamic>> register(
      String email, String password, String firstName, String lastName) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/signup/'),
      headers: {'Content-Type': 'application/json'},
      body: json.encode({
        'email': email,
        'password': password,
        'first_name': firstName,
        'last_name': lastName,
      }),
    );
    if (response.statusCode == 200 || response.statusCode == 201) {
      return json.decode(response.body);
    }
    throw Exception('Registration failed: ${response.body}');
  }

  Future<Map<String, dynamic>> login(String email, String password) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/login/'),
      headers: {'Content-Type': 'application/json'},
      body: json.encode({
        'email': email,
        'password': password,
      }),
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Login failed: ${response.body}');
  }

  // Google Login - Exchange access token for JWT
  Future<Map<String, dynamic>> googleLogin(String? authCode, {String? accessToken}) async {
    final Map<String, dynamic> body = {};
    if (authCode != null) {
      body['authorization_code'] = authCode;
    }
    if (accessToken != null) {
      body['access_token'] = accessToken;
    }

    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/google/login/'),
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
      },
      body: jsonEncode(body),
    );

    if (response.statusCode == 200) {
      return jsonDecode(response.body);
    } else {
      throw Exception('Failed to login with Google: ${response.body}');
    }
  } // LinkedIn Login
  Future<Map<String, dynamic>> linkedinLogin(String code, String redirectUri) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/linkedin/login/'),
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
      },
      body: json.encode({
        'code': code,
        'redirect_uri': redirectUri,
      }),
    );
     if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('LinkedIn Login failed: ${response.body}');
  }

  // GitHub Login
  Future<Map<String, dynamic>> githubLogin(String code, String redirectUri) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/github/login/'),
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
      },
      body: json.encode({
        'code': code,
        'redirect_uri': redirectUri,
      }),
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('GitHub Login failed: ${response.body}');
  }

  // GitLab Login
  Future<Map<String, dynamic>> gitlabLogin(String code, String redirectUri) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/gitlab/login/'),
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
      },
      body: json.encode({
        'code': code,
        'redirect_uri': redirectUri,
      }),
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('GitLab Login failed: ${response.body}');
  }

  Future<void> logout(String token) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/logout/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode != 200) {
      throw Exception('Logout failed: ${response.statusCode}');
    }
  }

  // Profile endpoint
  Future<Map<String, dynamic>> getProfile(String token) async {
    final response = await http.get(
      Uri.parse('$baseUrl/api/auth/profile/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to load profile: ${response.statusCode}');
  }

  // User services (OAuth2 connected services)
  Future<Map<String, dynamic>> getUserServices(String token) async {
    final response = await http.get(
      Uri.parse('$baseUrl/api/auth/services/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to load user services: ${response.statusCode}');
  }

  // Connect Google Service
  Future<Map<String, dynamic>> connectGoogleService(String token, String accessToken) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/google/connect/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
      body: json.encode({
        'access_token': accessToken,
      }),
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to connect Google service: ${response.body}');
  }

  // Connect LinkedIn Service
  Future<Map<String, dynamic>> connectLinkedInService(String token, String code, String redirectUri) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/linkedin/connect/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
      body: json.encode({
        'code': code,
        'redirect_uri': redirectUri,
      }),
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to connect LinkedIn service: ${response.body}');
  }

  // Connect Dropbox Service
  Future<Map<String, dynamic>> connectDropboxService(String token, String code, String redirectUri) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/dropbox/connect/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
      body: json.encode({
        'code': code,
        'redirect_uri': redirectUri,
      }),
    );
     if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to connect Dropbox service: ${response.body}');
  }

  // Connect GitHub Service
  Future<Map<String, dynamic>> connectGitHubService(String token, String code, String redirectUri) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/github/connect/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
      body: json.encode({
        'code': code,
        'redirect_uri': redirectUri,
      }),
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to connect GitHub service: ${response.body}');
  }

  // Connect GitLab Service
  Future<Map<String, dynamic>> connectGitLabService(String token, String code, String redirectUri) async {
    final response = await http.post(
      Uri.parse('$baseUrl/api/auth/gitlab/connect/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
      body: json.encode({
        'code': code,
        'redirect_uri': redirectUri,
      }),
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to connect GitLab service: ${response.body}');
  }

  // Disconnect Service
  Future<void> disconnectService(String token, int serviceId) async {
    // Note: The backend uses DELETE on /services/ for this, or specific endpoint
    // Based on authentication_urls.py: path('services/<int:service_id>/', main_views.UserServiceDetailView.as_view(), name='disconnect_service'),
    // Actually authentication_urls.py has:
    // path('services/', auth_views.UserServicesView.as_view(), name='user_services'),
    // And UserServicesView has a delete method:
    /*
      def delete(self, request):
          service_id = request.data.get('service_id')
    */
    // But there's also: path('services/<int:service_id>/', main_views.UserServiceDetailView.as_view(), name='disconnect_service')
    // Let's use the one that seems most standard REST, or the one from UserServicesView if it's better.
    // UserServicesView.delete expects 'service_id' in body.
    
    final response = await http.delete(
      Uri.parse('$baseUrl/api/auth/services/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
      body: json.encode({
        'service_id': serviceId,
      }),
    );

    if (response.statusCode != 200) {
       throw Exception('Failed to disconnect service: ${response.body}');
    }
  }

  // AREAs endpoints
  Future<Map<String, dynamic>> getAreas(String token) async {
    final response = await http.get(
      Uri.parse('$baseUrl/areas/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to load areas: ${response.statusCode}');
  }

  Future<Map<String, dynamic>> createArea(
      String token, Map<String, dynamic> areaData) async {
    final response = await http.post(
      Uri.parse('$baseUrl/areas/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
      body: json.encode(areaData),
    );
    if (response.statusCode == 200 || response.statusCode == 201) {
      return json.decode(response.body);
    }
    throw Exception('Failed to create area: ${response.body}');
  }

  Future<Map<String, dynamic>> updateArea(
      String token, int areaId, Map<String, dynamic> data) async {
    final response = await http.patch(
      Uri.parse('$baseUrl/areas/$areaId/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
      body: json.encode(data),
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to update area: ${response.body}');
  }

  Future<void> deleteArea(String token, int areaId) async {
    final response = await http.delete(
      Uri.parse('$baseUrl/areas/$areaId/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode != 200 && response.statusCode != 204) {
      throw Exception('Failed to delete area: ${response.statusCode}');
    }
  }
  // Workflow Executor
  Future<Map<String, dynamic>> getExecutorStatus(String token) async {
    final response = await http.get(
      Uri.parse('$baseUrl/executor/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    // Return unknown if failed, or throw. For now consistent with others:
    throw Exception('Failed to get executor status: ${response.statusCode}');
  }

  Future<Map<String, dynamic>> startExecutor(String token) async {
    final response = await http.post(
      Uri.parse('$baseUrl/executor/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to start executor: ${response.body}');
  }

  Future<Map<String, dynamic>> stopExecutor(String token) async {
    final response = await http.delete(
      Uri.parse('$baseUrl/executor/'),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Token $token',
      },
    );
    if (response.statusCode == 200) {
      return json.decode(response.body);
    }
    throw Exception('Failed to stop executor: ${response.body}');
  }
}
