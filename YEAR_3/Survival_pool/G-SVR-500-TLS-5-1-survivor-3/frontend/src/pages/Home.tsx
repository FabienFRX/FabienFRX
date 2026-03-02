import React, { useState, useEffect, useMemo, useCallback } from 'react';
import { Link } from 'react-router-dom';
import { startupService, Startup } from '../services/api';

const Home: React.FC = () => {
  const [allStartups, setAllStartups] = useState<Startup[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [rotationIndex, setRotationIndex] = useState(0);

  // Memoize random startups to avoid recalculation on every render
  const { featuredStartups, testimonialStartups } = useMemo(() => {
    if (allStartups.length === 0) {
      return { featuredStartups: [], testimonialStartups: [] };
    }

    const getRandomStartups = (startups: Startup[], count: number = 3): Startup[] => {
      if (startups.length <= count) return startups;
      
      // Use rotationIndex to get different sets each time
      const startIndex = (rotationIndex * count) % startups.length;
      const selected = [];
      
      for (let i = 0; i < count; i++) {
        const index = (startIndex + i) % startups.length;
        selected.push(startups[index]);
      }
      
      return selected;
    };

    const featured = getRandomStartups(allStartups, 3);
    const startupsWithFounders = allStartups.filter(s => s.founders && s.founders.length > 0);
    const testimonials = getRandomStartups(startupsWithFounders, 3);

    return { featuredStartups: featured, testimonialStartups: testimonials };
  }, [allStartups, rotationIndex]);

  const generateTestimonialText = useCallback((startup: Startup): string => {
    const testimonials = {
      'Technology': "The incubator provided us with invaluable mentorship and cutting-edge resources that helped us scale our tech solutions from concept to market leader.",
      'Healthcare': "Thanks to the exceptional support network, we transformed our healthcare innovation from a prototype to a product that's making a real difference in patients' lives.",
      'Finance': "The fintech-focused guidance and industry connections helped us navigate regulatory challenges and achieve rapid growth in the financial sector.",
      'Education': "The collaborative environment and educational technology expertise here enabled us to create solutions that are revolutionizing how people learn.",
      'Environment': "With their sustainability focus and green tech network, we were able to develop and scale our environmental solutions beyond our wildest expectations.",
      'Energy': "The energy sector expertise and clean technology resources provided here were instrumental in bringing our renewable energy innovations to market.",
      'Default': "The incredible support system, mentorship, and collaborative environment here helped us transform our vision into a thriving business reality."
    };

    return testimonials[startup.sector as keyof typeof testimonials] || testimonials.Default;
  }, []);

  const rotateStartups = useCallback(() => {
    if (allStartups.length <= 3) return;
    setRotationIndex(prev => (prev + 1) % Math.ceil(allStartups.length / 3));
  }, [allStartups.length]);

  useEffect(() => {
    const fetchFeaturedStartups = async () => {
      try {
        setLoading(true);
        setError(null);
        const startups = await startupService.getAll();
        setAllStartups(startups);
      } catch (err) {
        console.error('Error fetching startups:', err);
        setError(err instanceof Error ? err.message : 'Failed to load featured projects');
      } finally {
        setLoading(false);
      }
    };

    fetchFeaturedStartups();
  }, []);

  useEffect(() => {
    if (allStartups.length <= 3) return;

    const interval = setInterval(() => {
      rotateStartups();
    }, 12000);

    return () => clearInterval(interval);
  }, [allStartups, rotateStartups]);
  return (
    <div>
      {}
      <section className="hero">
        <div className="hero-content">
          <h1>Showcase Innovation from JEB Incubator</h1>
          <p>
            Discover the most promising startups, connect with visionary entrepreneurs,
            and be part of the next generation of business leaders.
          </p>
          <Link to="/projects">
            <button className="cta-button">Explore Projects</button>
          </Link>
        </div>
      </section>

      {}
      <section className="container">
        <h2 style={{ textAlign: 'center', marginBottom: '3rem' }}>Featured Projects</h2>
        {loading ? (
          <div style={{ textAlign: 'center', padding: '3rem' }}>
            <div style={{
              width: '40px',
              height: '40px',
              border: '4px solid var(--lavender-light)',
              borderTop: '4px solid var(--purple-medium)',
              borderRadius: '50%',
              animation: 'spin 1s linear infinite',
              margin: '0 auto'
            }}></div>
            <p style={{ marginTop: '1rem', color: 'var(--purple-medium)' }}>Loading featured projects...</p>
          </div>
        ) : error ? (
          <div style={{ textAlign: 'center', padding: '3rem', color: 'var(--coral-medium)' }}>
            <p>{error}</p>
          </div>
        ) : (
          <div 
            className="featured-grid"
            style={{
              display: 'grid',
              gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))',
              gap: '2rem',
              transition: 'all 0.5s ease'
            }}
          >
            {featuredStartups.map((startup, index) => (
              <div 
                key={`${startup.id}-${rotationIndex}`} 
                className="featured-card"
                style={{
                  animation: `fadeInUp 1.2s ease-out ${index * 0.2}s both`,
                  transition: 'all 0.5s ease'
                }}
              >
                <div className="card">
                  <h3>{startup.name}</h3>
                  <p><strong>Sector:</strong> {startup.sector}</p>
                  <p><strong>Stage:</strong> {startup.maturity}</p>
                  <p>{startup.description || 'Innovative startup working on cutting-edge solutions.'}</p>
                  <Link to={`/projects?search=${encodeURIComponent(startup.name)}`}
                    style={{
                      color: 'var(--coral-medium)',
                      textDecoration: 'none',
                      fontWeight: '600'
                    }}>
                    Learn more →
                  </Link>
                </div>
              </div>
            ))}
            {featuredStartups.length === 0 && !loading && !error && (
              <div style={{ textAlign: 'center', padding: '3rem', gridColumn: '1 / -1' }}>
                <p style={{ color: 'var(--purple-medium)' }}>No featured projects available at the moment.</p>
                <Link to="/projects">
                  <button className="cta-button" style={{ marginTop: '1rem' }}>
                    Browse All Projects
                  </button>
                </Link>
              </div>
            )}
          </div>
        )}
      </section>

      {}
      {/* Community Testimonials Section */}
      <section style={{
        background: 'linear-gradient(135deg, var(--lavender-light), var(--pink-light))',
        padding: '4rem 2rem',
        marginTop: '4rem'
      }}>
        <div className="container">
          <h2 style={{ textAlign: 'center', marginBottom: '3rem' }}>What Our Community Says</h2>
          {loading ? (
            <div style={{ textAlign: 'center', padding: '3rem' }}>
              <div style={{
                width: '40px',
                height: '40px',
                border: '4px solid var(--lavender-light)',
                borderTop: '4px solid var(--purple-medium)',
                borderRadius: '50%',
                animation: 'spin 1s linear infinite',
                margin: '0 auto'
              }}></div>
              <p style={{ marginTop: '1rem', color: 'var(--purple-medium)' }}>Loading testimonials...</p>
            </div>
          ) : testimonialStartups.length > 0 ? (
            <div style={{
              display: 'grid',
              gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))',
              gap: '2rem'
            }}>
              {testimonialStartups.map((startup) => (
                <div key={startup.id} style={{
                  background: 'rgba(255, 255, 255, 0.8)',
                  padding: '2rem',
                  borderRadius: '15px',
                  boxShadow: '0 4px 20px rgba(0, 0, 0, 0.1)',
                  textAlign: 'center'
                }}>
                  <p style={{ 
                    fontStyle: 'italic', 
                    fontSize: '1.1rem', 
                    marginBottom: '1.5rem',
                    color: 'var(--gray-dark)' 
                  }}>
                    "{generateTestimonialText(startup)}"
                  </p>
                  <div style={{ color: 'var(--purple-dark)', fontWeight: '600' }}>
                    {startup.founders && startup.founders.length > 0 
                      ? startup.founders[0].name 
                      : 'Anonymous Founder'
                    }
                  </div>
                  <div style={{ color: 'var(--gray-medium)', fontSize: '0.9rem' }}>
                    {startup.founders && startup.founders.length > 0 
                      ? `Founder, ${startup.name}`
                      : `CEO, ${startup.name}`
                    }
                  </div>
                </div>
              ))}
            </div>
          ) : (
            // Fallback to static testimonials if no startups with founders
            <div style={{
              display: 'grid',
              gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))',
              gap: '2rem'
            }}>
              <div style={{
                background: 'rgba(255, 255, 255, 0.8)',
                padding: '2rem',
                borderRadius: '15px',
                boxShadow: '0 4px 20px rgba(0, 0, 0, 0.1)',
                textAlign: 'center'
              }}>
                <p style={{ 
                  fontStyle: 'italic', 
                  fontSize: '1.1rem', 
                  marginBottom: '1.5rem',
                  color: 'var(--gray-dark)' 
                }}>
                  "The incubator provided us with invaluable mentorship and resources that helped us scale from idea to market leader."
                </p>
                <div style={{ color: 'var(--purple-dark)', fontWeight: '600' }}>
                  Sarah Chen
                </div>
                <div style={{ color: 'var(--gray-medium)', fontSize: '0.9rem' }}>
                  CEO, TechVision AI
                </div>
              </div>
              <div style={{
                background: 'rgba(255, 255, 255, 0.8)',
                padding: '2rem',
                borderRadius: '15px',
                boxShadow: '0 4px 20px rgba(0, 0, 0, 0.1)',
                textAlign: 'center'
              }}>
                <p style={{ 
                  fontStyle: 'italic', 
                  fontSize: '1.1rem', 
                  marginBottom: '1.5rem',
                  color: 'var(--gray-dark)' 
                }}>
                  "Amazing network of entrepreneurs and investors. The collaborative environment here is truly inspiring."
                </p>
                <div style={{ color: 'var(--purple-dark)', fontWeight: '600' }}>
                  Marcus Rodriguez
                </div>
                <div style={{ color: 'var(--gray-medium)', fontSize: '0.9rem' }}>
                  Founder, GreenEnergy Solutions
                </div>
              </div>
              <div style={{
                background: 'rgba(255, 255, 255, 0.8)',
                padding: '2rem',
                borderRadius: '15px',
                boxShadow: '0 4px 20px rgba(0, 0, 0, 0.1)',
                textAlign: 'center'
              }}>
                <p style={{ 
                  fontStyle: 'italic', 
                  fontSize: '1.1rem', 
                  marginBottom: '1.5rem',
                  color: 'var(--gray-dark)' 
                }}>
                  "From prototype to product launch in 18 months. The support system here is unmatched."
                </p>
                <div style={{ color: 'var(--purple-dark)', fontWeight: '600' }}>
                  Alex Thompson
                </div>
                <div style={{ color: 'var(--gray-medium)', fontSize: '0.9rem' }}>
                  Co-founder, HealthTech Innovation
                </div>
              </div>
            </div>
          )}
        </div>
      </section>

      {}
      <section className="container" style={{ textAlign: 'center', padding: '4rem 2rem' }}>
        <h2>Ready to Join Our Innovation Community?</h2>
        <p style={{ fontSize: '1.1rem', marginBottom: '2rem', color: 'var(--gray-medium)' }}>
          Whether you're an entrepreneur, investor, or partner, discover the opportunities that await.
        </p>
        <div style={{ display: 'flex', gap: '1rem', justifyContent: 'center', flexWrap: 'wrap' }}>
          <Link to="/projects">
            <button className="cta-button">Browse Projects</button>
          </Link>
          <Link to="/events">
            <button className="cta-button" style={{
              background: 'transparent',
              color: 'var(--purple-dark)',
              border: '2px solid var(--purple-dark)'
            }}>
              Upcoming Events
            </button>
          </Link>
        </div>
      </section>
    </div>
  );
};

export default Home;