<template>
  <div class="areas-list">
    <h2>Mes AREAs ({{ areas.length }})</h2>
    
    <div v-if="areas.length === 0" class="no-areas">
      <p>Aucune AREA créée. Commencez par en créer une !</p>
    </div>

    <div 
      v-for="area in areas" 
      :key="area.id"
      :class="['area-card', { active: area.active, inactive: !area.active }]"
    >
      <div class="area-content">
        <div class="area-description">
          <span class="area-when">
            <strong>Quand:</strong> {{ area.action.label }}
          </span>
          <span class="area-then">
            <strong>Alors:</strong> {{ area.reaction.label }}
          </span>
        </div>
        <div class="area-actions">
          <button 
            @click="toggleArea(area.id)"
            :class="['toggle-button', area.active ? 'deactivate' : 'activate']"
          >
            {{ area.active ? 'Désactiver' : 'Activer' }}
          </button>
          <button 
            @click="deleteArea(area.id)"
            class="delete-button"
          >
            Supprimer
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { useAreas } from '../composables/useAreas.js'

const { areas, toggleArea, deleteArea } = useAreas()
</script>

<style scoped>
.areas-list h2 {
  color: #2c3e50;
  margin-bottom: 20px;
  text-align: center;
}

.no-areas {
  color: #7f8c8d;
  font-style: italic;
  padding: 20px;
  text-align: center;
}

.area-card {
  border: 1px solid #ddd;
  border-radius: 8px;
  padding: 15px;
  margin: 10px 0;
  background-color: white;
  transition: all 0.3s ease;
}

.area-card.active {
  border-left: 4px solid #27ae60;
}

.area-card.inactive {
  border-left: 4px solid #e74c3c;
  opacity: 0.7;
}

.area-content {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.area-description {
  flex: 1;
}

.area-when, .area-then {
  display: block;
  margin-bottom: 5px;
}

.area-actions {
  display: flex;
  gap: 10px;
}

.toggle-button, .delete-button {
  padding: 8px 15px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 14px;
  transition: background-color 0.3s;
}

.toggle-button.activate {
  background-color: #27ae60;
  color: white;
}

.toggle-button.deactivate {
  background-color: #e74c3c;
  color: white;
}

.delete-button {
  background-color: #95a5a6;
  color: white;
}

.delete-button:hover {
  background-color: #7f8c8d;
}

/* Responsive */
@media (max-width: 768px) {
  .area-content {
    flex-direction: column;
    align-items: flex-start;
    gap: 15px;
  }
  
  .area-actions {
    align-self: stretch;
    justify-content: space-between;
  }
}
</style>