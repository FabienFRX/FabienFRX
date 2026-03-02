import 'package:flutter/material.dart';

class ReactionConfigForm extends StatefulWidget {
  final String reactionType;
  final Map<String, dynamic> initialValues;
  final Function(Map<String, dynamic>) onChanged;

  const ReactionConfigForm({
    super.key,
    required this.reactionType,
    required this.initialValues,
    required this.onChanged,
  });

  @override
  State<ReactionConfigForm> createState() => _ReactionConfigFormState();
}

class _ReactionConfigFormState extends State<ReactionConfigForm> {
  late Map<String, dynamic> _values;

  @override
  void initState() {
    super.initState();
    _values = Map.from(widget.initialValues);
  }

  void _updateValue(String key, dynamic value) {
    setState(() {
      _values[key] = value;
    });
    widget.onChanged(_values);
  }

  @override
  Widget build(BuildContext context) {
    // Shared styles for Dark Theme
    const labelStyle = TextStyle(color: Color(0xFF9CA3AF), fontSize: 13, fontWeight: FontWeight.w500); // gray-400
    const inputFillColor = Color(0xFF374151); // gray-700
    const inputTextColor = Colors.white;
    const borderColor = Color(0xFF4B5563); // gray-600

    InputDecoration inputDecoration(String hint) {
      return InputDecoration(
        hintText: hint,
        hintStyle: const TextStyle(color: Color(0xFF9CA3AF)),
        filled: true,
        fillColor: inputFillColor,
        border: OutlineInputBorder(borderRadius: BorderRadius.circular(8), borderSide: const BorderSide(color: borderColor)),
        enabledBorder: OutlineInputBorder(borderRadius: BorderRadius.circular(8), borderSide: const BorderSide(color: borderColor)),
        focusedBorder: OutlineInputBorder(borderRadius: BorderRadius.circular(8), borderSide: const BorderSide(color: Color(0xFF6366F1), width: 2)),
        contentPadding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
      );
    }

    switch (widget.reactionType) {
      case 'send_email':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('To Email', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['to_email'],
              decoration: inputDecoration('recipient@example.com'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('to_email', val),
            ),
            const SizedBox(height: 16),
            const Text('Subject', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['subject'],
              decoration: inputDecoration('Email Subject'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('subject', val),
            ),
            const SizedBox(height: 16),
            const Text('Body', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['body'],
              maxLines: 4,
              decoration: inputDecoration('Email Content...'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('body', val),
            ),
          ],
        );

      case 'create_calendar_event':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Event Title', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['title'],
              decoration: inputDecoration('Meeting Title'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('title', val),
            ),
            const SizedBox(height: 16),
            const Text('Duration (minutes)', style: labelStyle),
            const SizedBox(height: 8),
             TextFormField(
              keyboardType: TextInputType.number,
              initialValue: (_values['duration'] ?? 60).toString(),
              decoration: inputDecoration('60'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('duration', int.tryParse(val) ?? 60),
            ),
             const SizedBox(height: 16),
            const Text('Description', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['description'],
              maxLines: 3,
              decoration: inputDecoration('Event details...'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('description', val),
            ),
            const SizedBox(height: 16),
            const Text('Attendees (comma separated)', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['attendees'],
              decoration: inputDecoration('user1@example.com, user2@example.com'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('attendees', val),
            ),
          ],
        );

      case 'save_to_drive':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('File Name', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['filename'],
              decoration: inputDecoration('my-file.txt'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('filename', val),
            ),
             const SizedBox(height: 16),
            const Text('Content', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['content'],
              maxLines: 4,
              decoration: inputDecoration('File content...'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('content', val),
            ),
          ],
        );

      case 'create_drive_folder':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Folder Name', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['folder_name'],
              decoration: inputDecoration('New Folder'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('folder_name', val),
            ),
          ],
        );

      // --- Dropbox Reactions ---
      case 'create_folder': // Dropbox
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Folder Path', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['folder_path'],
              decoration: inputDecoration('/NewFolder'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('folder_path', val),
            ),
          ],
        );

      case 'move_file':
      case 'copy_file':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('From Path', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['from_path'],
              decoration: inputDecoration('/source/file.txt'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('from_path', val),
            ),
            const SizedBox(height: 16),
            const Text('To Path', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['to_path'],
              decoration: inputDecoration('/dest/file.txt'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('to_path', val),
            ),
          ],
        );

      case 'delete_file':
      case 'share_file':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('File Path', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['file_path'],
              decoration: inputDecoration('/path/to/file.txt'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('file_path', val),
            ),
          ],
        );

      // --- LinkedIn Reactions ---
      case 'share_text_post':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
             const Text('Post Content', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['text'],
              maxLines: 4,
              decoration: inputDecoration('What do you want to talk about?'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('text', val),
            ),
          ],
        );

      case 'share_article_post':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Article URL', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['article_url'],
              decoration: inputDecoration('https://example.com/article'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('article_url', val),
            ),
             const SizedBox(height: 16),
             const Text('Comment (Optional)', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['text'],
              maxLines: 3,
              decoration: inputDecoration('Check this out!'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('text', val),
            ),
          ],
        );

      default:
        return Container(
          padding: const EdgeInsets.all(12),
          decoration: BoxDecoration(
            color: const Color(0xFF1F2937),
            borderRadius: BorderRadius.circular(8),
            border: Border.all(color: borderColor),
          ),
          child: const Text('No configuration needed for this reaction.', 
            style: TextStyle(color: Color(0xFF9CA3AF))),
        );
    }
  }
}
