// Données mockées pour les services
export const availableServices = {
  gmail: {
    name: 'Gmail',
    icon: '📧',
    actions: [
      { id: 'new_email', label: 'Nouvel email reçu', description: 'Déclenché quand un nouvel email arrive' },
      { id: 'email_with_attachment', label: 'Email avec pièce jointe', description: 'Déclenché quand un email avec pièce jointe arrive' }
    ],
    reactions: []
  },
  onedrive: {
    name: 'OneDrive',
    icon: '☁️',
    actions: [],
    reactions: [
      { id: 'save_file', label: 'Sauvegarder un fichier', description: 'Sauvegarde un fichier dans OneDrive' },
      { id: 'create_folder', label: 'Créer un dossier', description: 'Crée un nouveau dossier' }
    ]
  },
  github: {
    name: 'GitHub',
    icon: '🐙',
    actions: [
      { id: 'new_pr', label: 'Nouvelle Pull Request', description: 'Déclenché quand une nouvelle PR est créée' },
      { id: 'new_issue', label: 'Nouvelle Issue', description: 'Déclenché quand une nouvelle issue est ouverte' }
    ],
    reactions: [
      { id: 'create_issue', label: 'Créer une Issue', description: 'Crée une nouvelle issue dans un repository' },
      { id: 'star_repo', label: 'Starrer un repository', description: 'Star un repository spécifié' }
    ]
  },
  twitter: {
    name: 'Twitter',
    icon: '🐦',
    actions: [
      { id: 'new_tweet', label: 'Nouveau tweet', description: 'Déclenché quand un nouveau tweet est posté' },
      { id: 'new_follower', label: 'Nouveau follower', description: 'Déclenché quand vous gagnez un follower' }
    ],
    reactions: [
      { id: 'post_tweet', label: 'Poster un tweet', description: 'Poste un nouveau tweet' },
      { id: 'like_tweet', label: 'Liker un tweet', description: 'Like un tweet spécifié' }
    ]
  }
}

// Helper functions
export const getServiceActions = () => {
  return Object.entries(availableServices).flatMap(([serviceKey, service]) =>
    service.actions.map(action => ({
      service: serviceKey,
      ...action
    }))
  )
}

export const getServiceReactions = () => {
  return Object.values(availableServices).flatMap(service =>
    service.reactions
  )
}