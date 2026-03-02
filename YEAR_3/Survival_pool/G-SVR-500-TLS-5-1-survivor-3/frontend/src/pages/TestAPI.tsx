import React, { useState } from 'react';
import { api, Startup } from '../services/api';

const TestAPI: React.FC = () => {
  const [data, setData] = useState<Startup[] | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  const testAPI = async () => {
    setLoading(true);
    setError('');
    try {
      const response = await api.get('/startups/');
      setData(response.data);
    } catch (err) {
      setError('API test failed');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="p-6">
      <h1 className="text-2xl font-bold mb-4">API Test</h1>
      <button
        onClick={testAPI}
        disabled={loading}
        className="bg-blue-500 text-white px-4 py-2 rounded disabled:opacity-50"
      >
        {loading ? 'Testing...' : 'Test API'}
      </button>
      {error && <div className="text-red-500 mt-2">{error}</div>}
      {data && (
        <div className="mt-4">
          <h2 className="text-lg font-semibold">Response:</h2>
          <pre className="bg-gray-100 p-4 rounded overflow-auto">
            {JSON.stringify(data, null, 2)}
          </pre>
        </div>
      )}
    </div>
  );
};

export default TestAPI;
