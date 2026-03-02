import React from 'react';
import AREABuilder from './AREABuilder';
import './App.css';

function App() {
  return (
    <div className="App">
      <header className="app-header">
        <h1>Automation Platform - POC React</h1>
      </header>
      <main>
        <AREABuilder />
      </main>
    </div>
  );
}

export default App;