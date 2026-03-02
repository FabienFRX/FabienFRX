import React, { useState, useEffect } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { api } from '../services/api';
import ProtectedRoute from '../components/ProtectedRoute';
import LoadingSpinner from '../components/LoadingSpinner';

interface DashboardData {
  stats: {
    total_startups: number;
    total_events: number;
    total_news: number;
    total_users: number;
    total_messages: number;
    recent_startups: number;
    recent_events: number;
    recent_news: number;
  };
  charts: {
    startups_by_sector: Array<{ sector: string; count: number }>;
    startups_by_maturity: Array<{ maturity: string; count: number }>;
    users_by_role: Array<{ role: string; count: number }>;
    daily_activity: Array<{
      date: string;
      startups: number;
      events: number;
      news: number;
    }>;
  };
}

const DashboardContent: React.FC = () => {
  const { user } = useAuth();
  const [data, setData] = useState<DashboardData | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  // Debug: Afficher les informations de l'utilisateur
  console.log('Dashboard - User info:', user);
  console.log('Dashboard - User role:', user?.role);
  console.log('Dashboard - Is admin?', user?.role === 'admin');

  useEffect(() => {
    const fetchData = async () => {
      try {
        setLoading(true);
        const response = await api.get('/dashboard/data/');
        setData(response.data);
      } catch (error) {
        console.error('Error fetching dashboard data:', error);
        setError('Erreur lors du chargement des données');
      } finally {
        setLoading(false);
      }
    };

    fetchData();
  }, []);

  useEffect(() => {
    if (data) {
      // Charger Chart.js dynamiquement
      const script = document.createElement('script');
      script.src = 'https://cdn.jsdelivr.net/npm/chart.js';
      script.onload = () => {
        createCharts();
      };
      document.head.appendChild(script);

      return () => {
        const existingScript = document.querySelector('script[src="https://cdn.jsdelivr.net/npm/chart.js"]');
        if (existingScript) {
          document.head.removeChild(existingScript);
        }
      };
    }
  }, [data]);

  const createCharts = () => {
    if (!data) return;

    // Graphique en barres - Secteurs
    const sectorCtx = document.getElementById('sectorChart') as HTMLCanvasElement;
    if (sectorCtx) {
      new (window as any).Chart(sectorCtx, {
        type: 'bar',
        data: {
          labels: data.charts.startups_by_sector.map(s => s.sector || 'Not defined'),
          datasets: [{
            label: 'Number of startups',
            data: data.charts.startups_by_sector.map(s => s.count),
            backgroundColor: 'rgba(193, 116, 242, 0.6)',
            borderColor: 'rgba(193, 116, 242, 1)',
            borderWidth: 2,
            borderRadius: 8,
            borderSkipped: false,
          }]
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          plugins: {
            title: {
              display: true,
              text: 'Startups by sector',
              font: {
                family: 'Montserrat',
                size: 16,
                weight: '600'
              },
              color: 'var(--purple-dark)'
            },
            legend: {
              labels: {
                font: {
                  family: 'Open Sans'
                }
              }
            }
          },
          scales: {
            y: {
              beginAtZero: true,
              grid: {
                color: 'rgba(0,0,0,0.1)'
              },
              ticks: {
                font: {
                  family: 'Open Sans'
                }
              }
            },
            x: {
              grid: {
                display: false
              },
              ticks: {
                font: {
                  family: 'Open Sans'
                }
              }
            }
          }
        }
      });
    }

    // Graphique en secteurs - Maturité
    const maturityCtx = document.getElementById('maturityChart') as HTMLCanvasElement;
    if (maturityCtx) {
      new (window as any).Chart(maturityCtx, {
        type: 'doughnut',
        data: {
          labels: data.charts.startups_by_maturity.map(m => m.maturity || 'Not defined'),
          datasets: [{
            data: data.charts.startups_by_maturity.map(m => m.count),
            backgroundColor: [
              '#f18585', // coral-light
              '#f49c9c', // coral-medium
              '#f6aeae', // coral-soft
              '#f8cacf', // pink-light
              '#eed5fb', // lavender-light
              '#e4bef8', // lavender-medium
              '#d5a8f2', // purple-light
              '#cb90f1', // purple-medium
              '#c174f2'  // purple-dark
            ],
            borderWidth: 2,
            borderColor: '#ffffff'
          }]
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          plugins: {
            title: {
              display: true,
              text: 'Distribution by maturity',
              font: {
                family: 'Montserrat',
                size: 16,
                weight: '600'
              },
              color: 'var(--purple-dark)'
            },
            legend: {
              position: 'bottom',
              labels: {
                font: {
                  family: 'Open Sans'
                },
                padding: 20
              }
            }
          }
        }
      });
    }

    // Graphique linéaire - Activité quotidienne
    const dailyCtx = document.getElementById('dailyChart') as HTMLCanvasElement;
    if (dailyCtx) {
      new (window as any).Chart(dailyCtx, {
        type: 'line',
        data: {
          labels: data.charts.daily_activity.map(d => d.date),
          datasets: [
            {
              label: 'Startups',
              data: data.charts.daily_activity.map(d => d.startups),
              borderColor: 'var(--purple-dark)',
              backgroundColor: 'rgba(193, 116, 242, 0.1)',
              tension: 0.4,
              fill: true,
              borderWidth: 3
            },
            {
              label: 'Events',
              data: data.charts.daily_activity.map(d => d.events),
              borderColor: 'var(--coral-medium)',
              backgroundColor: 'rgba(244, 156, 156, 0.1)',
              tension: 0.4,
              fill: true,
              borderWidth: 3
            },
            {
              label: 'News',
              data: data.charts.daily_activity.map(d => d.news),
              borderColor: 'var(--purple-medium)',
              backgroundColor: 'rgba(203, 144, 241, 0.1)',
              tension: 0.4,
              fill: true,
              borderWidth: 3
            }
          ]
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          plugins: {
            title: {
              display: true,
              text: 'Activity over the last 30 days',
              font: {
                family: 'Montserrat',
                size: 16,
                weight: '600'
              },
              color: 'var(--purple-dark)'
            },
            legend: {
              labels: {
                font: {
                  family: 'Open Sans'
                }
              }
            }
          },
          scales: {
            y: {
              beginAtZero: true,
              grid: {
                color: 'rgba(0,0,0,0.1)'
              },
              ticks: {
                font: {
                  family: 'Open Sans'
                }
              }
            },
            x: {
              grid: {
                color: 'rgba(0,0,0,0.1)'
              },
              ticks: {
                font: {
                  family: 'Open Sans'
                }
              }
            }
          }
        }
      });
    }
  };

  if (loading) {
    return (
      <div className="dashboard-loading">
        <LoadingSpinner size="large" text="Loading dashboard..." />
      </div>
    );
  }

  if (error) {
    return (
      <div className="dashboard-error">
        <div className="error-card">
          <h2>❌ Error</h2>
          <p>{error}</p>
          <button 
            className="btn btn-primary"
            onClick={() => window.location.reload()}
          >
            Retry
          </button>
        </div>
      </div>
    );
  }

  if (!data) {
    return (
      <div className="dashboard-error">
        <div className="error-card">
          <h2>⚠️ No Data</h2>
          <p>No data available at the moment.</p>
        </div>
      </div>
    );
  }

  return (
    <div className="admin-dashboard">
      <div className="dashboard-header">
        <div className="header-content">
          <h1 className="animated-title">Admin Dashboard</h1>
          <p className="welcome-text">
            Welcome, <strong>{user?.first_name || user?.username}</strong>
          </p>
        </div>
        <div className="header-actions">
          <div className="last-updated">
            Last updated: {new Date().toLocaleTimeString('en-US')}
          </div>
        </div>
      </div>

      {/* Main Statistics */}
      <div className="stats-grid animated-grid">
        <div className="stat-card card-depth">
          <div className="stat-icon">🚀</div>
          <div className="stat-content">
            <h3>Startups</h3>
            <div className="stat-number">{data.stats.total_startups}</div>
            <div className="stat-change positive">
              +{data.stats.recent_startups} this month
            </div>
          </div>
        </div>
        
        <div className="stat-card card-depth">
          <div className="stat-icon">📅</div>
          <div className="stat-content">
            <h3>Events</h3>
            <div className="stat-number">{data.stats.total_events}</div>
            <div className="stat-change positive">
              +{data.stats.recent_events} this month
            </div>
          </div>
        </div>
        
        <div className="stat-card card-depth">
          <div className="stat-icon">📰</div>
          <div className="stat-content">
            <h3>News</h3>
            <div className="stat-number">{data.stats.total_news}</div>
            <div className="stat-change positive">
              +{data.stats.recent_news} this month
            </div>
          </div>
        </div>
        
        <div className="stat-card card-depth">
          <div className="stat-icon">👥</div>
          <div className="stat-content">
            <h3>Users</h3>
            <div className="stat-number">{data.stats.total_users}</div>
            <div className="stat-change">
              {data.stats.total_messages} messages
            </div>
          </div>
        </div>
      </div>

      {/* Charts */}
      <div className="charts-section">
        <div className="charts-grid">
          <div className="chart-container card-depth">
            <div className="chart-header">
              <h3>Distribution by Sector</h3>
            </div>
            <div className="chart-wrapper">
              <canvas id="sectorChart"></canvas>
            </div>
          </div>
          
          <div className="chart-container card-depth">
            <div className="chart-header">
              <h3>Startup Maturity</h3>
            </div>
            <div className="chart-wrapper">
              <canvas id="maturityChart"></canvas>
            </div>
          </div>
        </div>
        
        <div className="chart-container chart-full card-depth">
          <div className="chart-header">
            <h3>Activity Evolution</h3>
          </div>
          <div className="chart-wrapper chart-large">
            <canvas id="dailyChart"></canvas>
          </div>
        </div>
      </div>

      <style jsx>{`
        .admin-dashboard {
          min-height: 100vh;
          background: linear-gradient(135deg, var(--lavender-light) 0%, var(--pink-light) 100%);
          padding: 2rem;
        }
        
        .dashboard-header {
          display: flex;
          justify-content: space-between;
          align-items: center;
          margin-bottom: 3rem;
          background: white;
          padding: 2rem;
          border-radius: 15px;
          box-shadow: 0 10px 30px rgba(0,0,0,0.1);
        }
        
        .header-content h1 {
          color: var(--purple-dark);
          margin-bottom: 0.5rem;
          font-size: 2.5rem;
        }
        
        .welcome-text {
          color: var(--gray-medium);
          font-size: 1.1rem;
          margin: 0;
        }
        
        .header-actions {
          text-align: right;
        }
        
        .last-updated {
          color: var(--gray-medium);
          font-size: 0.9rem;
        }
        
        .stats-grid {
          display: grid;
          grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
          gap: 2rem;
          margin-bottom: 3rem;
        }
        
        .stat-card {
          background: white;
          padding: 2rem;
          border-radius: 15px;
          box-shadow: 0 10px 30px rgba(0,0,0,0.1);
          display: flex;
          align-items: center;
          gap: 1.5rem;
          transition: all 0.3s ease;
          border-top: 4px solid var(--coral-medium);
        }
        
        .stat-card:hover {
          transform: translateY(-5px);
          box-shadow: 0 20px 40px rgba(0,0,0,0.15);
        }
        
        .stat-icon {
          font-size: 3rem;
          opacity: 0.8;
        }
        
        .stat-content h3 {
          color: var(--gray-medium);
          font-size: 1rem;
          margin: 0 0 0.5rem 0;
          font-weight: 500;
        }
        
        .stat-number {
          font-size: 2.5rem;
          font-weight: 700;
          color: var(--purple-dark);
          margin-bottom: 0.5rem;
          font-family: 'Montserrat', sans-serif;
        }
        
        .stat-change {
          font-size: 0.9rem;
          font-weight: 500;
        }
        
        .stat-change.positive {
          color: #28a745;
        }
        
        .charts-section {
          margin-bottom: 2rem;
        }
        
        .charts-grid {
          display: grid;
          grid-template-columns: 1fr 1fr;
          gap: 2rem;
          margin-bottom: 2rem;
        }
        
        .chart-container {
          background: white;
          border-radius: 15px;
          box-shadow: 0 10px 30px rgba(0,0,0,0.1);
          overflow: hidden;
        }
        
        .chart-full {
          grid-column: 1 / -1;
        }
        
        .chart-header {
          padding: 1.5rem 2rem 1rem;
          border-bottom: 1px solid #f0f0f0;
        }
        
        .chart-header h3 {
          color: var(--purple-dark);
          margin: 0;
          font-size: 1.3rem;
          font-weight: 600;
        }
        
        .chart-wrapper {
          padding: 1.5rem 2rem 2rem;
          height: 300px;
        }
        
        .chart-large {
          height: 400px;
        }
        
        .dashboard-loading {
          display: flex;
          justify-content: center;
          align-items: center;
          min-height: 50vh;
        }
        
        .dashboard-error {
          display: flex;
          justify-content: center;
          align-items: center;
          min-height: 50vh;
        }
        
        .error-card {
          background: white;
          padding: 3rem;
          border-radius: 15px;
          box-shadow: 0 10px 30px rgba(0,0,0,0.1);
          text-align: center;
          max-width: 500px;
        }
        
        .error-card h2 {
          color: var(--coral-medium);
          margin-bottom: 1rem;
        }
        
        .error-card p {
          color: var(--gray-medium);
          margin-bottom: 2rem;
        }
        
        @media (max-width: 768px) {
          .admin-dashboard {
            padding: 1rem;
          }
          
          .dashboard-header {
            flex-direction: column;
            text-align: center;
            gap: 1rem;
          }
          
          .header-content h1 {
            font-size: 2rem;
          }
          
          .stats-grid {
            grid-template-columns: 1fr;
            gap: 1rem;
          }
          
          .charts-grid {
            grid-template-columns: 1fr;
            gap: 1rem;
          }
          
          .stat-card {
            padding: 1.5rem;
          }
          
          .chart-wrapper {
            height: 250px;
          }
          
          .chart-large {
            height: 300px;
          }
        }
      `}</style>
    </div>
  );
};

const Dashboard: React.FC = () => {
  return (
    <ProtectedRoute requiredRole="admin">
      <DashboardContent />
    </ProtectedRoute>
  );
};

export default Dashboard; 