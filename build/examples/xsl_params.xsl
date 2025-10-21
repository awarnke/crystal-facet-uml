<?xml version='1.0' encoding="iso-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
  <xsl:param name="toc.section.depth">4</xsl:param>
  <xsl:param name="term.breakline">0</xsl:param>
  <!-- TOC links in the titles, and in blue. -->
  <xsl:param name="latex.hyperparam">colorlinks,linkcolor=blue,pdfstartview=FitH</xsl:param>

  <xsl:template match="emphasis">
    <xsl:param name="content">
      <xsl:apply-templates/>
    </xsl:param>
    <xsl:text>{\emph{\color{gray}</xsl:text>
    <xsl:copy-of select="$content"/>
    <xsl:text>}}</xsl:text>
  </xsl:template>

  <xsl:template match="token[@role='id']">
    <xsl:param name="content">
      <xsl:apply-templates/>
    </xsl:param>
    <xsl:text>\fcolorbox{lightgray}{white}{</xsl:text>
    <xsl:copy-of select="$content"/>
    <xsl:text>}</xsl:text>
  </xsl:template>

  <xsl:template match="token[@role='type']">
    <xsl:param name="content">
      <xsl:apply-templates/>
    </xsl:param>
    <xsl:text>\color{teal}</xsl:text>
    <xsl:copy-of select="$content"/>
    <xsl:text>\color{black}\hspace{0.1cm}</xsl:text>
  </xsl:template>

  <xsl:template match="token[@role='indent']">
    <xsl:param name="content">
      <xsl:apply-templates/>
    </xsl:param>
    <xsl:text>\hspace{0.9cm}</xsl:text>
  </xsl:template>

</xsl:stylesheet>
