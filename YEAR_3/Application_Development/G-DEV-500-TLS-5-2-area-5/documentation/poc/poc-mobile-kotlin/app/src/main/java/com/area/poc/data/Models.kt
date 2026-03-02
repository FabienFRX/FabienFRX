package com.area.poc.data

data class ServiceAction(
    val id: String,
    val label: String,
    val description: String,
    val serviceKey: String
)

data class ServiceReaction(
    val id: String,
    val label: String,
    val description: String,
    val serviceKey: String
)

data class Service(
    val name: String,
    val icon: String,
    val actions: List<ServiceAction>,
    val reactions: List<ServiceReaction>
)

data class Area(
    val id: Long,
    val action: ServiceAction,
    val reaction: ServiceReaction,
    var active: Boolean = true,
    val createdAt: Long = System.currentTimeMillis()
)
