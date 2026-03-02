import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'package:webview_flutter/webview_flutter.dart';

class GitHubAuthScreen extends StatefulWidget {
  final String clientId;
  final String redirectUri;

  const GitHubAuthScreen({
    super.key,
    required this.clientId,
    required this.redirectUri,
  });

  @override
  State<GitHubAuthScreen> createState() => _GitHubAuthScreenState();
}

class _GitHubAuthScreenState extends State<GitHubAuthScreen> {
  late final WebViewController _controller;

  @override
  void initState() {
    super.initState();
    _controller = WebViewController();
    if (!kIsWeb) {
      _controller.setJavaScriptMode(JavaScriptMode.unrestricted);
      _controller.setNavigationDelegate(
        NavigationDelegate(
          onNavigationRequest: (NavigationRequest request) {
            if (request.url.startsWith(widget.redirectUri)) {
              final uri = Uri.parse(request.url);
              final code = uri.queryParameters['code'];
              if (code != null) {
                Navigator.pop(context, code);
              } else {
                Navigator.pop(context); // Canceled or error
              }
              return NavigationDecision.prevent;
            }
            return NavigationDecision.navigate;
          },
        ),
      );
    }
    _controller.loadRequest(
      Uri.https('github.com', '/login/oauth/authorize', {
        'client_id': widget.clientId,
        'redirect_uri': widget.redirectUri,
        'scope': 'repo user read:org',
      }),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('GitHub Login'),
        backgroundColor: Colors.black, // GitHub style
        foregroundColor: Colors.white,
      ),
      body: WebViewWidget(controller: _controller),
    );
  }
}
