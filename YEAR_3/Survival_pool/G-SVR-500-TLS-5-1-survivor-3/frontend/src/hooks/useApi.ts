import { useState, useEffect } from 'react';
import { startupService, eventService, newsService, Startup as StartupType, Event as EventType, News as NewsType } from '../services/api';

export const useStartups = () => {
  const [startups, setStartups] = useState<StartupType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchStartups = async () => {
      try {
        setLoading(true);
        const data = await startupService.getAll();
        setStartups(data);
      } catch (err) {
        setError('Erreur lors du chargement des startups');
        console.error('Error fetching startups:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchStartups();
  }, []);

  return { startups, loading, error };
};

export const useEvents = () => {
  const [events, setEvents] = useState<EventType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchEvents = async () => {
      try {
        setLoading(true);
        const data = await eventService.getAll();
        setEvents(data);
      } catch (err) {
        setError('Erreur lors du chargement des événements');
        console.error('Error fetching events:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchEvents();
  }, []);

  return { events, loading, error };
};

export const useNews = () => {
  const [news, setNews] = useState<NewsType[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchNews = async () => {
      try {
        setLoading(true);
        const data = await newsService.getAll();
        setNews(data);
      } catch (err) {
        setError('Erreur lors du chargement des actualités');
        console.error('Error fetching news:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchNews();
  }, []);

  return { news, loading, error };
};
