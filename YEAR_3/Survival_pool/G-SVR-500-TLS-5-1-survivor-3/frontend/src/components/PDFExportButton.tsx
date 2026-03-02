import React, { useState } from 'react';
import { Startup } from '../services/api';
import { generateQuickPitchPDF, exportStartupList } from '../utils/pdfExport';

interface PDFExportButtonProps {
  startup?: Startup;
  startups?: Startup[];
  type: 'single' | 'list' | 'custom';
  children?: React.ReactNode;
  className?: string;
  variant?: 'primary' | 'secondary' | 'outline';
}

const PDFExportButton: React.FC<PDFExportButtonProps> = ({ 
  startup, 
  startups, 
  type, 
  children, 
  className = '',
  variant = 'primary'
}) => {
  const [isExporting, setIsExporting] = useState(false);

  const handleExport = async () => {
    setIsExporting(true);
    try {
      switch (type) {
        case 'single':
          if (startup) {
            await generateQuickPitchPDF(startup);
          }
          break;
        case 'list':
          if (startups && startups.length > 0) {
            await exportStartupList(startups);
          }
          break;
        default:
          console.warn('Custom export type requires additional implementation');
      }
    } catch (error) {
      console.error('Export failed:', error);
      alert('Failed to export PDF. Please try again.');
    } finally {
      setIsExporting(false);
    }
  };

  const getButtonText = () => {
    if (isExporting) return 'Generating PDF...';
    if (children) return children;
    
    switch (type) {
      case 'single':
        return '📄 Export PDF';
      case 'list':
        return '📊 Export Directory';
      default:
        return '📄 Export';
    }
  };

  const getButtonClass = () => {
    const baseClass = 'pdf-export-btn';
    const variantClass = `pdf-export-btn--${variant}`;
    return `${baseClass} ${variantClass} ${className}`.trim();
  };

  return (
    <button 
      onClick={handleExport}
      disabled={isExporting || (type === 'single' && !startup) || (type === 'list' && (!startups || startups.length === 0))}
      className={getButtonClass()}
      title={type === 'single' ? 'Export startup profile as PDF' : 'Export startup directory as PDF'}
    >
      {getButtonText()}
    </button>
  );
};

export default PDFExportButton;
