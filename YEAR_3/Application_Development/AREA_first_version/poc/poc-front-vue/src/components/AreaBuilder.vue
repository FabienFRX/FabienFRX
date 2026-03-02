<template>
  <div class="area-builder">
    <div class="creation-section">
      <h2>Créer une nouvelle AREA</h2>
      
      <div class="form-group">
        <label for="action-select">Quand (Action) :</label>
        <select 
          id="action-select"
          v-model="selectedAction"
          @change="onActionChange"
          class="form-select"
        >
          <option :value="null">Choisir une action...</option>
          <option 
            v-for="action in actions" 
            :key="`${action.service}-${action.id}`"
            :value="action"
          >
            {{ action.label }} ({{ action.service }})
          </option>
        </select>
      </div>

      <div class="form-group">
        <label for="reaction-select">Alors (REAction) :</label>
        <select 
          id="reaction-select"
          v-model="selectedReaction"
          :disabled="!selectedAction"
          class="form-select"
        >
          <option :value="null">Choisir une réaction...</option>
          <option 
            v-for="reaction in availableReactions" 
            :key="reaction.id"
            :value="reaction"
          >
            {{ reaction.label }}
          </option>
        </select>
      </div>

      <button 
        @click="createArea"
        :disabled="!canCreateArea"
        class="create-button"
      >
        Créer l'AREA
      </button>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { useServices } from '../composables/useServices.js'

const props = defineProps(['areas'])
const emit = defineEmits(['create-area'])

const { actions, reactions, getReactionsForAction } = useServices()

const selectedAction = ref(null)
const selectedReaction = ref(null)
const availableReactions = ref([])

const onActionChange = () => {
  selectedReaction.value = null
  if (selectedAction.value) {
    availableReactions.value = getReactionsForAction(selectedAction.value)
  }
}

const canCreateArea = computed(() => {
  return selectedAction.value && selectedReaction.value
})

const createArea = () => {
  if (selectedAction.value && selectedReaction.value) {
    emit('create-area', selectedAction.value, selectedReaction.value)
    selectedAction.value = null
    selectedReaction.value = null
    availableReactions.value = []
  }
}


</script>

<style scoped>
.area-builder {
  margin-bottom: 30px;
}

.creation-section {
  border: 2px solid #e1e5e9;
  border-radius: 10px;
  padding: 25px;
  background-color: #f8f9fa;
}

.creation-section h2 {
  color: #2c3e50;
  margin-bottom: 20px;
  text-align: center;
}

.form-group {
  margin-bottom: 20px;
}

.form-group label {
  display: block;
  margin-bottom: 8px;
  font-weight: 600;
  color: #34495e;
}

.form-select {
  width: 100%;
  padding: 10px;
  border: 1px solid #bdc3c7;
  border-radius: 5px;
  font-size: 14px;
  background-color: white;
}

.form-select:disabled {
  background-color: #ecf0f1;
  color: #7f8c8d;
}

.create-button {
  background-color: #3498db;
  color: white;
  border: none;
  padding: 12px 30px;
  border-radius: 5px;
  font-size: 16px;
  cursor: pointer;
  transition: background-color 0.3s;
  display: block;
  margin: 0 auto;
}

.create-button:hover:not(:disabled) {
  background-color: #2980b9;
}

.create-button:disabled {
  background-color: #bdc3c7;
  cursor: not-allowed;
}
</style>