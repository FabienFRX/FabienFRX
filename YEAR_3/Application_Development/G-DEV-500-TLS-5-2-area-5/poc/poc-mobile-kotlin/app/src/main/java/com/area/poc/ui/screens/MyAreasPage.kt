package com.area.poc.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.area.poc.data.Area
import com.area.poc.data.ServicesData
import com.area.poc.viewmodel.AREAViewModel

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MyAreasPage(
    viewModel: AREAViewModel,
    onNavigateToBuilder: () -> Unit
) {
    val userAreas by viewModel.userAreas.collectAsState()
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("Mes AREAs (${userAreas.size})") }
            )
        },
        floatingActionButton = {
            FloatingActionButton(
                onClick = onNavigateToBuilder
            ) {
                Icon(Icons.Default.Add, contentDescription = "Créer une AREA")
            }
        }
    ) { paddingValues ->
        if (userAreas.isEmpty()) {
            Box(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(paddingValues),
                contentAlignment = Alignment.Center
            ) {
                Column(
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {
                    Text(
                        text = "Aucune AREA créée",
                        fontSize = 18.sp,
                        fontWeight = FontWeight.Medium,
                        color = MaterialTheme.colorScheme.onSurfaceVariant
                    )
                    Spacer(modifier = Modifier.height(8.dp))
                    Text(
                        text = "Commencez par en créer une !",
                        fontSize = 14.sp,
                        color = MaterialTheme.colorScheme.onSurfaceVariant
                    )
                }
            }
        } else {
            LazyColumn(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(paddingValues)
                    .padding(16.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                items(userAreas) { area ->
                    AreaCard(
                        area = area,
                        onToggle = { viewModel.toggleArea(area.id) },
                        onDelete = { viewModel.deleteArea(area.id) }
                    )
                }
            }
        }
    }
}

@Composable
fun AreaCard(
    area: Area,
    onToggle: () -> Unit,
    onDelete: () -> Unit
) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = if (area.active) 
                MaterialTheme.colorScheme.primaryContainer 
            else 
                MaterialTheme.colorScheme.surfaceVariant
        )
    ) {
        Column(
            modifier = Modifier.padding(16.dp)
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = if (area.active) "Actif" else "Inactif",
                    fontSize = 12.sp,
                    fontWeight = FontWeight.Medium,
                    color = if (area.active) 
                        MaterialTheme.colorScheme.primary 
                    else 
                        MaterialTheme.colorScheme.onSurfaceVariant
                )
                
                AssistChip(
                    onClick = {},
                    label = {
                        Text(
                            text = if (area.active) "✓ Actif" else "○ Inactif",
                            fontSize = 12.sp
                        )
                    },
                    colors = AssistChipDefaults.assistChipColors(
                        containerColor = if (area.active)
                            MaterialTheme.colorScheme.primary
                        else
                            MaterialTheme.colorScheme.surfaceVariant
                    )
                )
            }
            
            Spacer(modifier = Modifier.height(12.dp))
            
            Text(
                text = "QUAND: ${area.action.label}",
                fontSize = 14.sp,
                fontWeight = FontWeight.Medium,
                color = MaterialTheme.colorScheme.onPrimaryContainer
            )
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Text(
                text = "ALORS: ${area.reaction.label}",
                fontSize = 14.sp,
                fontWeight = FontWeight.Medium,
                color = MaterialTheme.colorScheme.onPrimaryContainer
            )
            
            Spacer(modifier = Modifier.height(16.dp))
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                Button(
                    onClick = onToggle,
                    modifier = Modifier.weight(1f),
                    colors = ButtonDefaults.buttonColors(
                        containerColor = if (area.active)
                            MaterialTheme.colorScheme.error
                        else
                            MaterialTheme.colorScheme.primary
                    )
                ) {
                    Text(if (area.active) "Désactiver" else "Activer")
                }
                
                OutlinedButton(
                    onClick = onDelete,
                    modifier = Modifier.weight(1f)
                ) {
                    Text("Supprimer")
                }
            }
        }
    }
}
