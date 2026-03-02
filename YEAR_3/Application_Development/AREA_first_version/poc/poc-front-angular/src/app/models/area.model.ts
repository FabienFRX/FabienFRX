import { AreaAction, AreaReaction } from './service.model';

export interface Area {
  id: number;
  action: AreaAction;
  reaction: AreaReaction;
  active: boolean;
  createdAt: string;
}