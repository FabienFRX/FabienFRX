import React from 'react';
import { useExample } from '../hooks/useExample';

export function Counter() {
  const { count, inc } = useExample();
  return (
    <button onClick={inc} style={{ padding: '0.5rem 1rem' }}>
      Cliccato {count} volte
    </button>
  );
}
