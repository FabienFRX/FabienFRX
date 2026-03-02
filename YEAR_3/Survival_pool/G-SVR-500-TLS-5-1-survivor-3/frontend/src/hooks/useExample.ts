import { useState } from 'react';

export function useExample() {
  const [count, setCount] = useState(0);
  return { count, inc: () => setCount(c => c + 1) };
}
