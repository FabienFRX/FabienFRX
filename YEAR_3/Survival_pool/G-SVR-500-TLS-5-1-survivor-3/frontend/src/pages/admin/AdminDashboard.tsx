import React, { useState, useEffect } from 'react';
import { useAuth } from '../../contexts/AuthContext';
import { startupService, newsService, eventService, jebUserService } from '../../services/api';

interface DashboardStats {
  totalStartups: number;
  totalNews: number;
  totalEvents: number;
  totalUsers: number;
  recentStartups: any[];
  recentNews: any[];
}

const AdminDashboard: React.FC = () => {
  const { user } = useAuth();
  const [stats, setStats] = useState<DashboardStats>({
    totalStartups: 0,
    totalNews: 0,
    totalEvents: 0,
    totalUsers: 0,
    recentStartups: [],
    recentNews: []
  });
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const fetchDashboardData = async () => {
      try {
        setLoading(true);
