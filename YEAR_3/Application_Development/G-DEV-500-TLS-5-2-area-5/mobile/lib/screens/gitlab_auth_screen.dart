import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'package:webview_flutter/webview_flutter.dart';

class GitLabAuthScreen extends StatefulWidget {
  final String clientId;
  final String redirectUri;

  const GitLabAuthScreen({
    super.key,
    required this.clientId,
    required this.redirectUri,
  });

  @override
  State<GitLabAuthScreen> createState() => _GitLabAuthScreenState();
}

class _GitLabAuthScreenState extends State<GitLabAuthScreen> {
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
      Uri.https('gitlab.com', '/oauth/authorize', {
        'client_id': widget.clientId,
        'redirect_uri': widget.redirectUri,
        'response_type': 'code',
        'scope': 'api read_user', // 'api' grants full access, including creating issues
      }),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('GitLab Login'),
        backgroundColor: const Color(0xFFFC6D26), // GitLab Orange
        foregroundColor: Colors.white,
      ),
      body: WebViewWidget(controller: _controller),
    );
  }
}
