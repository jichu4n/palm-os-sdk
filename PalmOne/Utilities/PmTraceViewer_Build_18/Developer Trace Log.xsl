<?xml version='1.0'?>

<!-- ______________________________________________________________
File: Developer Trace Log
Stylesheet: 
Project: 	DUO
Author: 	Kiran prasad
Description:  
  This is a sample XSL style sheet that will hopefully show how to
  some of the basic transformation that can be done to quickly isolate
  the problems within a log. You can use the style sheet as is and 
  all should work okay. Please read the comments above each section
  to understand how to custmoize the stylesheet for your own purposes.

Changes:
___________________________________________________________________
-->


<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

   <xsl:template match="SystemInfo"/>   

    <xsl:template match="TraceDB">
	  <table cellspacing="5" cellpadding="5">
	    <xsl:apply-templates select="TraceRec">
          <xsl:sort data-type="text" select="./Time[1]"/>
        </xsl:apply-templates>
	  </table>
	</xsl:template>
   
   <!--
   	This matches on the TraceRec node and checks if there is an Err sub node in the 
   	the trace record. If Err subnode exists then it prints the whole row in dark
   	red. It also checks if there is a data node on the given trace record and if
   	there is then it prints the row in grey.
   -->
   <xsl:template match="TraceRec">
      <xsl:choose>
         <xsl:when test="child::Data">
		   <tr>
		   <td><xsl:apply-templates select="Time" /></td>
		   <td>
		   <xsl:call-template name="Strip-Hex-Data">
		     <xsl:with-param name="text" select="Data" />
		   </xsl:call-template>
		   </td>
		   </tr>
         </xsl:when>
         <xsl:otherwise>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
   
   <xsl:template name="Strip-Hex-Data">
   <xsl:param name="text" />
      <xsl:choose>
         <xsl:when test="contains($text,'AT')">
            <font color="Blue">
  			  &gt;&gt;&gt; <xsl:value-of select="substring-after($text, '0D')" /> 
            </font>
         </xsl:when>

         <xsl:otherwise>
            <font color="DarkRed">
              &lt;&lt;&lt; <xsl:value-of select="substring-after($text, '0A')" /> 
            </font>
         </xsl:otherwise>
      </xsl:choose>
  
   </xsl:template>
   
   


</xsl:stylesheet>

