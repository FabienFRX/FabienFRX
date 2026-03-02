export interface ServiceAction {
  id: string;
  label: string;
  description: string;
}

export interface ServiceReaction {
  id: string;
  label: string;
  description: string;
}

export interface Service {
  name: string;
  icon: string;
  actions: ServiceAction[];
  reactions: ServiceReaction[];
}

export interface AreaAction {
  service: string;
  id: string;
  label: string;
}

export interface AreaReaction {
  id: string;
  label: string;
}