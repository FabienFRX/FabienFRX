import React from 'react';

const AdminUsers: React.FC = () => {
  return (
    <div className="container" style={{ paddingTop: '2rem' }}>
      <h1>User Management</h1>
      <p>Role and permission management</p>

      <div style={{ marginTop: '2rem' }}>
        <button style={{
          padding: '0.75rem 1.5rem',
          backgroundColor: 'var(--coral-medium)',
          color: 'white',
          border: 'none',
          borderRadius: '4px',
          marginBottom: '1rem'
        }}>
          + Add User
        </button>

        <div className="card">
          <h3>User List</h3>
          <div style={{ overflowX: 'auto' }}>
            <table style={{ width: '100%', borderCollapse: 'collapse' }}>
              <thead>
                <tr style={{ backgroundColor: '#f5f5f5' }}>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>ID</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Name</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Email</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Role</th>
                  <th style={{ padding: '0.75rem', border: '1px solid #ddd' }}>Actions</th>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>1</td>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>admin</td>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>admin@jeb.com</td>
                  <td style={{ padding: '0.75rem', border: '1px solid #ddd' }}>admin</td>
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

export default AdminUsers;
