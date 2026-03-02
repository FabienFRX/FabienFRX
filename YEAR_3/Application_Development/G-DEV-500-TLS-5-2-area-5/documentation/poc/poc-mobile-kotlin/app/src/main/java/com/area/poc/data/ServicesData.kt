package com.area.poc.data

object ServicesData {
    val availableServices = mapOf(
        "gmail" to Service(
            name = "Gmail",
            icon = "📧",
            actions = listOf(
                ServiceAction(
                    id = "new_email",
                    label = "Nouvel email reçu",
                    description = "Déclenché quand un nouvel email arrive",
                    serviceKey = "gmail"
                ),
                ServiceAction(
                    id = "email_with_attachment",
                    label = "Email avec pièce jointe",
                    description = "Déclenché quand un email avec pièce jointe arrive",
                    serviceKey = "gmail"
                )
            ),
            reactions = emptyList()
        ),
        "onedrive" to Service(
            name = "OneDrive",
            icon = "☁️",
            actions = emptyList(),
            reactions = listOf(
                ServiceReaction(
                    id = "save_file",
                    label = "Sauvegarder un fichier",
                    description = "Sauvegarde un fichier dans OneDrive",
                    serviceKey = "onedrive"
                ),
                ServiceReaction(
                    id = "create_folder",
                    label = "Créer un dossier",
                    description = "Crée un nouveau dossier",
                    serviceKey = "onedrive"
                )
            )
        ),
        "github" to Service(
            name = "GitHub",
            icon = "🐙",
            actions = listOf(
                ServiceAction(
                    id = "new_pr",
                    label = "Nouvelle Pull Request",
                    description = "Déclenché quand une nouvelle PR est créée",
                    serviceKey = "github"
                ),
                ServiceAction(
                    id = "new_issue",
                    label = "Nouvelle Issue",
                    description = "Déclenché quand une nouvelle issue est ouverte",
                    serviceKey = "github"
                )
            ),
            reactions = listOf(
                ServiceReaction(
                    id = "create_issue",
                    label = "Créer une Issue",
                    description = "Crée une nouvelle issue dans un repository",
                    serviceKey = "github"
                ),
                ServiceReaction(
                    id = "star_repo",
                    label = "Starrer un repository",
                    description = "Star un repository spécifié",
                    serviceKey = "github"
                )
            )
        ),
        "twitter" to Service(
            name = "Twitter",
            icon = "🐦",
            actions = listOf(
                ServiceAction(
                    id = "new_tweet",
                    label = "Nouveau tweet",
                    description = "Déclenché quand un nouveau tweet est posté",
                    serviceKey = "twitter"
                ),
                ServiceAction(
                    id = "new_follower",
                    label = "Nouveau follower",
                    description = "Déclenché quand vous gagnez un follower",
                    serviceKey = "twitter"
                )
            ),
            reactions = listOf(
                ServiceReaction(
                    id = "post_tweet",
                    label = "Poster un tweet",
                    description = "Poste un nouveau tweet",
                    serviceKey = "twitter"
                ),
                ServiceReaction(
                    id = "like_tweet",
                    label = "Liker un tweet",
                    description = "Like un tweet spécifié",
                    serviceKey = "twitter"
                )
            )
        )
    )
    
    val allActions: List<ServiceAction> = availableServices.values.flatMap { it.actions }
    val allReactions: List<ServiceReaction> = availableServices.values.flatMap { it.reactions }
}
