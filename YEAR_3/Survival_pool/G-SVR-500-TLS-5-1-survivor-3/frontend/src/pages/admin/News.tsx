import React from 'react';

const AdminNews: React.FC = () => {
  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <h1>News Management</h1>
      <p>News CRUD</p>

      <div style={{ marginTop: '2rem' }}>
        <button style={{
          padding: '0.75rem 1.5rem',
          backgroundColor: 'var(--coral-medium)',
          color: 'white',
          border: 'none',
          borderRadius: '4px',
          marginBottom: '1rem'
        }}>
          + Add News
        </button>

        <div className="card">
          <h3>News List</h3>
          <div style={{ overflowX: 'auto' }}>
            <table style={{ width: '100%', borderCollapse: 'collapse' }}>
              <thead>
                <tr style={{ backgroundColor: '#f5f5f5' }}>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>ID</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Title</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Category</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Date</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Actions</th>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>1</td>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>New startup incubated</td>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Startup</td>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>2024-01-15</td>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>
                    <button style={{ marginRight: '0.5rem', padding: '0.25rem 0.5rem' }}>Edit</button>
                    <button style={{ padding: '0.25rem 0.5rem', backgroundColor: 'red', color: 'white', border: 'none' }}>Delete</button>
                  </td>
                </tr>
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </div>
  );
};

export default AdminNews;
