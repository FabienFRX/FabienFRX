from io import BytesIO
from django.http import HttpResponse
from django.template.loader import render_to_string
from reportlab.lib.pagesizes import letter, A4
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, Image
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import inch
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_LEFT, TA_RIGHT
from datetime import datetime
from .models import Startup, Founder

class PDFExportService:
    """Service for generating PDF exports of startup data"""
    
    def __init__(self):
        self.styles = getSampleStyleSheet()
        self.setup_custom_styles()
    
    def setup_custom_styles(self):
        """Setup custom styles for the PDF"""
        # Title style
        self.title_style = ParagraphStyle(
            'CustomTitle',
            parent=self.styles['Heading1'],
            fontSize=24,
            spaceAfter=30,
            textColor=colors.HexColor('#c174f2'),  # purple-dark
            alignment=TA_CENTER
        )
        
        # Subtitle style
        self.subtitle_style = ParagraphStyle(
            'CustomSubtitle',
            parent=self.styles['Heading2'],
            fontSize=16,
            spaceAfter=20,
            textColor=colors.HexColor('#f49c9c'),  # coral-medium
            alignment=TA_LEFT
        )
        
        # Body style
        self.body_style = ParagraphStyle(
            'CustomBody',
            parent=self.styles['Normal'],
            fontSize=11,
            spaceAfter=12,
            alignment=TA_LEFT
        )
        
        # Info style
        self.info_style = ParagraphStyle(
            'CustomInfo',
            parent=self.styles['Normal'],
            fontSize=10,
            textColor=colors.grey,
            alignment=TA_LEFT
        )
    
    def generate_startup_profile_pdf(self, startup_id):
        """Generate a PDF profile for a single startup"""
        try:
            startup = Startup.objects.get(id=startup_id)
        except Startup.DoesNotExist:
            return None
            
        buffer = BytesIO()
        doc = SimpleDocTemplate(buffer, pagesize=A4, topMargin=1*inch)
        story = []
        
        # Title
        story.append(Paragraph(startup.name, self.title_style))
        story.append(Spacer(1, 20))
        
        # Sector and maturity badges
        sector_text = f"<b>Sector:</b> {startup.sector} | <b>Maturity:</b> {startup.maturity}"
        story.append(Paragraph(sector_text, self.body_style))
        story.append(Spacer(1, 15))
        
        # Description
        if startup.description:
            story.append(Paragraph("Description", self.subtitle_style))
            story.append(Paragraph(startup.description, self.body_style))
            story.append(Spacer(1, 15))
        
        # Contact Information
        story.append(Paragraph("Contact Information", self.subtitle_style))
        contact_data = [
            ['Email:', startup.email or 'N/A'],
            ['Phone:', startup.phone or 'N/A'],
            ['Address:', startup.address or 'N/A'],
            ['Website:', startup.website_url or 'N/A'],
            ['Social Media:', startup.social_media_url or 'N/A'],
        ]
        
        contact_table = Table(contact_data, colWidths=[2*inch, 4*inch])
        contact_table.setStyle(TableStyle([
            ('BACKGROUND', (0, 0), (0, -1), colors.HexColor('#f8cacf')),  # pink-light
            ('TEXTCOLOR', (0, 0), (0, -1), colors.black),
            ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
            ('FONTNAME', (0, 0), (0, -1), 'Helvetica-Bold'),
            ('FONTNAME', (1, 0), (1, -1), 'Helvetica'),
            ('FONTSIZE', (0, 0), (-1, -1), 10),
            ('GRID', (0, 0), (-1, -1), 1, colors.black),
            ('VALIGN', (0, 0), (-1, -1), 'TOP'),
        ]))
        story.append(contact_table)
        story.append(Spacer(1, 20))
        
        # Project Details
        if startup.project_status or startup.needs or startup.progress:
            story.append(Paragraph("Project Details", self.subtitle_style))
            
            if startup.project_status:
                story.append(Paragraph(f"<b>Status:</b> {startup.project_status}", self.body_style))
            
            if startup.needs:
                story.append(Paragraph("<b>Current Needs:</b>", self.body_style))
                story.append(Paragraph(startup.needs, self.body_style))
                story.append(Spacer(1, 10))
            
            if startup.progress:
                story.append(Paragraph("<b>Progress & Achievements:</b>", self.body_style))
                story.append(Paragraph(startup.progress, self.body_style))
                story.append(Spacer(1, 10))
        
        # Founders
        founders = startup.founders.all()
        if founders.exists():
            story.append(Paragraph("Founders", self.subtitle_style))
            founder_data = [['Name']]
            for founder in founders:
                founder_data.append([founder.name])
            
            founder_table = Table(founder_data, colWidths=[4*inch])
            founder_table.setStyle(TableStyle([
                ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor('#e4bef8')),  # lavender-medium
                ('TEXTCOLOR', (0, 0), (-1, 0), colors.black),
                ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
                ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
                ('FONTNAME', (0, 1), (-1, -1), 'Helvetica'),
                ('FONTSIZE', (0, 0), (-1, -1), 10),
                ('GRID', (0, 0), (-1, -1), 1, colors.black),
            ]))
            story.append(founder_table)
            story.append(Spacer(1, 20))
        
        # Footer
        story.append(Spacer(1, 50))
        footer_text = f"Generated by JEB Incubator Platform | {datetime.now().strftime('%B %d, %Y')}"
        story.append(Paragraph(footer_text, self.info_style))
        
        doc.build(story)
        buffer.seek(0)
        return buffer
    
    def generate_startup_directory_pdf(self, startups_queryset=None):
        """Generate a PDF directory of all startups"""
        if startups_queryset is None:
            startups = Startup.objects.all().order_by('name')
        else:
            startups = startups_queryset
            
        buffer = BytesIO()
        doc = SimpleDocTemplate(buffer, pagesize=A4, topMargin=1*inch)
        story = []
        
        # Title
        story.append(Paragraph("JEB Incubator - Startup Directory", self.title_style))
        story.append(Spacer(1, 20))
        
        # Summary
        total_startups = startups.count()
        sectors = startups.values_list('sector', flat=True).distinct().count()
        summary_text = f"<b>Total Startups:</b> {total_startups} | <b>Sectors Represented:</b> {sectors}"
        story.append(Paragraph(summary_text, self.body_style))
        story.append(Spacer(1, 30))
        
        # Startup list
        for index, startup in enumerate(startups, 1):
            # Startup header
            startup_title = f"{index}. {startup.name}"
            story.append(Paragraph(startup_title, self.subtitle_style))
            
            # Startup info
            info_data = [
                ['Sector:', startup.sector],
                ['Maturity:', startup.maturity],
                ['Email:', startup.email],
                ['Status:', startup.project_status or 'N/A'],
            ]
            
            info_table = Table(info_data, colWidths=[1.5*inch, 4*inch])
            info_table.setStyle(TableStyle([
                ('BACKGROUND', (0, 0), (0, -1), colors.HexColor('#eed5fb')),  # lavender-light
                ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
                ('FONTNAME', (0, 0), (0, -1), 'Helvetica-Bold'),
                ('FONTSIZE', (0, 0), (-1, -1), 9),
                ('GRID', (0, 0), (-1, -1), 0.5, colors.grey),
                ('VALIGN', (0, 0), (-1, -1), 'TOP'),
            ]))
            story.append(info_table)
            
            # Brief description
            if startup.description:
                brief_desc = startup.description[:200] + "..." if len(startup.description) > 200 else startup.description
                story.append(Paragraph(brief_desc, self.info_style))
            
            story.append(Spacer(1, 15))
            
            # Page break every 3 startups
            if index % 3 == 0 and index < total_startups:
                story.append(Spacer(1, 50))
        
        # Footer
        story.append(Spacer(1, 30))
        footer_text = f"Generated by JEB Incubator Platform | {datetime.now().strftime('%B %d, %Y')} | Total: {total_startups} startups"
        story.append(Paragraph(footer_text, self.info_style))
        
        doc.build(story)
        buffer.seek(0)
        return buffer
    
    def generate_sector_analysis_pdf(self):
        """Generate a PDF with sector analysis"""
        buffer = BytesIO()
        doc = SimpleDocTemplate(buffer, pagesize=A4, topMargin=1*inch)
        story = []
        
        # Title
        story.append(Paragraph("JEB Incubator - Sector Analysis", self.title_style))
        story.append(Spacer(1, 30))
        
        # Get sector statistics
        from django.db.models import Count
        sector_stats = Startup.objects.values('sector').annotate(
            count=Count('id')
        ).order_by('-count')
        
        # Sector breakdown table
        story.append(Paragraph("Startups by Sector", self.subtitle_style))
        
        sector_data = [['Sector', 'Number of Startups', 'Percentage']]
        total_startups = Startup.objects.count()
        
        for sector_stat in sector_stats:
            sector = sector_stat['sector']
            count = sector_stat['count']
            percentage = round((count / total_startups) * 100, 1) if total_startups > 0 else 0
            sector_data.append([sector, str(count), f"{percentage}%"])
        
        sector_table = Table(sector_data, colWidths=[2.5*inch, 1.5*inch, 1*inch])
        sector_table.setStyle(TableStyle([
            ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor('#c174f2')),  # purple-dark
            ('TEXTCOLOR', (0, 0), (-1, 0), colors.white),
            ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
            ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
            ('FONTNAME', (0, 1), (-1, -1), 'Helvetica'),
            ('FONTSIZE', (0, 0), (-1, -1), 10),
            ('GRID', (0, 0), (-1, -1), 1, colors.black),
            ('ALTERNATEBACKCOLOR', (0, 1), (-1, -1), colors.HexColor('#f8cacf')),  # pink-light
        ]))
        story.append(sector_table)
        story.append(Spacer(1, 30))
        
        # Maturity analysis
        story.append(Paragraph("Startups by Maturity Level", self.subtitle_style))
        
        maturity_stats = Startup.objects.values('maturity').annotate(
            count=Count('id')
        ).order_by('-count')
        
        maturity_data = [['Maturity Level', 'Number of Startups']]
        for maturity_stat in maturity_stats:
            maturity_data.append([maturity_stat['maturity'], str(maturity_stat['count'])])
        
        maturity_table = Table(maturity_data, colWidths=[3*inch, 2*inch])
        maturity_table.setStyle(TableStyle([
            ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor('#f49c9c')),  # coral-medium
            ('TEXTCOLOR', (0, 0), (-1, 0), colors.white),
            ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
            ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
            ('FONTNAME', (0, 1), (-1, -1), 'Helvetica'),
            ('FONTSIZE', (0, 0), (-1, -1), 10),
            ('GRID', (0, 0), (-1, -1), 1, colors.black),
        ]))
        story.append(maturity_table)
        
        # Footer
        story.append(Spacer(1, 50))
        footer_text = f"Generated by JEB Incubator Platform | {datetime.now().strftime('%B %d, %Y')}"
        story.append(Paragraph(footer_text, self.info_style))
        
        doc.build(story)
        buffer.seek(0)
        return buffer
