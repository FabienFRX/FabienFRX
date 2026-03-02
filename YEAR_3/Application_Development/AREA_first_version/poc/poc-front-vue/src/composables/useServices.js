import { ref, computed } from 'vue'
import { availableServices, getServiceActions, getServiceReactions } from '../data/servicesData.js'

export function useServices() {
  const services = ref(availableServices)
  const actions = ref(getServiceActions())
  const reactions = ref(getServiceReactions())

  const getReactionsForAction = (action) => {

    return reactions.value
  }

  return {
    services: computed(() => services.value),
    actions: computed(() => actions.value),
    reactions: computed(() => reactions.value),
    getReactionsForAction
  }
}