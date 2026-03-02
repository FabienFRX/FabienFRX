import React from 'react';

interface KPICardProps {
  title: string;
  value: string | number;
  icon: string;
  trend?: string;
  description: string;
  color: string;
}

const KPICard: React.FC<KPICardProps> = ({ title, value, icon, trend, description }) => {
  return (
    <div className="card" style={{ textAlign: 'center' }}>
      <div style={{ fontSize: '4rem', marginBottom: '1rem' }}>{icon}</div>
      <h3 style={{ marginBottom: '0.5rem', fontSize: '2.5rem', fontWeight: 'bold' }}>
        {value}
      </h3>
      <p style={{ color: 'var(--purple-medium)', marginBottom: '1rem', fontWeight: '600' }}>
        {title}
      </p>
      {trend && (
        <div style={{ 
          color: trend.startsWith('+') ? '#28a745' : '#dc3545',
          fontSize: '0.9rem',
          fontWeight: '600',
          marginBottom: '1rem'
        }}>
          {trend}
        </div>
      )}
      <p style={{ fontSize: '0.9rem', color: 'var(--gray-dark)' }}>
        {description}
      </p>
    </div>
  );
};

export default KPICard;
