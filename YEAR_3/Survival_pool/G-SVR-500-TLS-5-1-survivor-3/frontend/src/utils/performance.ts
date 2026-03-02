// Performance monitoring utility
class PerformanceMonitor {
  private static instance: PerformanceMonitor;
  private metrics: Map<string, number> = new Map();

  public static getInstance(): PerformanceMonitor {
    if (!PerformanceMonitor.instance) {
      PerformanceMonitor.instance = new PerformanceMonitor();
    }
    return PerformanceMonitor.instance;
  }

  public startTiming(label: string): void {
    this.metrics.set(label, performance.now());
  }

  public endTiming(label: string): number {
    const startTime = this.metrics.get(label);
    if (startTime) {
      const duration = performance.now() - startTime;
      console.log(`⏱️ ${label}: ${duration.toFixed(2)}ms`);
      this.metrics.delete(label);
      return duration;
    }
    return 0;
  }

  public measureApiCall<T>(
    label: string,
    apiCall: () => Promise<T>
  ): Promise<T> {
    this.startTiming(label);
    return apiCall()
      .then((result) => {
        this.endTiming(label);
        return result;
      })
      .catch((error) => {
        this.endTiming(label);
        console.error(`❌ ${label} failed:`, error);
        throw error;
      });
  }

  public logPageLoad(): void {
    window.addEventListener('load', () => {
      const navigation = performance.getEntriesByType('navigation')[0] as PerformanceNavigationTiming;
      console.log('📊 Page Performance Metrics:');
      console.log(`  • DNS Lookup: ${(navigation.domainLookupEnd - navigation.domainLookupStart).toFixed(2)}ms`);
      console.log(`  • Server Response: ${(navigation.responseEnd - navigation.requestStart).toFixed(2)}ms`);
      console.log(`  • DOM Content Loaded: ${(navigation.domContentLoadedEventEnd - navigation.fetchStart).toFixed(2)}ms`);
      console.log(`  • Page Load Complete: ${(navigation.loadEventEnd - navigation.fetchStart).toFixed(2)}ms`);
    });
  }
}

export const perfMonitor = PerformanceMonitor.getInstance();

// Initialize page load monitoring
if (typeof window !== 'undefined') {
  perfMonitor.logPageLoad();
}
