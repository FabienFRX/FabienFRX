import { ref, computed } from 'vue'

export function useAreas() {
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

  return {
    areas: computed(() => areas.value),
    createArea,
    toggleArea,
    deleteArea
  }
}