import React from 'react';
import ReactMarkdown from 'react-markdown';
import remarkGfm from 'remark-gfm';
import rehypeHighlight from 'rehype-highlight';
import 'highlight.js/styles/github.css';

interface MarkdownContentProps {
  content: string;
  className?: string;
  maxLength?: number;
}

const MarkdownContent: React.FC<MarkdownContentProps> = ({
  content,
  className = '',
  maxLength
}) => {
  if (!content) return null;

  const displayContent = maxLength && content.length > maxLength 
    ? content.substring(0, maxLength) + '...'
    : content;

  return (
    <div className={`markdown-content ${className}`}>
      <ReactMarkdown
        remarkPlugins={[remarkGfm]}
        rehypePlugins={[rehypeHighlight]}
        components={{
          h1: ({ children }) => <h1 style={{ color: 'var(--purple-dark)', fontSize: '1.5rem', marginBottom: '1rem', borderBottom: '2px solid var(--coral-medium)', paddingBottom: '0.5rem' }}>{children}</h1>,
          h2: ({ children }) => <h2 style={{ color: 'var(--purple-medium)', fontSize: '1.3rem', marginBottom: '0.8rem', borderBottom: '1px solid var(--coral-light)', paddingBottom: '0.3rem' }}>{children}</h2>,
          h3: ({ children }) => <h3 style={{ color: 'var(--coral-medium)', fontSize: '1.1rem', marginBottom: '0.6rem' }}>{children}</h3>,
          h4: ({ children }) => <h4 style={{ color: 'var(--coral-medium)', fontSize: '1rem', marginBottom: '0.5rem' }}>{children}</h4>,
          h5: ({ children }) => <h5 style={{ color: 'var(--coral-medium)', fontSize: '0.9rem', marginBottom: '0.4rem' }}>{children}</h5>,
          h6: ({ children }) => <h6 style={{ color: 'var(--coral-medium)', fontSize: '0.8rem', marginBottom: '0.3rem' }}>{children}</h6>,
          p: ({ children }) => <p style={{ marginBottom: '1rem', lineHeight: '1.6' }}>{children}</p>,
          strong: ({ children }) => <strong style={{ color: 'var(--purple-medium)', fontWeight: '600' }}>{children}</strong>,
          em: ({ children }) => <em style={{ color: 'var(--coral-medium)', fontStyle: 'italic' }}>{children}</em>,
          a: ({ href, children }) => (
            <a 
              href={href} 
              style={{ 
                color: 'var(--coral-medium)', 
                textDecoration: 'none',
                borderBottom: '1px solid transparent',
                transition: 'border-bottom 0.3s ease'
              }}
              onMouseEnter={(e) => e.currentTarget.style.borderBottom = '1px solid var(--coral-medium)'}
              onMouseLeave={(e) => e.currentTarget.style.borderBottom = '1px solid transparent'}
            >
              {children}
            </a>
          ),
          ul: ({ children }) => <ul style={{ marginBottom: '1rem', paddingLeft: '1.5rem' }}>{children}</ul>,
          ol: ({ children }) => <ol style={{ marginBottom: '1rem', paddingLeft: '1.5rem' }}>{children}</ol>,
          li: ({ children }) => <li style={{ marginBottom: '0.25rem' }}>{children}</li>,
          blockquote: ({ children }) => (
            <blockquote style={{ 
              borderLeft: '4px solid var(--coral-medium)', 
              paddingLeft: '1rem', 
              margin: '1rem 0', 
              fontStyle: 'italic',
              color: '#666',
              backgroundColor: '#f8f9fa',
              padding: '1rem',
              borderRadius: '4px'
            }}>
              {children}
            </blockquote>
          ),
          code: ({ children, className }) => {
            const isInline = !className;
            if (isInline) {
              return (
                <code style={{ 
                  backgroundColor: '#f1f3f4', 
                  padding: '0.2rem 0.4rem', 
                  borderRadius: '3px',
                  fontFamily: 'Monaco, Consolas, "Courier New", monospace',
                  fontSize: '0.9em',
                  color: '#d63384'
                }}>
                  {children}
                </code>
              );
            }
            return <code className={className}>{children}</code>;
          },
          pre: ({ children }) => (
            <pre style={{ 
              backgroundColor: '#f8f9fa', 
              padding: '1rem', 
              borderRadius: '6px', 
              overflow: 'auto',
              marginBottom: '1rem',
              border: '1px solid #e9ecef'
            }}>
              {children}
            </pre>
          ),
          table: ({ children }) => (
            <div style={{ overflowX: 'auto', marginBottom: '1rem' }}>
              <table style={{ 
                width: '100%', 
                borderCollapse: 'collapse',
                border: '1px solid #dee2e6'
              }}>
                {children}
              </table>
            </div>
          ),
          thead: ({ children }) => (
            <thead style={{ backgroundColor: '#f8f9fa' }}>
              {children}
            </thead>
          ),
          th: ({ children }) => (
            <th style={{ 
              padding: '0.75rem', 
              border: '1px solid #dee2e6',
              fontWeight: '600',
              textAlign: 'left'
            }}>
              {children}
            </th>
          ),
          td: ({ children }) => (
            <td style={{ 
              padding: '0.75rem', 
              border: '1px solid #dee2e6'
            }}>
              {children}
            </td>
          ),
          hr: () => (
            <hr style={{ 
              border: 'none',
              height: '2px',
              background: 'linear-gradient(90deg, var(--coral-medium), var(--purple-medium))',
              margin: '2rem 0'
            }} />
          )
        }}
      >
        {displayContent}
      </ReactMarkdown>
    </div>
  );
};

export default MarkdownContent;
