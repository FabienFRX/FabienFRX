import React, { useState, useEffect } from 'react';
import { useAuth } from '../../contexts/AuthContext';
import { startupService, newsService } from '../../services/api';

interface StartupDashboardData {
  startupProfile: any;
  profileViews: number;
  recentNews: any[];
  opportunities: any[];
}

const StartupDashboard: React.FC = () => {
  const { user } = useAuth();
  const [dashboardData, setDashboardData] = useState<StartupDashboardData>({
    startupProfile: null,
    profileViews: 0,
    recentNews: [],
    opportunities: []
  });
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const fetchDashboardData = async () => {
      try {
        setLoading(true);
