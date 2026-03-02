package com.area.poc.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.area.poc.data.ServiceAction
import com.area.poc.data.ServiceReaction
import com.area.poc.data.ServicesData
import com.area.poc.viewmodel.AREAViewModel

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun AREABuilderPage(
    viewModel: AREAViewModel,
    onNavigateToMyAreas: () -> Unit
) {
    val selectedAction by viewModel.selectedAction.collectAsState()
    val selectedReaction by viewModel.selectedReaction.collectAsState()
    
    var actionExpanded by remember { mutableStateOf(false) }
    var reactionExpanded by remember { mutableStateOf(false) }
    
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ) {
        Text(
            text = "Créer une AREA",
            fontSize = 28.sp,
            fontWeight = FontWeight.Bold,
            modifier = Modifier.padding(bottom = 24.dp)
        )
        
        // Action Dropdown
        Text(
            text = "Quand (Action)",
            fontSize = 16.sp,
            fontWeight = FontWeight.Medium,
            modifier = Modifier.padding(bottom = 8.dp)
        )
        
        ExposedDropdownMenuBox(
            expanded = actionExpanded,
            onExpandedChange = { actionExpanded = !actionExpanded }
        ) {
            OutlinedTextField(
                value = selectedAction?.let { "${ServicesData.availableServices[it.serviceKey]?.icon} ${it.label}" } ?: "",
                onValueChange = {},
                readOnly = true,
                placeholder = { Text("Choisir une action...") },
                trailingIcon = { ExposedDropdownMenuDefaults.TrailingIcon(expanded = actionExpanded) },
                modifier = Modifier
                    .fillMaxWidth()
                    .menuAnchor()
            )
            
            ExposedDropdownMenu(
                expanded = actionExpanded,
                onDismissRequest = { actionExpanded = false }
            ) {
                ServicesData.allActions.forEach { action ->
                    DropdownMenuItem(
                        text = {
                            Text("${ServicesData.availableServices[action.serviceKey]?.icon} ${action.label}")
                        },
                        onClick = {
                            viewModel.selectAction(action)
                            actionExpanded = false
                        }
                    )
                }
            }
        }
        
        Spacer(modifier = Modifier.height(24.dp))
        
        // Reaction Dropdown
        Text(
            text = "Alors (REAction)",
            fontSize = 16.sp,
            fontWeight = FontWeight.Medium,
            modifier = Modifier.padding(bottom = 8.dp)
        )
        
        ExposedDropdownMenuBox(
            expanded = reactionExpanded,
            onExpandedChange = { reactionExpanded = !reactionExpanded }
        ) {
            OutlinedTextField(
                value = selectedReaction?.let { "${ServicesData.availableServices[it.serviceKey]?.icon} ${it.label}" } ?: "",
                onValueChange = {},
                readOnly = true,
                placeholder = { Text("Choisir une réaction...") },
                trailingIcon = { ExposedDropdownMenuDefaults.TrailingIcon(expanded = reactionExpanded) },
                enabled = selectedAction != null,
                modifier = Modifier
                    .fillMaxWidth()
                    .menuAnchor()
            )
            
            ExposedDropdownMenu(
                expanded = reactionExpanded,
                onDismissRequest = { reactionExpanded = false }
            ) {
                ServicesData.allReactions.forEach { reaction ->
                    DropdownMenuItem(
                        text = {
                            Text("${ServicesData.availableServices[reaction.serviceKey]?.icon} ${reaction.label}")
                        },
                        onClick = {
                            viewModel.selectReaction(reaction)
                            reactionExpanded = false
                        }
                    )
                }
            }
        }
        
        // Preview Card
        if (selectedAction != null || selectedReaction != null) {
            Spacer(modifier = Modifier.height(24.dp))
            Card(
                modifier = Modifier.fillMaxWidth(),
                colors = CardDefaults.cardColors(
                    containerColor = MaterialTheme.colorScheme.secondaryContainer
                )
            ) {
                Column(
                    modifier = Modifier.padding(16.dp)
                ) {
                    Text(
                        text = "Aperçu",
                        fontSize = 16.sp,
                        fontWeight = FontWeight.SemiBold,
                        color = MaterialTheme.colorScheme.onSecondaryContainer
                    )
                    Spacer(modifier = Modifier.height(8.dp))
                    
                    selectedAction?.let { action ->
                        Text(
                            text = "SI: ${action.label}",
                            fontSize = 14.sp,
                            color = MaterialTheme.colorScheme.onSecondaryContainer
                        )
                    }
                    
                    selectedReaction?.let { reaction ->
                        Text(
                            text = "ALORS: ${reaction.label}",
                            fontSize = 14.sp,
                            color = MaterialTheme.colorScheme.onSecondaryContainer
                        )
                    }
                }
            }
        }
        
        Spacer(modifier = Modifier.weight(1f))
        
        // Create Button
        Button(
            onClick = {
                viewModel.createArea()
                onNavigateToMyAreas()
            },
            enabled = selectedAction != null && selectedReaction != null,
            modifier = Modifier
                .fillMaxWidth()
                .height(56.dp)
        ) {
            Text(
                text = "Créer l'AREA",
                fontSize = 16.sp,
                fontWeight = FontWeight.Medium
            )
        }
    }
}
