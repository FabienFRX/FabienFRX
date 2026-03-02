import { Injectable } from '@angular/core';
import { Area } from '../models/area.model';
import { BehaviorSubject } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class AreaService {
  private areas: Area[] = [];
  private areasSubject = new BehaviorSubject<Area[]>(this.areas);

  areas$ = this.areasSubject.asObservable();

  createArea(action: any, reaction: any): void {
    const newArea: Area = {
      id: Date.now(),
      action: action,
      reaction: reaction,
      active: true,
      createdAt: new Date().toISOString()
    };

    this.areas.push(newArea);
    this.areasSubject.next([...this.areas]);
  }

  toggleArea(areaId: number): void {
    const area = this.areas.find(a => a.id === areaId);
    if (area) {
      area.active = !area.active;
      this.areasSubject.next([...this.areas]);
    }
  }

  deleteArea(areaId: number): void {
    this.areas = this.areas.filter(area => area.id !== areaId);
    this.areasSubject.next([...this.areas]);
  }

  getAreas(): Area[] {
    return [...this.areas];
  }
}