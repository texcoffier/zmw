<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="html"/>

  <xsl:template match="/">
    <html>
      <head>
        <title>Preferences for 'book' application</title>
      </head>
      <body bgcolor="#FFFFFF">

        <h1>Preferences for 'book' application</h1>

        <table border='1'>
          <caption>Interface colors</caption>
          <tbody>
            <TR>
              <TH>Name</TH>
              <TH>Red</TH>
              <TH>Green</TH>
              <TH>Blue</TH>
            </TR>
            <xsl:for-each select="//color">
              <TR>
                <TH><xsl:value-of select="@name"/></TH>
                <TD><xsl:value-of select="@r"/></TD>
                <TD><xsl:value-of select="@g"/></TD>
                <TD><xsl:value-of select="@b"/></TD>
              </TR>
            </xsl:for-each>
          </tbody>
        </table>
        <p></p>
        
        <table border='1'>
          <caption>Columns attributes</caption>
          <tbody>
            <TR>
              <TH>Name</TH>
              <TH>Width (pixels)</TH>
              <TH>Visible</TH>
              <TH>Default for 'New Book'</TH>
            </TR>
            <xsl:for-each select="//column">
              <TR>
                <TH><xsl:value-of select="@name"/></TH>
                <TD><xsl:value-of select="@width"/></TD>
                <TD><xsl:value-of select="@visible"/></TD>
                <TD><xsl:value-of select="@default"/></TD>
              </TR>
            </xsl:for-each>
          </tbody>
        </table>

        <xsl:apply-templates/>

      </body>
    </html>
  </xsl:template>

  <xsl:template match="color|book-preferences|column">
    <xsl:apply-templates/>
  </xsl:template>
  
  <xsl:template match="column-order">
    <p>Columns are displayed in the following order:
    <xsl:apply-templates/>
  </p>
</xsl:template>

<xsl:template match="column-name">
  [<xsl:value-of select="text()"/>]
</xsl:template>

<xsl:template match="language">
  User Interface language: <xsl:value-of select="@name"/>
</xsl:template>

<xsl:template match="*">
  <p>Unknown tag: <xsl:value-of select="local-name()"/></p>
  <xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>
