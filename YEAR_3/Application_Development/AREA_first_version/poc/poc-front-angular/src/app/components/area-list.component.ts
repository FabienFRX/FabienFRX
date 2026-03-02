import { Component, OnInit } from '@angular/core';
import { Area } from '../models/area.model';
import { AreaService } from '../services/area.service';

@Component({
  selector: 'app-area-list',
  templateUrl: './area-list.component.html',
  styleUrls: ['./area-list.component.css']
})
export class AreaListComponent implements OnInit {
  areas: Area[] = [];

  constructor(private areaService: AreaService) {}

  ngOnInit(): void {
    this.areaService.areas$.subscribe(areas => {
      this.areas = areas;
    });
  }

  onToggleArea(areaId: number): void {
    this.areaService.toggleArea(areaId);
  }

  onDeleteArea(areaId: number): void {
    this.areaService.deleteArea(areaId);
  }
}