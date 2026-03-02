import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';

import { AppComponent } from './app.component';
import { AreaBuilderComponent } from './components/area-builder.component';
import { AreaListComponent } from './components/area-list.component';

@NgModule({
  declarations: [
    AppComponent,
    AreaBuilderComponent,
    AreaListComponent
  ],
  imports: [
    BrowserModule,
    FormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }