import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';

export default defineConfig({
  plugins: [react()],
  envDir: '..', // Look for .env files in parent directory (root)
  server: {
    host: '0.0.0.0',
    port: 5173,
    hmr: {
      host: 'jeb-platform.local'
    },
    // Accepter les domaines JEB
    allowedHosts: [
      'jeb-platform.local',
      'www.jeb-platform.local',
      'jeb-api.local', 
      'www.jeb-api.local',
      'jeb-admin.local',
      'www.jeb-admin.local'
    ]
  },
  build: {
    rollupOptions: {
      output: {
        manualChunks: {
          'pdf-libs': ['jspdf', 'html2canvas']
        }
      }
    },
    chunkSizeWarningLimit: 1000
  }
});
