import jsPDF from 'jspdf';
import { Startup } from '../services/api';

export interface PitchDeckData {
  startup: Startup;
  founders: Array<{
    name: string;
    role?: string;
  }>;
  financials?: {
    currentFunding?: string;
    targetFunding?: string;
    revenue?: string;
    projectedRevenue?: string;
  };
  metrics?: {
    users?: number;
    growth?: string;
    marketSize?: string;
  };
  competitive?: {
    advantages: string[];
    competitors: string[];
  };
}

export const generatePitchDeckPDF = async (
  data: PitchDeckData
): Promise<void> => {
  try {
    const pdf = new jsPDF({
      orientation: 'landscape',
      unit: 'mm',
      format: [297, 167]
    });

    const pageWidth = pdf.internal.pageSize.getWidth();
    const pageHeight = pdf.internal.pageSize.getHeight();
    const margin = 15;
    const availableWidth = pageWidth - (margin * 2);

    // Brand colors
    const primaryColor = [120, 53, 231]; // purple
    const secondaryColor = [239, 68, 68]; // coral
    const darkGray = [55, 65, 81];
    const lightGray = [156, 163, 175];

    const addSlideHeaderFooter = (title: string) => {
      // Add header
      pdf.setFillColor(primaryColor[0], primaryColor[1], primaryColor[2]);
      pdf.rect(0, 0, pageWidth, 30, 'F');
      
      pdf.setTextColor(255, 255, 255);
      pdf.setFontSize(20);
      pdf.setFont('helvetica', 'bold');
      pdf.text(title, margin, 20);
      
      pdf.setFontSize(16);
      const companyName = data.startup.name;
      const companyNameWidth = pdf.getTextWidth(companyName);
      pdf.text(companyName, pageWidth - margin - companyNameWidth, 20);

      // Add footer
      pdf.setFillColor(lightGray[0], lightGray[1], lightGray[2]);
      pdf.rect(0, pageHeight - 12, pageWidth, 12, 'F');
      pdf.setTextColor(255, 255, 255);
      pdf.setFontSize(10);
      pdf.text('JEB Incubator - Pitch Deck', margin, pageHeight - 4);
    };

    // Slide 1
    addSlideHeaderFooter('Company Overview');
    
    let yPos = 50;
    
    pdf.setTextColor(darkGray[0], darkGray[1], darkGray[2]);
    pdf.setFontSize(36);
    pdf.setFont('helvetica', 'bold');
    pdf.text(data.startup.name, margin, yPos);
    
    yPos += 25;
    
    pdf.setFontSize(16);
    pdf.setFont('helvetica', 'normal');
    const descriptionLines = pdf.splitTextToSize(data.startup.description || 'No description available', availableWidth);
    pdf.text(descriptionLines, margin, yPos);
    
    yPos += Math.max(30, descriptionLines.length * 6 + 10);
    
    const overviewData = [
      ['Sector', data.startup.sector || 'Not specified'],
      ['Maturity', data.startup.maturity || 'Not specified']
    ];
    
    overviewData.forEach((item, index) => {
      const xPos = margin + index * (availableWidth / 2);
      
      // Create a card background
      pdf.setFillColor(248, 250, 252);
      pdf.roundedRect(xPos, yPos, (availableWidth / 2) - 15, 30, 8, 8, 'F');
      
      // Label
      pdf.setFont('helvetica', 'bold');
      pdf.setFontSize(14);
      pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
      pdf.text(item[0], xPos + 10, yPos + 12);
      
      // Value
      pdf.setFont('helvetica', 'normal');
      pdf.setFontSize(16);
      pdf.setTextColor(darkGray[0], darkGray[1], darkGray[2]);
      pdf.text(item[1], xPos + 10, yPos + 22);
    });

    // Slide 2
    pdf.addPage();
    addSlideHeaderFooter('Startup Information');
    
    yPos = 50;
    
    const infoData = [
      ['Legal Status', data.startup.legal_status || 'Not specified'],
      ['Project Status', data.startup.project_status || 'Not specified'],
      ['Address', data.startup.address || 'Not specified'],
      ['Email', data.startup.email || 'Not specified'],
      ['Phone', data.startup.phone || 'Not specified'],
      ['Website', data.startup.website_url || 'Not specified']
    ];
    
    infoData.forEach((item, index) => {
      const col = index % 2;
      const row = Math.floor(index / 2);
      const xPos = margin + col * (availableWidth / 2);
      const yPosition = yPos + row * 35;
      
      // Create a card background
      pdf.setFillColor(248, 250, 252);
      pdf.roundedRect(xPos, yPosition, (availableWidth / 2) - 15, 30, 8, 8, 'F');
      
      // Label
      pdf.setFont('helvetica', 'bold');
      pdf.setFontSize(12);
      pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
      pdf.text(item[0], xPos + 10, yPosition + 12);
      
      // Value
      pdf.setFont('helvetica', 'normal');
      pdf.setFontSize(10);
      pdf.setTextColor(darkGray[0], darkGray[1], darkGray[2]);
      const cleanValue = String(item[1]).trim();
      const valueLines = pdf.splitTextToSize(cleanValue, (availableWidth / 2) - 25);
      pdf.text(valueLines, xPos + 10, yPosition + 22);
    });

    // Slide 3
    pdf.addPage();
    addSlideHeaderFooter('Financial Overview');
    
    yPos = 50;
    
    const financialData = [
      ['Current Funding', data.financials?.currentFunding || 'Not specified'],
      ['Target Funding', data.financials?.targetFunding || 'Not specified'],
      ['Current Revenue', data.financials?.revenue || 'Not specified'],
      ['Projected Revenue', data.financials?.projectedRevenue || 'Not specified']
    ];
    
    financialData.forEach((item, index) => {
      const col = index % 2;
      const row = Math.floor(index / 2);
      const xPos = margin + col * (availableWidth / 2);
      const yPosition = yPos + row * 50;
      
      // Create a card background
      pdf.setFillColor(248, 250, 252);
      pdf.roundedRect(xPos, yPosition, (availableWidth / 2) - 15, 40, 8, 8, 'F');
      
      // Label
      pdf.setFont('helvetica', 'bold');
      pdf.setFontSize(14);
      pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
      pdf.text(item[0], xPos + 15, yPosition + 15);
      
      // Value
      pdf.setFont('helvetica', 'normal');
      pdf.setFontSize(16);
      pdf.setTextColor(darkGray[0], darkGray[1], darkGray[2]);
      const cleanValue = String(item[1]).trim();
      const valueLines = pdf.splitTextToSize(cleanValue, (availableWidth / 2) - 35);
      pdf.text(valueLines, xPos + 15, yPosition + 28);
    });

    // Slide 4
    pdf.addPage();
    addSlideHeaderFooter('Key Metrics');
    
    yPos = 40;
    
    const metricsData = [
      ['User Base', data.metrics?.users?.toString() || 'Not specified'],
      ['Growth Rate', data.metrics?.growth || 'Not specified'],
      ['Market Size', data.metrics?.marketSize || 'Not specified']
    ];
    
    metricsData.forEach((item, index) => {
      const yPosition = yPos + index * 40;
      
      pdf.setFillColor(248, 250, 252);
      pdf.roundedRect(margin, yPosition, availableWidth, 35, 8, 8, 'F');
      
      // Label
      pdf.setFont('helvetica', 'bold');
      pdf.setFontSize(16);
      pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
      pdf.text(item[0], margin + 20, yPosition + 15);
      
      // Value
      pdf.setFont('helvetica', 'normal');
      pdf.setFontSize(18);
      pdf.setTextColor(darkGray[0], darkGray[1], darkGray[2]);
      pdf.text(String(item[1]).trim(), margin + 20, yPosition + 27);
    });

    // Slide 5
    pdf.addPage();
    addSlideHeaderFooter('Competitive Advantages');
    
    yPos = 50;
    
    if (data.competitive?.advantages && data.competitive.advantages.length > 0) {
      data.competitive.advantages.forEach((advantage, index) => {
        if (advantage.trim()) {
          const yPosition = yPos + index * 35;
          
          // Create a card background
          pdf.setFillColor(248, 250, 252);
          pdf.roundedRect(margin, yPosition, availableWidth, 30, 8, 8, 'F');
          
          // Bullet point
          pdf.setFillColor(secondaryColor[0], secondaryColor[1], secondaryColor[2]);
          pdf.circle(margin + 15, yPosition + 15, 4, 'F');
          
          // Advantage text
          pdf.setFont('helvetica', 'normal');
          pdf.setFontSize(14);
          pdf.setTextColor(darkGray[0], darkGray[1], darkGray[2]);
          const advantageLines = pdf.splitTextToSize(advantage, availableWidth - 60);
          pdf.text(advantageLines, margin + 30, yPosition + 18);
        }
      });
    } else {
      // Create a card for "no advantages"
      pdf.setFillColor(248, 250, 252);
      pdf.roundedRect(margin, yPos, availableWidth, 30, 8, 8, 'F');
      
      pdf.setFont('helvetica', 'italic');
      pdf.setFontSize(16);
      pdf.setTextColor(lightGray[0], lightGray[1], lightGray[2]);
      pdf.text('No competitive advantages specified', margin + 20, yPos + 18);
    }

    // Download the PDF
    const fileName = `${data.startup.name.replace(/[^a-z0-9]/gi, '_').toLowerCase()}_pitch_deck.pdf`;
    pdf.save(fileName);
    
  } catch (error) {
    console.error('Error generating PDF:', error);
    throw new Error('Failed to generate PDF. Please try again.');
  }
};

export const generateQuickPitchPDF = async (startup: Startup): Promise<void> => {
  const pdf = new jsPDF();
  
  // Set colors from your brand palette
  const primaryColor = [193, 116, 242]; // purple-dark
  const secondaryColor = [241, 133, 133]; // coral-light
  
  let yPosition = 30;
  
  // Header with company name
  pdf.setFontSize(24);
  pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
  pdf.text(startup.name, 20, yPosition);
  
  yPosition += 15;
  
  // Sector and maturity
  pdf.setFontSize(14);
  pdf.setTextColor(secondaryColor[0], secondaryColor[1], secondaryColor[2]);
  pdf.text(`${startup.sector} • ${startup.maturity}`, 20, yPosition);
  
  yPosition += 20;
  
  // Description
  pdf.setFontSize(12);
  pdf.setTextColor(0, 0, 0);
  if (startup.description) {
    const splitDescription = pdf.splitTextToSize(startup.description, 170);
    pdf.text('Description:', 20, yPosition);
    yPosition += 8;
    pdf.text(splitDescription, 20, yPosition);
    yPosition += splitDescription.length * 6 + 10;
  }
  
  // Contact information
  pdf.setFontSize(12);
  pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
  pdf.text('Contact Information:', 20, yPosition);
  yPosition += 8;
  
  pdf.setTextColor(0, 0, 0);
  if (startup.email) {
    pdf.text(`Email: ${startup.email}`, 20, yPosition);
    yPosition += 6;
  }
  
  if (startup.phone) {
    pdf.text(`Phone: ${startup.phone}`, 20, yPosition);
    yPosition += 6;
  }
  
  if (startup.website_url) {
    pdf.text(`Website: ${startup.website_url}`, 20, yPosition);
    yPosition += 6;
  }
  
  if (startup.address) {
    pdf.text(`Address: ${startup.address}`, 20, yPosition);
    yPosition += 10;
  }
  
  // Project status and needs
  if (startup.project_status) {
    pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
    pdf.text('Project Status:', 20, yPosition);
    yPosition += 8;
    pdf.setTextColor(0, 0, 0);
    pdf.text(startup.project_status, 20, yPosition);
    yPosition += 10;
  }
  
  if (startup.needs) {
    pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
    pdf.text('Current Needs:', 20, yPosition);
    yPosition += 8;
    pdf.setTextColor(0, 0, 0);
    const splitNeeds = pdf.splitTextToSize(startup.needs, 170);
    pdf.text(splitNeeds, 20, yPosition);
    yPosition += splitNeeds.length * 6 + 10;
  }
  
  if (startup.progress) {
    pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
    pdf.text('Progress & Achievements:', 20, yPosition);
    yPosition += 8;
    pdf.setTextColor(0, 0, 0);
    const splitProgress = pdf.splitTextToSize(startup.progress, 170);
    pdf.text(splitProgress, 20, yPosition);
  }
  
  // Footer
  const pageHeight = pdf.internal.pageSize.height;
  pdf.setFontSize(10);
  pdf.setTextColor(128, 128, 128);
  pdf.text('Generated by JEB Incubator Platform', 20, pageHeight - 10);
  pdf.text(new Date().toLocaleDateString(), 160, pageHeight - 10);
  
  // Download
  const fileName = `${startup.name.replace(/[^a-z0-9]/gi, '_').toLowerCase()}_profile.pdf`;
  pdf.save(fileName);
};

export const exportStartupList = async (startups: Startup[]): Promise<void> => {
  const pdf = new jsPDF();
  const primaryColor = [193, 116, 242];
  let yPosition = 30;
  
  // Title
  pdf.setFontSize(20);
  pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
  pdf.text('JEB Incubator - Startup Directory', 20, yPosition);
  yPosition += 20;
  
  startups.forEach((startup, index) => {
    // Check if we need a new page
    if (yPosition > 250) {
      pdf.addPage();
      yPosition = 30;
    }
    
    // Startup name
    pdf.setFontSize(14);
    pdf.setTextColor(primaryColor[0], primaryColor[1], primaryColor[2]);
    pdf.text(`${index + 1}. ${startup.name}`, 20, yPosition);
    yPosition += 8;
    
    // Sector and maturity
    pdf.setFontSize(10);
    pdf.setTextColor(0, 0, 0);
    pdf.text(`Sector: ${startup.sector} | Maturity: ${startup.maturity}`, 25, yPosition);
    yPosition += 6;
    
    // Contact
    if (startup.email) {
      pdf.text(`Email: ${startup.email}`, 25, yPosition);
      yPosition += 6;
    }
    
    // Brief description
    if (startup.description) {
      const briefDesc = startup.description.substring(0, 100) + (startup.description.length > 100 ? '...' : '');
      const splitDesc = pdf.splitTextToSize(briefDesc, 150);
      pdf.text(splitDesc, 25, yPosition);
      yPosition += splitDesc.length * 4;
    }
    
    yPosition += 8; // Space between startups
  });
  
  // Footer
  const pageCount = pdf.getNumberOfPages();
  for (let i = 1; i <= pageCount; i++) {
    pdf.setPage(i);
    pdf.setFontSize(10);
    pdf.setTextColor(128, 128, 128);
    pdf.text(`Page ${i} of ${pageCount}`, 20, pdf.internal.pageSize.height - 10);
    pdf.text(new Date().toLocaleDateString(), 160, pdf.internal.pageSize.height - 10);
  }
  
  pdf.save('jeb_startup_directory.pdf');
};
