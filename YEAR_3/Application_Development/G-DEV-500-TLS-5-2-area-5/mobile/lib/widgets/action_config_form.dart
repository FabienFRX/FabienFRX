import 'package:flutter/material.dart';

class ActionConfigForm extends StatefulWidget {
  final String actionType;
  final Map<String, dynamic> initialValues;
  final Function(Map<String, dynamic>) onChanged;

  const ActionConfigForm({
    super.key,
    required this.actionType,
    required this.initialValues,
    required this.onChanged,
  });

  @override
  State<ActionConfigForm> createState() => _ActionConfigFormState();
}

class _ActionConfigFormState extends State<ActionConfigForm> {
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

    switch (widget.actionType) {
      case 'specific_time':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Date', style: labelStyle),
            const SizedBox(height: 8),
            GestureDetector(
              onTap: () async {
                final date = await showDatePicker(
                  context: context,
                  initialDate: DateTime.now(),
                  firstDate: DateTime.now(),
                  lastDate: DateTime.now().add(const Duration(days: 365)),
                );
                if (date != null) {
                  _updateValue('date', date.toIso8601String().split('T')[0]);
                }
              },
              child: AbsorbPointer(
                child: TextFormField(
                  decoration: inputDecoration('YYYY-MM-DD'),
                  controller: TextEditingController(text: _values['date']),
                  style: const TextStyle(color: inputTextColor),
                ),
              ),
            ),
            const SizedBox(height: 16),
            const Text('Time (HH:MM)', style: labelStyle),
            const SizedBox(height: 8),
             GestureDetector(
              onTap: () async {
                final time = await showTimePicker(
                  context: context,
                  initialTime: TimeOfDay.now(),
                );
                if (time != null) {
                  final hour = time.hour.toString().padLeft(2, '0');
                  final minute = time.minute.toString().padLeft(2, '0');
                  _updateValue('time', '$hour:$minute');
                }
              },
              child: AbsorbPointer(
                child: TextFormField(
                  decoration: inputDecoration('HH:MM'),
                  controller: TextEditingController(text: _values['time']),
                  style: const TextStyle(color: inputTextColor),
                ),
              ),
            ),
          ],
        );

      case 'daily_at_time':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Time (HH:MM)', style: labelStyle),
            const SizedBox(height: 8),
             GestureDetector(
              onTap: () async {
                final time = await showTimePicker(
                  context: context,
                  initialTime: TimeOfDay(hour: 9, minute: 0),
                );
                if (time != null) {
                  final hour = time.hour.toString().padLeft(2, '0');
                  final minute = time.minute.toString().padLeft(2, '0');
                  _updateValue('time', '$hour:$minute');
                }
              },
              child: AbsorbPointer(
                child: TextFormField(
                  decoration: inputDecoration('09:00'),
                  controller: TextEditingController(text: _values['time'] ?? '09:00'),
                  style: const TextStyle(color: inputTextColor),
                ),
              ),
            ),
          ],
        );

      case 'weekly_on_day':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Day of week', style: labelStyle),
            const SizedBox(height: 8),
            DropdownButtonFormField<String>(
              value: _values['day'] ?? 'monday',
              dropdownColor: inputFillColor,
              style: const TextStyle(color: inputTextColor),
              decoration: inputDecoration('Select day'),
              items: ['monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday', 'sunday']
                  .map((d) => DropdownMenuItem(value: d, child: Text(d[0].toUpperCase() + d.substring(1))))
                  .toList(),
              onChanged: (val) => _updateValue('day', val),
            ),
            const SizedBox(height: 16),
            const Text('Time (HH:MM)', style: labelStyle),
            GestureDetector(
              onTap: () async {
                final time = await showTimePicker(
                  context: context,
                  initialTime: TimeOfDay(hour: 9, minute: 0),
                );
                if (time != null) {
                  final hour = time.hour.toString().padLeft(2, '0');
                  final minute = time.minute.toString().padLeft(2, '0');
                  _updateValue('time', '$hour:$minute');
                }
              },
              child: AbsorbPointer(
                child: TextFormField(
                  decoration: inputDecoration('09:00'),
                  controller: TextEditingController(text: _values['time'] ?? '09:00'),
                  style: const TextStyle(color: inputTextColor),
                ),
              ),
            ),
          ],
        );

      case 'every_x_seconds':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Interval (seconds)', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              keyboardType: TextInputType.number,
              initialValue: (_values['seconds'] ?? 30).toString(),
              decoration: inputDecoration('30'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('seconds', int.tryParse(val) ?? 30),
            ),
          ],
        );
      
      case 'every_x_minutes':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Interval (minutes)', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              keyboardType: TextInputType.number,
              initialValue: (_values['minutes'] ?? 5).toString(),
              decoration: inputDecoration('5'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('minutes', int.tryParse(val) ?? 5),
            ),
          ],
        );

      case 'every_x_hours':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Interval (hours)', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              keyboardType: TextInputType.number,
              initialValue: (_values['hours'] ?? 1).toString(),
              decoration: inputDecoration('1'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('hours', int.tryParse(val) ?? 1),
            ),
          ],
        );

      case 'calendar_event_starting':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Alert me (minutes before)', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              keyboardType: TextInputType.number,
              initialValue: (_values['minutes_before'] ?? 15).toString(),
              decoration: inputDecoration('15'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('minutes_before', int.tryParse(val) ?? 15),
            ),
          ],
        );
      
      case 'new_email_received':
      case 'new_email_from_sender':
         return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('From Email (optional)', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['from_email'],
              decoration: inputDecoration('sender@example.com'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('from_email', val),
            ),
            const SizedBox(height: 16),
            const Text('Subject contains (optional)', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['subject_contains'],
              decoration: inputDecoration('keyword'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('subject_contains', val),
            ),
          ],
        );

      case 'file_modified':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('File Path', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['file_path'],
              decoration: inputDecoration('/Documents/myfile.txt'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('file_path', val),
            ),
          ],
        );

      case 'new_file_in_folder':
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Folder Path', style: labelStyle),
            const SizedBox(height: 8),
            TextFormField(
              initialValue: _values['folder_path'],
              decoration: inputDecoration('/Documents'),
              style: const TextStyle(color: inputTextColor),
              onChanged: (val) => _updateValue('folder_path', val),
            ),
          ],
        );

      // --- Google Drive Actions ---
      case 'new_drive_file':
      case 'drive_file_uploaded': // Alias
        return Container(
          padding: const EdgeInsets.all(12),
          decoration: BoxDecoration(
            color: const Color(0xFF1F2937),
            borderRadius: BorderRadius.circular(8),
            border: Border.all(color: borderColor),
          ),
          child: const Text('Triggers when any new file is uploaded to your Drive.', 
            style: TextStyle(color: Color(0xFF9CA3AF))),
        );

      case 'drive_file_shared':
        return Container(
          padding: const EdgeInsets.all(12),
          decoration: BoxDecoration(
            color: const Color(0xFF1F2937),
            borderRadius: BorderRadius.circular(8),
            border: Border.all(color: borderColor),
          ),
          child: const Text('Triggers when a file is shared with you.', 
            style: TextStyle(color: Color(0xFF9CA3AF))),
        );

      // --- Calendar Actions ---
      case 'calendar_event_created':
        return Container(
          padding: const EdgeInsets.all(12),
          decoration: BoxDecoration(
            color: const Color(0xFF1F2937),
            borderRadius: BorderRadius.circular(8),
            border: Border.all(color: borderColor),
          ),
          child: const Text('Triggers when a new event is added to your primary calendar.', 
            style: TextStyle(color: Color(0xFF9CA3AF))),
        );

      // --- LinkedIn Actions ---
      case 'new_post_by_user':
        return Container(
          padding: const EdgeInsets.all(12),
          decoration: BoxDecoration(
            color: const Color(0xFF1F2937),
            borderRadius: BorderRadius.circular(8),
            border: Border.all(color: borderColor),
          ),
          child: const Text('Triggers when you create a new post on LinkedIn.', 
            style: TextStyle(color: Color(0xFF9CA3AF))),
        );


      default:
        return Container(
          padding: const EdgeInsets.all(12),
          decoration: BoxDecoration(
            color: const Color(0xFF1F2937), // gray-800
            borderRadius: BorderRadius.circular(8),
            border: Border.all(color: borderColor),
          ),
          child: const Text('No configuration needed for this action.', 
            style: TextStyle(color: Color(0xFF9CA3AF))),
        );
    }
  }
}
