<template>
  <div class="app">
    <header class="app-header">
      <h1>Automation Platform - POC Vue.js</h1>
    </header>
    
    <main class="main-content">
      <AreaBuilder :areas="areas" @create-area="createArea" />
      <AreaList :areas="areas" @toggle-area="toggleArea" @delete-area="deleteArea" />
    </main>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import AreaBuilder from './components/AreaBuilder.vue'
import AreaList from './components/AreaList.vue'

const areas = ref([])

const createArea = (action, reaction) => {
  const newArea = {
    id: Date.now(),
    action,
    reaction,
    active: true,
    createdAt: new Date().toISOString()
  }
  areas.value.push(newArea)
}

const toggleArea = (areaId) => {
  const area = areas.value.find(a => a.id === areaId)
  if (area) {
    area.active = !area.active
  }
}

const deleteArea = (areaId) => {
  areas.value = areas.value.filter(area => area.id !== areaId)
}
</script>