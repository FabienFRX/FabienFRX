import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'package:provider/provider.dart';
import '../providers/auth_provider.dart';
import '../providers/server_provider.dart';
import '../services/api_service.dart';
import 'linkedin_auth_screen.dart';
import 'github_auth_screen.dart';
import 'gitlab_auth_screen.dart';
import '../config/secrets.dart';

class LoginScreen extends StatefulWidget {
  const LoginScreen({super.key});

  @override
  State<LoginScreen> createState() => _LoginScreenState();
}

class _LoginScreenState extends State<LoginScreen> {
  final _formKey = GlobalKey<FormState>();
  final _emailController = TextEditingController();
  final _passwordController = TextEditingController();
  bool _isLoading = false;
  bool _obscurePassword = true;

  @override
  void dispose() {
    _emailController.dispose();
    _passwordController.dispose();
    super.dispose();
  }

  Future<void> _login() async {
    if (!_formKey.currentState!.validate()) return;

    setState(() => _isLoading = true);

    try {
      final serverProvider =
          Provider.of<ServerProvider>(context, listen: false);
      final authProvider = Provider.of<AuthProvider>(context, listen: false);
      final apiService = ApiService(serverProvider.serverUrl);

      final response = await apiService.login(
        _emailController.text.trim(),
        _passwordController.text,
      );

      await authProvider.login(response['token'], response['user']);

      if (!mounted) return;
      Navigator.pushReplacementNamed(context, '/home');
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

  @override
  Widget build(BuildContext context) {
    // Dark theme colors to match Web
    const backgroundColor = Color(0xFF111827); // gray-900
    const cardColor = Color(0xFF1F2937); // gray-800
    const textColor = Color(0xFFE5E7EB); // gray-200
    const secondaryTextColor = Color(0xFF9CA3AF); // gray-400
    const inputFillColor = Color(0xFF374151); // gray-700
    const borderColor = Color(0xFF4B5563); // gray-600
    const buttonColor = Color(0xFF6366F1); // indigo-500
    
    return Scaffold(
      backgroundColor: backgroundColor,
      appBar: AppBar(
        backgroundColor: Colors.transparent,
        elevation: 0,
        actions: [
          IconButton(
            icon: const Icon(Icons.settings, color: textColor),
            onPressed: () => Navigator.pushNamed(context, '/settings'),
          ),
        ],
      ),
      body: Center(
        child: SingleChildScrollView(
          child: Container(
            constraints: const BoxConstraints(maxWidth: 400),
            padding: const EdgeInsets.all(32.0),
            child: Form(
              key: _formKey,
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                crossAxisAlignment: CrossAxisAlignment.stretch,
                children: [
                  // Titre
                  Text(
                    'Login',
                    style: Theme.of(context).textTheme.headlineMedium?.copyWith(
                          fontWeight: FontWeight.w600,
                          color: textColor,
                        ),
                    textAlign: TextAlign.center,
                  ),
                  const SizedBox(height: 8),
                  Text(
                    'Login to access your AREA account',
                    style: const TextStyle(
                      color: secondaryTextColor,
                      fontSize: 14,
                    ),
                    textAlign: TextAlign.center,
                  ),
                  const SizedBox(height: 40),

                  // Email Field
                  Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      const Text(
                        'Email Address',
                        style: TextStyle(
                          fontSize: 13,
                          color: secondaryTextColor,
                          fontWeight: FontWeight.w500,
                        ),
                      ),
                      const SizedBox(height: 8),
                      TextFormField(
                        controller: _emailController,
                        enabled: !_isLoading,
                        style: const TextStyle(color: Colors.white),
                        decoration: InputDecoration(
                          hintText: 'you@company.com',
                          hintStyle: const TextStyle(color: secondaryTextColor),
                          filled: true,
                          fillColor: inputFillColor,
                          border: OutlineInputBorder(
                            borderRadius: BorderRadius.circular(8),
                            borderSide: const BorderSide(color: borderColor),
                          ),
                          enabledBorder: OutlineInputBorder(
                            borderRadius: BorderRadius.circular(8),
                            borderSide: const BorderSide(color: borderColor),
                          ),
                          focusedBorder: OutlineInputBorder(
                            borderRadius: BorderRadius.circular(8),
                            borderSide: const BorderSide(
                                color: buttonColor, width: 2),
                          ),
                          contentPadding: const EdgeInsets.symmetric(
                            horizontal: 16,
                            vertical: 12,
                          ),
                        ),
                        keyboardType: TextInputType.emailAddress,
                        validator: (value) {
                          if (value == null || value.isEmpty) {
                            return 'Veuillez entrer votre email';
                          }
                          return null;
                        },
                      ),
                    ],
                  ),
                  const SizedBox(height: 20),

                  // Password Field
                  Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        children: [
                          const Text(
                            'Password',
                            style: TextStyle(
                              fontSize: 13,
                              color: secondaryTextColor,
                              fontWeight: FontWeight.w500,
                            ),
                          ),
                          TextButton(
                            onPressed: () {},
                            style: TextButton.styleFrom(
                              padding: EdgeInsets.zero,
                              minimumSize: const Size(0, 0),
                              tapTargetSize: MaterialTapTargetSize.shrinkWrap,
                            ),
                            child: const Text(
                              'Forgot password?',
                              style: TextStyle(
                                fontSize: 13,
                                color: secondaryTextColor,
                              ),
                            ),
                          ),
                        ],
                      ),
                      const SizedBox(height: 8),
                      TextFormField(
                        controller: _passwordController,
                        enabled: !_isLoading,
                        style: const TextStyle(color: Colors.white),
                        decoration: InputDecoration(
                          hintText: 'Your Password',
                          hintStyle: const TextStyle(color: secondaryTextColor),
                          filled: true,
                          fillColor: inputFillColor,
                          border: OutlineInputBorder(
                            borderRadius: BorderRadius.circular(8),
                            borderSide: const BorderSide(color: borderColor),
                          ),
                          enabledBorder: OutlineInputBorder(
                            borderRadius: BorderRadius.circular(8),
                            borderSide: const BorderSide(color: borderColor),
                          ),
                          focusedBorder: OutlineInputBorder(
                            borderRadius: BorderRadius.circular(8),
                            borderSide: const BorderSide(
                                color: buttonColor, width: 2),
                          ),
                          contentPadding: const EdgeInsets.symmetric(
                            horizontal: 16,
                            vertical: 12,
                          ),
                          suffixIcon: IconButton(
                            icon: Icon(
                              _obscurePassword
                                  ? Icons.visibility_outlined
                                  : Icons.visibility_off_outlined,
                              color: secondaryTextColor,
                            ),
                            onPressed: () {
                              setState(
                                  () => _obscurePassword = !_obscurePassword);
                            },
                          ),
                        ),
                        obscureText: _obscurePassword,
                        validator: (value) {
                          if (value == null || value.isEmpty) {
                            return 'Veuillez entrer votre mot de passe';
                          }
                          return null;
                        },
                      ),
                    ],
                  ),
                  const SizedBox(height: 24),

                  // Login Button
                  ElevatedButton(
                    onPressed: _isLoading ? null : _login,
                    style: ElevatedButton.styleFrom(
                      backgroundColor: buttonColor,
                      foregroundColor: Colors.white,
                      padding: const EdgeInsets.symmetric(vertical: 16),
                      shape: RoundedRectangleBorder(
                        borderRadius: BorderRadius.circular(8),
                      ),
                      elevation: 0,
                      disabledBackgroundColor: buttonColor.withOpacity(0.5),
                    ),
                    child: _isLoading
                        ? const SizedBox(
                            height: 20,
                            width: 20,
                            child: CircularProgressIndicator(
                              strokeWidth: 2,
                              color: Colors.white,
                            ),
                          )
                        : const Text(
                            'Login',
                            style: TextStyle(
                              fontSize: 16,
                              fontWeight: FontWeight.w500,
                            ),
                          ),
                  ),

                  // Divider
                  const SizedBox(height: 24),
                  const Row(
                    children: [
                      Expanded(child: Divider(color: borderColor)),
                      Padding(
                        padding: EdgeInsets.symmetric(horizontal: 16),
                        child: Text(
                          'OR',
                          style: TextStyle(
                            color: secondaryTextColor,
                            fontSize: 13,
                          ),
                        ),
                      ),
                      Expanded(child: Divider(color: borderColor)),
                    ],
                  ),
                  const SizedBox(height: 24),

                  // Google Login Button
                  ElevatedButton(
                    onPressed: _isLoading
                        ? null
                        : () async {
                            setState(() => _isLoading = true);
                            try {
                              final authProvider = Provider.of<AuthProvider>(
                                  context,
                                  listen: false);
                              final serverProvider =
                                  Provider.of<ServerProvider>(context,
                                      listen: false);
                              final apiService =
                                  ApiService(serverProvider.serverUrl);

                              final result = await authProvider
                                  .signInWithGoogle(apiService);

                              if (!context.mounted) return;

                              if (result != null) {
                                Navigator.pushReplacementNamed(context, '/home');
                              }
                            } catch (e) {
                              if (!context.mounted) return;
                              ScaffoldMessenger.of(context).showSnackBar(
                                SnackBar(
                                    content: Text(
                                        'Google Sign In Error: ${e.toString()}')),
                              );
                            } finally {
                              if (mounted) {
                                setState(() => _isLoading = false);
                              }
                            }
                          },
                    style: ElevatedButton.styleFrom(
                      backgroundColor: Colors.white,
                      foregroundColor: Colors.black87,
                      padding: const EdgeInsets.symmetric(vertical: 12),
                      shape: RoundedRectangleBorder(
                        borderRadius: BorderRadius.circular(8),
                        side: BorderSide(color: Colors.grey[300]!),
                      ),
                      elevation: 0,
                    ),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        CustomPaint(
                          painter: _GoogleLogoPainter(),
                          size: const Size(20, 20),
                        ),
                        const SizedBox(width: 12),
                        const Text(
                          'Continue with Google',
                          style: TextStyle(
                            fontSize: 15,
                            fontWeight: FontWeight.w500,
                          ),
                        ),
                      ],
                    ),
                  ),

                  const SizedBox(height: 16),

                  // LinkedIn Login Button
                  _buildSocialButton(
                    onPressed: () async {
                      if (kIsWeb) {
                        ScaffoldMessenger.of(context).showSnackBar(
                           const SnackBar(content: Text('LinkedIn login is not supported on Web yet. Please use Google.')),
                        );
                        return;
                      }
                      
                      const String clientId = Secrets.linkedinClientId;
                      // Use a redirect URI that works for your mobile/web setup
                      // The backend must allow this URI
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

                      if (code == null || !context.mounted) return;

                      setState(() => _isLoading = true);
                      try {
                        final authProvider =
                            Provider.of<AuthProvider>(context, listen: false);
                        final serverProvider =
                            Provider.of<ServerProvider>(context, listen: false);
                          final apiService =
                              ApiService(serverProvider.serverUrl);

                          final result = await authProvider.signInWithLinkedIn(
                              apiService, code, redirectUri);

                          if (!context.mounted) return;

                          if (result != null) {
                            Navigator.pushReplacementNamed(context, '/home');
                          }
                        } catch (e) {
                          if (!context.mounted) return;
                          ScaffoldMessenger.of(context).showSnackBar(
                            SnackBar(content: Text('LinkedIn Sign In Error: ${e.toString()}')),
                          );
                        } finally {
                          if (mounted) {
                            setState(() => _isLoading = false);
                          }
                        }
                    },
                    icon: Container(
                      width: 20,
                      height: 20,
                      color: const Color(0xFF0077b5),
                      child: const Center(
                        child: Text(
                          'in',
                          style: TextStyle(
                            color: Colors.white,
                            fontWeight: FontWeight.bold,
                            fontSize: 14,
                          ),
                        ),
                      ),
                    ),
                    text: 'Continue with LinkedIn',
                    textColor: Colors.white,
                    backgroundColor: const Color(0xFF0077b5),
                    borderColor: Colors.transparent,
                  ),

                  const SizedBox(height: 16),

                  // GitHub Login Button
                  _buildSocialButton(
                    onPressed: () async {
                      if (kIsWeb) {
                        ScaffoldMessenger.of(context).showSnackBar(
                           const SnackBar(content: Text('GitHub login is not supported on Web yet. Please use Google.')),
                        );
                        return;
                      }

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

                      if (code == null || !context.mounted) return;

                      setState(() => _isLoading = true);
                      try {
                        final authProvider =
                            Provider.of<AuthProvider>(context, listen: false);
                        final serverProvider =
                            Provider.of<ServerProvider>(context, listen: false);
                        final apiService =
                            ApiService(serverProvider.serverUrl);

                        final result = await authProvider.signInWithGitHub(
                            apiService, code, redirectUri);

                        if (!context.mounted) return;

                        if (result != null) {
                          Navigator.pushReplacementNamed(context, '/home');
                        }
                      } catch (e) {
                        if (!context.mounted) return;
                        ScaffoldMessenger.of(context).showSnackBar(
                          SnackBar(content: Text('GitHub Sign In Error: ${e.toString()}')),
                        );
                      } finally {
                        if (mounted) {
                          setState(() => _isLoading = false);
                        }
                      }
                    },
                    icon: const Icon(Icons.code, color: Colors.white, size: 20),
                    text: 'Continue with GitHub',
                    textColor: Colors.white,
                    backgroundColor: const Color(0xFF24292e),
                    borderColor: Colors.transparent,
                  ),

                  const SizedBox(height: 16),

                  // GitLab Login Button
                  _buildSocialButton(
                    onPressed: () async {
                      if (kIsWeb) {
                        ScaffoldMessenger.of(context).showSnackBar(
                           const SnackBar(content: Text('GitLab login is not supported on Web yet. Please use Google.')),
                        );
                        return;
                      }

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

                      if (code == null || !context.mounted) return;

                      setState(() => _isLoading = true);
                      try {
                        final authProvider =
                            Provider.of<AuthProvider>(context, listen: false);
                        final serverProvider =
                            Provider.of<ServerProvider>(context, listen: false);
                        final apiService =
                            ApiService(serverProvider.serverUrl);

                        final result = await authProvider.signInWithGitLab(
                            apiService, code, redirectUri);

                        if (!context.mounted) return;

                        if (result != null) {
                          Navigator.pushReplacementNamed(context, '/home');
                        }
                      } catch (e) {
                        if (!context.mounted) return;
                        ScaffoldMessenger.of(context).showSnackBar(
                          SnackBar(content: Text('GitLab Sign In Error: ${e.toString()}')),
                        );
                      } finally {
                        if (mounted) {
                          setState(() => _isLoading = false);
                        }
                      }
                    },
                    icon: const Icon(Icons.code, color: Colors.white, size: 20), // Placeholder or GitLab icon
                    text: 'Continue with GitLab',
                    textColor: Colors.white,
                    backgroundColor: const Color(0xFFFC6D26), // GitLab orange
                    borderColor: Colors.transparent,
                  ),


                  const SizedBox(height: 24),

                  // Sign up link
                  Center(
                    child: Row(
                      mainAxisSize: MainAxisSize.min,
                      children: [
                        const Text(
                          'Don\'t have an account yet? ',
                          style: TextStyle(
                            color: secondaryTextColor,
                            fontSize: 13,
                          ),
                        ),
                        TextButton(
                          onPressed: () =>
                              Navigator.pushNamed(context, '/register'),
                          style: TextButton.styleFrom(
                            padding: EdgeInsets.zero,
                            minimumSize: const Size(0, 0),
                            tapTargetSize: MaterialTapTargetSize.shrinkWrap,
                          ),
                          child: const Text(
                            'Sign up',
                            style: TextStyle(
                              color: Color(0xFF6366F1), // indigo-500
                              fontSize: 13,
                              fontWeight: FontWeight.w500,
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildSocialButton({
    required VoidCallback onPressed,
    required Widget icon,
    required String text,
    required Color textColor,
    required Color backgroundColor,
    required Color borderColor,
  }) {
    // Determine hover color logic if you want (for mobile it's splash)
    return ElevatedButton(
      onPressed: _isLoading ? null : onPressed,
      style: ElevatedButton.styleFrom(
        backgroundColor: backgroundColor,
        foregroundColor: textColor,
        surfaceTintColor: Colors.transparent,
        padding: const EdgeInsets.symmetric(vertical: 12),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(8),
          side: BorderSide(color: borderColor),
        ),
        elevation: 0,
      ),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          SizedBox(
            width: 20,
            height: 20,
            child: icon,
          ),
          const SizedBox(width: 12),
          Text(
            text,
            style: TextStyle(
              color: textColor,
              fontSize: 15,
              fontWeight: FontWeight.w500,
            ),
          ),
        ],
      ),
    );
  }
}

// Custom painter pour le logo Google
class _GoogleLogoPainter extends CustomPainter {
  @override
  void paint(Canvas canvas, Size size) {
    // Basic Google G
    // ... (Keep existing implementation or improve)
    final paint = Paint()..style = PaintingStyle.fill;
    // Blue
    paint.color = const Color(0xFF4285F4);
    canvas.drawCircle(
        Offset(size.width * 0.5, size.height * 0.5), size.width * 0.5, paint);
    // White G shape
    paint.color = Colors.white;
    canvas.drawCircle(
        Offset(size.width * 0.5, size.height * 0.5), size.width * 0.3, paint);
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) => false;
}
