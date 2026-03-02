import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'package:webview_flutter/webview_flutter.dart';

class LinkedInAuthScreen extends StatefulWidget {
  final String clientId;
  final String redirectUri;
  final String state;

  const LinkedInAuthScreen({
    super.key,
    required this.clientId,
    required this.redirectUri,
    this.state = 'random_state_string', // Should be generated randomly in prod
  });

  @override
  State<LinkedInAuthScreen> createState() => _LinkedInAuthScreenState();
}

class _LinkedInAuthScreenState extends State<LinkedInAuthScreen> {
  late final WebViewController _controller;
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();

    final String authUrl =
        'https://www.linkedin.com/oauth/v2/authorization?response_type=code&client_id=${widget.clientId}&redirect_uri=${Uri.encodeComponent(widget.redirectUri)}&scope=openid%20profile%20email%20w_member_social';

    _controller = WebViewController();
    if (!kIsWeb) {
      _controller.setJavaScriptMode(JavaScriptMode.unrestricted);
      _controller.setNavigationDelegate(
        NavigationDelegate(
          onPageStarted: (String url) {
            setState(() {
              _isLoading = true;
            });
          },
          onPageFinished: (String url) {
            setState(() {
              _isLoading = false;
            });
          },
          onNavigationRequest: (NavigationRequest request) {
            if (request.url.startsWith(widget.redirectUri)) {
              final uri = Uri.parse(request.url);
              final code = uri.queryParameters['code'];
              final error = uri.queryParameters['error'];

              if (code != null) {
                 if (mounted) {
                   Navigator.of(context).pop(code);
                 }
                return NavigationDecision.prevent;
              } else if (error != null) {
                 if (mounted) {
                   Navigator.of(context).pop(null); // Or return error
                 }
                return NavigationDecision.prevent;
              }
            }
            return NavigationDecision.navigate;
          },
        ),
      );
    }
    _controller.loadRequest(Uri.parse(authUrl));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Connect with LinkedIn'),
      ),
      body: Stack(
        children: [
          WebViewWidget(controller: _controller),
          if (_isLoading)
            const Center(
              child: CircularProgressIndicator(),
            ),
        ],
      ),
    );
  }
}
