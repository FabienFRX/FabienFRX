import { Component, OnInit } from '@angular/core';
import { MockDataService } from '../services/mock-data.service';
import { AreaService } from '../services/area.service';

@Component({
  selector: 'app-area-builder',
  templateUrl: './area-builder.component.html',
  styleUrls: ['./area-builder.component.css']
})
export class AreaBuilderComponent implements OnInit {
  selectedAction: any = null;
  selectedReaction: any = null;
  availableActions: any[] = [];
  availableReactions: any[] = [];

  constructor(
    private mockDataService: MockDataService,
    private areaService: AreaService
  ) {}

  ngOnInit(): void {
    this.availableActions = this.mockDataService.getServiceActions();
    this.availableReactions = this.mockDataService.getServiceReactions();
  }

  onActionChange(): void {
    this.selectedReaction = null;
  }

  onCreateArea(): void {
    if (this.selectedAction && this.selectedReaction) {
      this.areaService.createArea(this.selectedAction, this.selectedReaction);
      this.selectedAction = null;
      this.selectedReaction = null;
    }
  }

  canCreateArea(): boolean {
    return !!this.selectedAction && !!this.selectedReaction;
  }
}