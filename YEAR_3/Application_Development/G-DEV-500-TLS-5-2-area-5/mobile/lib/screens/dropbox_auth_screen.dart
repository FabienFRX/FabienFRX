import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'package:webview_flutter/webview_flutter.dart';

class DropboxAuthScreen extends StatefulWidget {
  final String appKey;
  final String redirectUri;

  const DropboxAuthScreen({
    super.key,
    required this.appKey,
    required this.redirectUri,
  });

  @override
  State<DropboxAuthScreen> createState() => _DropboxAuthScreenState();
}

class _DropboxAuthScreenState extends State<DropboxAuthScreen> {
  late final WebViewController _controller;
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();

    final String authUrl =
        'https://www.dropbox.com/oauth2/authorize?client_id=${widget.appKey}&response_type=code&redirect_uri=${Uri.encodeComponent(widget.redirectUri)}';

    _controller = WebViewController();
    if (!kIsWeb) {
      _controller.setJavaScriptMode(JavaScriptMode.unrestricted);
      _controller.setNavigationDelegate(
        NavigationDelegate(
          onPageStarted: (String url) {
            setState(() => _isLoading = true);
          },
          onPageFinished: (String url) {
            setState(() => _isLoading = false);
          },
          onNavigationRequest: (NavigationRequest request) {
            if (request.url.startsWith(widget.redirectUri)) {
              final uri = Uri.parse(request.url);
              final code = uri.queryParameters['code'];
              
              if (code != null) {
                if (mounted) Navigator.of(context).pop(code);
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
        title: const Text('Connect with Dropbox'),
        backgroundColor: Colors.blue[900],
        foregroundColor: Colors.white,
      ),
      body: Stack(
        children: [
          WebViewWidget(controller: _controller),
          if (_isLoading)
            const Center(child: CircularProgressIndicator()),
        ],
      ),
    );
  }
}
