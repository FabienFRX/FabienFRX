package com.area.poc.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.area.poc.data.Area
import com.area.poc.data.ServiceAction
import com.area.poc.data.ServiceReaction
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

class AREAViewModel : ViewModel() {
    private val _selectedAction = MutableStateFlow<ServiceAction?>(null)
    val selectedAction: StateFlow<ServiceAction?> = _selectedAction.asStateFlow()
    
    private val _selectedReaction = MutableStateFlow<ServiceReaction?>(null)
    val selectedReaction: StateFlow<ServiceReaction?> = _selectedReaction.asStateFlow()
    
    private val _userAreas = MutableStateFlow<List<Area>>(emptyList())
    val userAreas: StateFlow<List<Area>> = _userAreas.asStateFlow()
    
    fun selectAction(action: ServiceAction?) {
        _selectedAction.value = action
    }
    
    fun selectReaction(reaction: ServiceReaction?) {
        _selectedReaction.value = reaction
    }
    
    fun createArea() {
        viewModelScope.launch {
            val action = _selectedAction.value
            val reaction = _selectedReaction.value
            
            if (action != null && reaction != null) {
                val newArea = Area(
                    id = System.currentTimeMillis(),
                    action = action,
                    reaction = reaction,
                    active = true
                )
                _userAreas.value = _userAreas.value + newArea
                _selectedAction.value = null
                _selectedReaction.value = null
            }
        }
    }
    
    fun toggleArea(areaId: Long) {
        viewModelScope.launch {
            _userAreas.value = _userAreas.value.map { area ->
                if (area.id == areaId) {
                    area.copy(active = !area.active)
                } else {
                    area
                }
            }
        }
    }
    
    fun deleteArea(areaId: Long) {
        viewModelScope.launch {
            _userAreas.value = _userAreas.value.filter { it.id != areaId }
        }
    }
}
