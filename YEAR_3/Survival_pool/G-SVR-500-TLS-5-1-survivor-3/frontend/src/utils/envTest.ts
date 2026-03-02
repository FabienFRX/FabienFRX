// Test file to verify environment variables are loaded from root .env
console.log('🔧 Environment Variables Test:');
console.log('VITE_API_BASE_URL:', import.meta.env.VITE_API_BASE_URL);
console.log('VITE_API_TIMEOUT:', import.meta.env.VITE_API_TIMEOUT);
console.log('MODE:', import.meta.env.MODE);
console.log('DEV:', import.meta.env.DEV);

// Export for testing
export const envTest = {
  apiBaseUrl: import.meta.env.VITE_API_BASE_URL,
  apiTimeout: import.meta.env.VITE_API_TIMEOUT,
  mode: import.meta.env.MODE,
  isDev: import.meta.env.DEV
};

export default envTest;
