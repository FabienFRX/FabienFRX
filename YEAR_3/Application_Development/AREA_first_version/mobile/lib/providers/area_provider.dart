import 'package:flutter/material.dart';
import '../models/area.dart';

class AreaProvider with ChangeNotifier {
  List<Area> _areas = [];
  List<Map<String, dynamic>> _services = [];

  List<Area> get areas => _areas;
  List<Map<String, dynamic>> get services => _services;

  void setAreas(List<Area> areas) {
    _areas = areas;
    notifyListeners();
  }

  void setServices(List<Map<String, dynamic>> services) {
    _services = services;
    notifyListeners();
  }

  void addArea(Area area) {
    _areas.add(area);
    notifyListeners();
  }

  void removeArea(String id) {
    _areas.removeWhere((area) => area.id == id);
    notifyListeners();
  }

  void toggleArea(String id) {
    final index = _areas.indexWhere((area) => area.id == id);
    if (index != -1) {
      _areas[index].isActive = !_areas[index].isActive;
      notifyListeners();
    }
  }
}
