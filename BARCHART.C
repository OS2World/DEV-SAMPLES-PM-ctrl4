#pragma	title("Bar Chart Control  --  Version 1.0 -- (BarChart.C)")
#pragma	subtitle("  Bar Chart Control DLL - Interface Definitions")

#pragma	info(noext)

#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

static char *MODID = "@(#)barchart.c:1.00";

#include <malloc.h>
#include <math.h>
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	MAXTICK	100

#include <pmcx.h>

#include "barchart.h"

/* This	module contains	an example installable control that can	be used	*/
/* within applications where additional	facilities are provided	that	*/
/* are not found within	the default controls of	OS/2 PM.		*/
/*									*/
/* For complete	details	regarding the PM Control Extensions (PMCX)	*/
/* consult the User's Guide.                                            */
/*									*/
/* The DLL is created using the	following command line invocation:	*/
/*									*/
/*     Icc -G5e- -O+ -Rn -C BarChart.C					*/

/* Filename:   BarChart.C						*/

/*  Version:   1.0							*/
/*  Created:   1996-02-20						*/
/*  Revised:   1996-06-26						*/

/* Routines:   BOOL EXPENTRY BarChartRegister(HAB hAB);			*/
/*	       BOOL EXPENTRY BarChartQuery(PUSERINFO pControlInfo);	*/
/*	       static BOOL SetDlgItemLong(HWND hwndDlg,	ULONG idItem,	*/
/*					  ULONG	ulValue, BOOL fSigned);	*/
/*	       static BOOL QueryDlgItemLong(HWND hwndDlg, ULONG	idItem,	*/
/*					    PLONG pResult,		*/
/*					    BOOL fSigned);		*/
/*	       static VOID InitBarChartData(HWND hWnd,			*/
/*					    PBARCHARTCDATA pbccd);	*/
/*	       static VOID GetBarChartData(HWND	hWnd,			*/
/*					   PBARCHARTCDATA pbccd,	*/
/*					   PUSERSTYLE pust);		*/
/*	       MRESULT EXPENTRY	BarChartStyles(HWND hWnd, ULONG	msg,	*/
/*					       MPARAM mp1, MPARAM mp2);	*/
/*	       static LONG lGetPresParam(HWND hWnd, ULONG ulID1,	*/
/*					 ULONG ulID2, LONG lDefault);	*/
/*	       static ULONG ulNiceScale(ULONG ulMax);			*/
/*	       static VOID CalcSize(HWND hWnd, PRECTL prcl,		*/
/*				    PBARCHART pbc);			*/
/*	       static VOID DecodeCTLDATA(PBARCHART pbc,			*/
/*					 PBARCHARTCDATA	pbccd);		*/
/*	       static LONG lSelectFont(CHAR *pszFacename, LONG lPoints);*/
/*	       MRESULT EXPENTRY	BarChartWndProc(HWND hWnd, ULONG msg,	*/
/*						MPARAM mp1, MPARAM mp2);*/


/************************************************************************/
/************************************************************************/
/************************************************************************/
/* DISCLAIMER OF WARRANTIES:						*/
/* -------------------------						*/
/* The following [enclosed] code is sample code	created	by IBM		*/
/* Corporation and Prominare Inc.  This	sample code is not part	of any	*/
/* standard IBM	product	and is provided	to you solely for the purpose	*/
/* of assisting	you in the development of your applications.  The code	*/
/* is provided "AS IS",	without	warranty of any	kind.  Neither IBM nor	*/
/* Prominare shall be liable for any damages arising out of your	*/
/* use of the sample code, even	if they	have been advised of the	*/
/* possibility of such damages.						*/
/************************************************************************/
/************************************************************************/
/************************************************************************/
/*		       D I S C L A I M E R				*/
/* This	code is	provided on an as is basis with	no implied support.	*/
/* It should be	considered freeware that cannot	be rebundled as		*/
/* part	of a larger "*ware" offering without our consent.		*/
/************************************************************************/
/************************************************************************/
/************************************************************************/

/* Copyright ¸ International Business Machines Corp., 1995.		*/
/* Copyright ¸ 1995  Prominare Inc.  All Rights	Reserved.		*/

/* --- Module Data Definitions ----------------------------------------	*/

static GRADIENTL gradl = { 0L, 1L };

#define	RGB_DARKGRAY   0x00838383L
#define	RGB_PALEGRAY   0x00ccccccL

/* --- Module Prototype	Definitions -----------------------------------	*/

static BOOL  SetDlgItemLong(HWND hwndDlg, ULONG	idItem,	ULONG ulValue, BOOL fSigned);
static BOOL  QueryDlgItemLong(HWND hwndDlg, ULONG idItem, PLONG	pResult, BOOL fSigned);
static VOID  InitBarChartData(HWND hWnd, PBARCHARTCDATA	pbccd);
static VOID  GetBarChartData(HWND hWnd,	PBARCHARTCDATA pbccd, PUSERSTYLE pust);
static LONG  lGetPresParam(HWND	hWnd, ULONG ulID1, ULONG ulID2,	LONG lDefault);
static ULONG ulNiceScale(ULONG ulMax);
static VOID  DecodeCTLDATA(PBARCHART pbc, PBARCHARTCDATA pbccd);
static LONG  lSelectFont(CHAR *pszFacename, LONG lPoints);
static VOID  CalcSize(HWND hWnd, PRECTL	prcl, PBARCHART	pbc);

BOOL	EXPENTRY BarChartRegister(HAB hAB);
BOOL	EXPENTRY BarChartQuery(PUSERINFO pUserInfo);
MRESULT	EXPENTRY BarChartWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT	EXPENTRY BarChartStyles(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);

#pragma	subtitle("   Bar Chart Control DLL - Control Initialization Function")
#pragma	page ( )

/* --- BarChartRegister	-------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	register the installable control class	*/
/*     with OS/2 Presentation Manager.	The registration must use the	*/
/*     USER_CWINDOWWORDS to reserve memory for the control to allow for	*/
/*     proper usage by Resource	Editor and for use by the control	*/
/*     dialog and window procedures.  The information for the control	*/
/*     containing the style, presentation parameters and control data	*/
/*     is pointed to by	a pointer that can be referenced by the		*/
/*     control's dialog and window procedure as required.  The memory   */
/*     for the structure is allocated and controlled through Resource	*/
/*     Editor.	 The control can reserve more memory for its use	*/
/*     by adding the memory required to	that of	the USER_CWINDOWWORDS	*/
/*     constant.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HAB hAB;	= Application Anchor Block Handle			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     BarChartRegister	=  TRUE	: Class	Registration Successful		*/
/*			= FALSE	: Class	Registration Failed		*/
/*									*/
/* --------------------------------------------------------------------	*/

BOOL EXPENTRY BarChartRegister(HAB hAB)

{
		       /* Register the control class with OS/2		*/
		       /* Presentation Manager and return registration	*/
		       /* result					*/

return(WinRegisterClass(hAB, "BarChart", BarChartWndProc, CS_SYNCPAINT | CS_SIZEREDRAW,	USER_CWINDOWWORDS));

}
#pragma	subtitle("   Bar Chart Control DLL - Query Control Information Function")
#pragma	page ( )

/* --- BarChartQuery ----------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	return to the caller information	*/
/*     regarding the installable control and its capabilities.	The	*/
/*     function	should return a	true value otherwise Resource		*/
/*     Editor will not register	the control as being usable.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PUSERINFO pUserInfo; = User Information Pointer			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     BarChartQuery =	TRUE : User Information	Being Returned		*/
/*		     = FALSE : No User Information Available		*/
/*									*/
/* --------------------------------------------------------------------	*/

BOOL EXPENTRY BarChartQuery(PUSERINFO pUserInfo)

{
		       /* Complete the User Information	structure	*/
		       /* passed to the	function by Resource Editor	*/

		       /* Complete the version and number of control	*/
		       /* types.  In Version 1.00 of PMCX, only	one	*/
		       /* control type is used.				*/
pUserInfo->ulMajor = 3UL;
pUserInfo->ulMinor = 0UL;
		       /* Complete the author and control classname	*/

memcpy(pUserInfo->szAuthor,    "Prominare Inc.", 15);
memcpy(pUserInfo->szClassname, "BarChart", 9);
memcpy(pUserInfo->szName,      "BarChart", 9);

		       /* Complete the default size and	style of the	*/
		       /* first	user control type			*/

pUserInfo->utDefined[0].cx	   = 100L;
pUserInfo->utDefined[0].cy	   = 100L;
pUserInfo->utDefined[0].flStyle	   = WS_VISIBLE;

		       /* Set the maximum amount of text control can	*/
		       /* accept including NULL	termination byte	*/

pUserInfo->utDefined[0].cMaxText     = CCHTEXTMAX;

		       /* Save the style's dialogue ID, type, control   */
		       /* data size and	count of style masks		*/

pUserInfo->utDefined[0].idDlg	     = DLG_CTRLUSER;
pUserInfo->utDefined[0].ulType	     = UTYPE_PRIVATE;
pUserInfo->utDefined[0].flOptions    = PMCXOPT_VARICDATA | PMCXOPT_REFRESH;
pUserInfo->utDefined[0].cCtlData     = 0UL;
pUserInfo->utDefined[0].cMasks	     = 0UL;
pUserInfo->utDefined[0].flStyleType  = STYLETYPE_BITFLAGS;

		       /* Save the description of the control		*/

memcpy(pUserInfo->utDefined[0].szDescription, "Bar Chart", 10);

		       /* Return the success flag back to Resource	*/
		       /* Editor					*/
return(TRUE);
}
#pragma	subtitle("   Bar Chart Control DLL - Set Long Value Function")
#pragma	page( )

/* --- SetDlgItemLong ---------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	complement the WinSetDlgItemShort	*/
/*     function	by providing a LONG equivalent.				*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND  hwndDlg; =	Dialogue Window	Handle				*/
/*     ULONG idItem;  =	Item ID						*/
/*     ULONG ulValue; =	Long Value					*/
/*     BOOL  fSigned; =	Signed Flag					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     SetDlgItemLong =	 TRUE :	Successfully Set Value			*/
/*		      =	FALSE :	Error Occurred				*/
/*									*/
/* --------------------------------------------------------------------	*/

static BOOL SetDlgItemLong(HWND	hwndDlg, ULONG idItem, ULONG ulValue, BOOL fSigned)

{
CHAR szNumBuf[32];		   /* Character	Buffer			*/

return(WinSetDlgItemText(hwndDlg, idItem, fSigned ? _ltoa((LONG)ulValue, szNumBuf, 10) : _ultoa(ulValue, szNumBuf, 10)));
}
#pragma	subtitle("   Bar Chart Control DLL - Get Long Value Function")
#pragma	page( )

/* --- QueryDlgItemLong	-------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	complement the WinSetDlgItemShort	*/
/*     function	by providing a LONG equivalent.				*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND  hwndDlg; =	Dialogue Window	Handle				*/
/*     ULONG idItem;  =	Item ID						*/
/*     ULONG ulValue; =	Long Value					*/
/*     BOOL  fSigned; =	Signed Flag					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     QueryDlgItemLong	=  TRUE	: Successfully Retrived	Value		*/
/*			= FALSE	: Error	Occurred			*/
/*									*/
/* --------------------------------------------------------------------	*/

static BOOL QueryDlgItemLong(HWND hwndDlg, ULONG idItem, PLONG pResult,	BOOL fSigned)

{
CHAR  szNumBuf[32];		   /* Character	Buffer			*/
PCHAR pch;			   /* String Character Pointer		*/

		       /* Try to get the text from the item specified	*/

if ( WinQueryDlgItemText(hwndDlg, idItem, 32L, szNumBuf) )
   {
		       /* Check	to see if the item is to be signed or	*/
		       /* unsigned, and	when unsigned, strip out the	*/
		       /* minus	sign					*/

   if (	!fSigned && (pch = strchr(szNumBuf, '-')) )
       *pch = (CHAR)' ';
		       /* Convert the string to	a number and return the	*/
		       /* value	back through the long pointer		*/

   *pResult = atol(szNumBuf);
   return(TRUE);
   }
else
   {
   *pResult = 0L;
   return(FALSE);
   }
}
#pragma	subtitle("   Bar Chart Control DLL - Control Data Decoding Procedure")
#pragma	page ( )

/* --- InitNotebookPageData ---------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	decode the control data	for the		*/
/*     bar chart and place it within the appropriate controls		*/
/*     within the styles dialogue.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND	      hWnd;  = Dialogue	Window Handle			*/
/*     PBARCHARTCDATA pbccd; = Bar Chart CLTDATA Pointer		*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID InitBarChartData(HWND hWnd, PBARCHARTCDATA	pbccd)

{
CHAR   szBuffer[256];		   /* Buffer				*/
INT    cLen;			   /* Text Length			*/
IPT    iptData1;		   /* MLE Data Start Position		*/
IPT    iptData2;		   /* MLE Data Start Position		*/
PULONG pulValues;		   /* Values Array			*/
PBYTE  pb;			   /* List Pointer			*/
register INT i,	n;		   /* Loop Counters			*/

		       /* Set the item count in	the Items entry	field	*/

SetDlgItemLong(hWnd, EF_ITEMS, (ULONG)pbccd->cItems, TRUE);

		       /* Set the vertical scale text			*/

WinSetDlgItemText(hWnd,	EF_VERTTITLE, pbccd->cbVertTitle ? PSZVERTTITLE(pbccd) : "");

		       /* Set the horizontal scale text			*/

WinSetDlgItemText(hWnd,	EF_HORZTITLE, pbccd->cbHorzTitle ? PSZHORZTITLE(pbccd) : "");

		       /* Initialize the MLEs before placing scale text	*/
		       /* and values within them			*/

WinSendDlgItemMsg(hWnd,	MLE_VALUES, MLM_DISABLEREFRESH,	0L, 0L);
WinSendDlgItemMsg(hWnd,	MLE_VALUES, MLM_RESETUNDO, 0L, 0L);
WinSendDlgItemMsg(hWnd,	MLE_HORZSCALE, MLM_RESETUNDO, 0L, 0L);
WinSendDlgItemMsg(hWnd,	MLE_HORZSCALE, MLM_DISABLEREFRESH, 0L, 0L);

if ( pbccd->cItems )
   {
   pulValues = PULONGVALUES(pbccd);

		       /* Point	to the start of	the data block that	*/
		       /* contains the tab text				*/

   pb =	PBYTELIST(pbccd);

		       /* Walk the array and convert the ID to ASCII	*/
		       /* before placing the value on a	single line	*/
		       /* within the MLE				*/

   for ( i = n = 0, iptData1 = iptData2	= 0L; i	< pbccd->cItems; i++ )
       {
       WinSendDlgItemMsg(hWnd, MLE_VALUES, MLM_SETIMPORTEXPORT,	MPFROMP(szBuffer),
			 MPFROMLONG(cLen = (INT)strlen(strcat(_ltoa((LONG)pulValues[i],	szBuffer, 10), "\n"))));
       WinSendDlgItemMsg(hWnd, MLE_VALUES, MLM_IMPORT, MPFROMP(&iptData1), MPFROMLONG(cLen));

       WinSendDlgItemMsg(hWnd, MLE_HORZSCALE, MLM_SETIMPORTEXPORT, MPFROMP(szBuffer),
			 MPFROMLONG(cLen = (INT)strlen(strcat(strcpy(szBuffer, &pb[n]),	"\n"))));
       WinSendDlgItemMsg(hWnd, MLE_HORZSCALE, MLM_IMPORT, MPFROMP(&iptData2), MPFROMLONG(cLen));
       n += (INT)strlen(&pb[n])	+ 1;
       }
   WinSendDlgItemMsg(hWnd, MLE_VALUES,	MLM_ENABLEREFRESH, 0L, 0L);
   WinSendDlgItemMsg(hWnd, MLE_HORZSCALE,  MLM_ENABLEREFRESH, 0L, 0L);
   }
}
#pragma	subtitle("   Bar Chart Control DLL - Control Data Save Procedure")
#pragma	page ( )

/* --- GetBarChartData --------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used query the dialogue	controls containing	*/
/*     information for the bar chart for their data and	create		*/
/*     control data for	the control.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND	      hWnd;  = Dialogue	Window Handle			*/
/*     PNOTEPAGECDATA pnpcd; = Control Data Structure Pointer		*/
/*     PUSERSTYLE     pust;  = Control Styles Structure	Pointer		*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID GetBarChartData(HWND hWnd, PBARCHARTCDATA pbccd, PUSERSTYLE	pust)

{
CHAR   szBuffer[CCHMAXPATH];	   /* Buffer				*/
INT    cLen;			   /* Text Length			*/
INT    cTextLen;		   /* Text Length			*/
IPT    iptData;			   /* MLE Data Start Position		*/
PCHAR  pch;			   /* Scale Text Pointer		*/
LONG   cLines;			   /* Lines Count			*/
LONG   cItems =	0L;		   /* Item Count			*/
PULONG pulValues;		   /* Values Array			*/
LONG   cVertTitle;		   /* Verticle Title Length		*/
LONG   cHorzTitle;		   /* Horizontal Title Length		*/
PCHAR  pchScale;		   /* Scale Item Text Pointer		*/
LONG   cScales;			   /* Scales Count			*/
register INT i,	n;		   /* Loop Counters			*/

		       /* Get the number of items that are to be part	*/
		       /* of the bar chart				*/

QueryDlgItemLong(hWnd, EF_ITEMS, (PLONG)(PVOID)&cItems,	TRUE);

		       /* Set the vertical scale text length		*/

if ( (cVertTitle = WinQueryDlgItemTextLength(hWnd, EF_VERTTITLE)) != 0 )
   ++cVertTitle;
		       /* Set the horizontal scale text	length		*/

if ( (cHorzTitle = WinQueryDlgItemTextLength(hWnd, EF_HORZTITLE)) != 0 )
   ++cHorzTitle;
		       /* Get the number of lines within the dialogue	*/
		       /* ID's MLE                                      */

cLines = SHORT1FROMMR(WinSendDlgItemMsg(hWnd, MLE_VALUES, MLM_QUERYLINECOUNT, 0L, 0L));

		       /* Allocate scratch memory for the dialogue ID	*/
		       /* array						*/

pulValues = (PULONG)malloc((ULONG)(cItems < cLines ? cLines * sizeof(ULONG) : cItems * sizeof(ULONG)));

		       /* Get each line	from the MLE and convert the	*/
		       /* dialogue ID from ASCII to binary		*/

for ( i	= n = cItems = 0, iptData = 0L;	i < cLines; i++	)
   if (	(cLen =	(INT)(cTextLen = (LONG)WinSendDlgItemMsg(hWnd, MLE_VALUES, MLM_QUERYLINELENGTH,	MPFROMLONG(iptData), 0L))) != 0	)
       {
		       /* Get the line from the	MLE			*/

       WinSendDlgItemMsg(hWnd, MLE_VALUES, MLM_SETIMPORTEXPORT,	MPFROMP(szBuffer), MPFROMLONG((IPT)cTextLen));
       WinSendDlgItemMsg(hWnd, MLE_VALUES, MLM_EXPORT, MPFROMP(&iptData), MPFROMP(&cTextLen));

		       /* Terminate the	text at	the carriage return or	*/
		       /* line feed mark				*/

       szBuffer[cLen] =	(CHAR)0;
       if ( (pch = strchr(szBuffer, '\r')) != NULL )
	   *pch	= (CHAR)0;
       else
	   if (	(pch = strchr(szBuffer,	'\n')) != NULL )
	       *pch = (CHAR)0;

		       /* Convert the ASCII dialogue ID	to binary and	*/
		       /* save within the scratch ID array		*/

       if ( szBuffer[0]	)
	   pulValues[cItems++] = (ULONG)atol(szBuffer);
       }
		       /* Get the number of lines within the tab text	*/
		       /* MLE						*/

cLines = SHORT1FROMMR(WinSendDlgItemMsg(hWnd, MLE_HORZSCALE, MLM_QUERYLINECOUNT, 0L, 0L));

		       /* Allocate scratch memory for the tab text	*/

memset(pchScale	= (PCHAR)malloc(16384),	0, 16384);

		       /* Get each line	from the MLE and save the text	*/
		       /* within the tab text block			*/

for ( i	= n = 0, cScales = iptData = 0L; i < cLines; i++ )
   if (	(cLen =	(INT)(cTextLen = (LONG)WinSendDlgItemMsg(hWnd, MLE_HORZSCALE, MLM_QUERYLINELENGTH, MPFROMLONG(iptData),	0L))) != 0 )
       {
       WinSendDlgItemMsg(hWnd, MLE_HORZSCALE, MLM_SETIMPORTEXPORT, MPFROMP(szBuffer), MPFROMLONG((IPT)cTextLen));
       WinSendDlgItemMsg(hWnd, MLE_HORZSCALE, MLM_EXPORT, MPFROMP(&iptData), MPFROMP(&cTextLen));

       szBuffer[cLen] =	(CHAR)0;
       if ( (pch = strchr(szBuffer, '\r')) != NULL )
	   *pch	= (CHAR)0;
       else
	   if (	(pch = strchr(szBuffer,	'\n')) != NULL )
	       *pch = (CHAR)0;

		       /* Copy the text	into the tab text block		*/

       if ( szBuffer[0]	)
	   {
	   n +=	(INT)strlen(strcpy(&pchScale[n], szBuffer)) + 1;
	   ++cScales;
	   }
       }

if ( cScales !=	cItems )
   n +=	cItems - cScales;

		       /* Reallocate the memory	for the	control	data	*/
		       /* based	upon the needs of the data just		*/
		       /* retrieved from the MLE's                      */

if ( !(pust->pbCtlData = (PBYTE)pust->pfnRealloc(pust->pbCtlData, cHorzTitle + cVertTitle + cItems * sizeof(ULONG) +
						 n + sizeof(BARCHARTCDATA) - 1L)) )
   WinMessageBox(HWND_DESKTOP, hWnd, "Memory error on reallocating control data!", "Bar Chart Control",	0UL, MB_OK | MB_ICONEXCLAMATION);
else
   {
		       /* Save the number of bytes within the control	*/
		       /* data within the user styles info		*/

   pust->cbCtlData = (ULONG)(cHorzTitle	+ cVertTitle + cItems *	sizeof(ULONG) +	n + sizeof(BARCHARTCDATA) - 1L);

		       /* Point	to the control data block		*/

   pbccd = (PBARCHARTCDATA)pust->pbCtlData;

		       /* Save the size	of the control data block	*/
		       /* within the block start as required by	OS/2 PM	*/

   pbccd->cb = pust->cbCtlData;

		       /* Get the title	text if	entered			*/

   if (	(pbccd->cbVertTitle = cVertTitle) != 0UL )
       WinQueryDlgItemText(hWnd, EF_VERTTITLE, cVertTitle, PSZVERTTITLE(pbccd));
   if (	(pbccd->cbHorzTitle = cHorzTitle) != 0UL )
       WinQueryDlgItemText(hWnd, EF_HORZTITLE, cHorzTitle, PSZHORZTITLE(pbccd));

		       /* Copy the values to the control data block	*/

   if (	(pbccd->cItems = cItems) != 0UL	)
       memcpy(PULONGVALUES(pbccd), pulValues, (UINT)(cItems * sizeof(ULONG)));

   if (	n )
       memcpy(PBYTELIST(pbccd),	pchScale, (UINT)n);
   }
		       /* Release the scratch memory used for the	*/
		       /* values and scale text				*/
free(pchScale);
free(pulValues);
}
#pragma	subtitle("   Bar Chart Control DLL - Control Styles Dialogue Procedure")
#pragma	page ( )

/* --- BarChartStyles ---------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used for the custom control's styles dialogue   */
/*     box procedure.							*/
/*									*/
/*     When the	dialogue is invoked from Resource Editor, the		*/
/*     address of the user style information is	contained in message	*/
/*     parameter 2.  The dialogue is responsible for saving the		*/
/*     address.	 The best method to do this is to save the pointer	*/
/*     in the dialogue's reserved memory where it can be retrieved as   */
/*     needed.								*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Dialog Window Handle				*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     BarChartStyles =	Message	Handling Result				*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY BarChartStyles(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)

{
PUSERSTYLE   pust;		   /* User Style Pointer		*/
SWP	     swp;		   /* Screen Window Position Holder	*/

switch ( msg )
   {
		       /* Perform dialogue initialization		*/
   case	WM_INITDLG :
		       /* Save the pointer to user style information	*/
		       /* within the dialog's reserved memory           */

       WinSetWindowPtr(hWnd, QWL_USER, (PVOID)mp2);

		       /* Get the pointer to the user style information	*/

       if ( (pust = (PUSERSTYLE)mp2) !=	NULL )
	   {
		       /* Set the text,	ID symbol and value for	the	*/
		       /* control					*/

	   WinSetDlgItemText(hWnd, EF_TEXT, pust->pszText);
	   pust->pfnSetSymbolID(hWnd, IDBX_SYMBOLVALUE,	pust);

	   if (	pust->flStyle &	WS_VISIBLE )
	       WinSendDlgItemMsg(hWnd, CB_VISIBLE, BM_SETCHECK,	MPFROMSHORT(TRUE), 0L);

	   if (	pust->flStyle &	WS_GROUP )
	       WinSendDlgItemMsg(hWnd, CB_GROUP, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

	   if (	pust->flStyle &	WS_DISABLED )
	       WinSendDlgItemMsg(hWnd, CB_DISABLED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

	   if (	pust->flStyle &	WS_TABSTOP )
	       WinSendDlgItemMsg(hWnd, CB_TABSTOP, BM_SETCHECK,	MPFROMSHORT(TRUE), 0L);

	   if (	pust->pbCtlData	)
	       InitBarChartData(hWnd, (PBARCHARTCDATA)pust->pbCtlData);
	   }
		       /* Centre dialog	on the screen			*/

       WinQueryWindowPos(hWnd, (PSWP)&swp);
       WinSetWindowPos(hWnd, HWND_TOP, (WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN) - swp.cx) /	2L,
		       (WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - swp.cy) /	2L, 0L,	0L, SWP_MOVE);
       break;
		       /* Process push button selections		*/
   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
		       /* Presentation push button selected		*/

	   case	DID_FONTCLR :

		       /* Get the pointer to the user style information	*/

	       if ( (pust = PDATAFROMDLG(hWnd))	!= NULL	)

		       /* Get the address of the look up function from	*/
		       /* user style information structure and display	*/
		       /* the dialog.  The value selected within the	*/
		       /* dialog will be automatically placed within	*/
		       /* the required entry fields			*/

		   pust->pfnGetFontClr(hWnd);
	       break;
		       /* Enter	pushbutton selected get	the definitions	*/
		       /* for the control				*/
	   case	DID_OK :

		       /* Get the pointer to the user style information	*/

	       if ( (pust = PDATAFROMDLG(hWnd))	!= NULL	)
		   {
		       /* Get the address of the symbol	validation	*/
		       /* function from	the user style information	*/
		       /* structure.  The function will	validate the	*/
		       /* symbol and will check	for duplications of	*/
		       /* values.  A return value of TRUE from the	*/
		       /* validation function indicates	that the symbol	*/
		       /* and value are	acceptable.  Conversely, a	*/
		       /* FALSE	return value indicates that symbol or	*/
		       /* value	was not	acceptable.  In	this case,	*/
		       /* the dialog should not	be exited from and the	*/
		       /* values within	the entry fields should	not be	*/
		       /* saved.					*/

		   if (	!pust->pfnGetSymbolID(hWnd, IDBX_SYMBOLVALUE, pust) )
		       break;
		   else
		       {
		       /* Symbol and value validated, get the text of	*/
		       /* the control and save within the user style	*/
		       /* information structure	for use	by Resource	*/
		       /* Editor					*/

		       pust->cText = WinQueryDlgItemText(hWnd, EF_TEXT,	CCHTEXTMAX, pust->pszText);

		       /* Mask out current styles clearing selectable	*/
		       /* styles and save new style			*/

		       pust->flStyle = 0UL;

		       if ( WinSendDlgItemMsg(hWnd, CB_VISIBLE,	BM_QUERYCHECK, 0L, 0L) )
			   pust->flStyle |= WS_VISIBLE;

		       if ( WinSendDlgItemMsg(hWnd, CB_GROUP, BM_QUERYCHECK, 0L, 0L) )
			   pust->flStyle |= WS_GROUP;

		       if ( WinSendDlgItemMsg(hWnd, CB_DISABLED, BM_QUERYCHECK,	0L, 0L)	)
			   pust->flStyle |= WS_DISABLED;

		       if ( WinSendDlgItemMsg(hWnd, CB_TABSTOP,	BM_QUERYCHECK, 0L, 0L) )
			   pust->flStyle |= WS_TABSTOP;

		       GetBarChartData(hWnd, (PBARCHARTCDATA)pust->pbCtlData, pust);
		       }
		   }
		       /* Exit the dialogue indicating changes made	*/

	       WinDismissDlg(hWnd, TRUE);
	       break;
		       /* Cancel selected, exit	the dialogue without	*/
		       /* changing anything				*/

	   case	DID_CANCEL :
	       WinDismissDlg(hWnd, FALSE);
	       break;
	   }
       break;
		       /* Close	received, exit dialog			*/
   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;
		       /* Pass through unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);

}
#pragma	subtitle("   Bar Chart Control DLL - Control Window Procedure")
#pragma	page( )

/* --- lGetPresParam ----------------------------------	[ Private } ---	*/
/*									*/
/*     This function is	used to	retrieve a presentation	parameter	*/
/*     that may	be present.  If	the presentation parameter is not,	*/
/*     the default colour passed to the	function will be used.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND  hWnd;     = Window	Handle					*/
/*     ULONG ulID1;    = Presentation Parameter	1 ID			*/
/*     ULONG ulID2;    = Presentation Parameter	2 ID			*/
/*     LONG  lDefault; = Default Colour					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     lGetPresParam = Colour to Use					*/
/*									*/
/* --------------------------------------------------------------------	*/

static LONG lGetPresParam(HWND hWnd, ULONG ulID1, ULONG	ulID2, LONG lDefault)

{
HPS   hPS;			   /* Presentation Space Handle		*/
LONG  lClr;			   /* Presentation Parameter Colour	*/
ULONG ulID;			   /* Presentation Parameter ID		*/

if ( WinQueryPresParam(hWnd, ulID1, ulID2, &ulID, 4UL, (PVOID)&lClr, QPF_NOINHERIT | QPF_ID2COLORINDEX | QPF_PURERGBCOLOR) )
   return(lClr);
else
   if (	(lDefault >= SYSCLR_SHADOWHILITEBGND) && (lDefault <= SYSCLR_HELPHILITE) )
       return(WinQuerySysColor(HWND_DESKTOP, lDefault, 0L));
   else
       if ( (lClr = GpiQueryRGBColor(hPS = WinGetPS(hWnd), LCOLOPT_REALIZED, lDefault))	== GPI_ALTERROR	)
	   {
	   WinReleasePS(hPS);
	   return(lDefault);
	   }
       else
	   {
	   WinReleasePS(hPS);
	   return(lClr);
	   }
}
#pragma	subtitle("   Bar Chart Control DLL - Control Window Sizing Procedure")
#pragma	page ( )

/* --- ulNiceScale ------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	determine the maximum value for	the	*/
/*     scale.								*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     ULONG ulMax; = Maximum Value					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     ulNiceScale = Scale Limit					*/
/*									*/
/* --------------------------------------------------------------------	*/

static ULONG ulNiceScale(ULONG ulMax)

{
double dMax = (double)ulMax;	   /* Maximum Value			*/
double dRange;			   /* Range				*/
double dMagnitude = 0.0;	   /* Magnitude				*/

if ( (dRange = fabs(dRange)) >=	1.0 )
   while ( dRange >= 10.0)
       {
       dRange /= 10.0;
       dMagnitude++;
       }
else
   while ( dRange <= 1.0 )
       {
       dRange *= 10.0;
       dMagnitude--;
       }
dRange = pow(10.0, dMagnitude);

return((ULONG)((ceil(dMax / dRange)) * dRange));

}
#pragma	subtitle("   Bar Chart Control DLL - Control Window Sizing Procedure")
#pragma	page ( )

/* --- CalcSize	---------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	calculate the sizes and	positions	*/
/*     of the various elements that are	used to	make up	the bar		*/
/*     chart.  The idea	of the function	is to only calculate display	*/
/*     points within the control when the control has been sized.  By	*/
/*     doing this after	a sizing instead of when the control is		*/
/*     painted,	time is	saved during the painting process.  This	*/
/*     allows the painting process to be as fast as possible.  In	*/
/*     effect, the control "caches" the	display	points.	
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND	 hWnd; = Window	Handle					*/
/*     PRECTL	 prcl; = Control Rectangle Pointer			*/
/*     PBARCHART pbc;  = Bar Chart Data	Pointer				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID CalcSize(HWND hWnd,	PRECTL prcl, PBARCHART pbc)

{
CHAR	    szBuffer[32];	   /* String Buffer			*/
FONTMETRICS fm;			   /* Font Metrics			*/
HPS	    hPS;		   /* Presentation Space Handle		*/
LONG	    cxAvgChar;		   /* Average Character	Width		*/
LONG	    cxBar;		   /* Bar Width				*/
LONG	    cyBar;		   /* Bar Height			*/
LONG	    cyFont;		   /* Font Height			*/
LONG	    cyScaleText;	   /* Scale Text Height			*/
LONG	    lScale;		   /* Scale Factor			*/
LONG	    xStart;		   /* Bar Starting Point		*/
POINTL	    rgptl[TXTBOX_COUNT];   /* Text Box Point Array		*/
ULONG	    ul;			   /* Chart High Water Mark		*/
register INT i;			   /* Loop Counter			*/

pbc->fDraw = TRUE;

		       /* Get the font metrics for the control		*/

GpiQueryFontMetrics(hPS	= WinGetPS(hWnd), sizeof(FONTMETRICS), &fm);
WinReleasePS(hPS);
		       /* Save the average character width of the	*/
		       /* default font					*/

cxAvgChar = fm.lAveCharWidth;

		       /* Calculate the	top edge of the	bar chart	*/
		       /* making sure that the bar char	title text is	*/
		       /* taken	into consideration			*/

pbc->aptl[2].x = pbc->aptl[3].x	= prcl->xRight - 1L;
if ( pbc->cTitleText )
   {
   pbc->rclTitleText = *prcl;
   pbc->rclTitleText.yBottom = (pbc->aptl[1].y = pbc->aptl[2].y	= prcl->yTop - (cyFont = fm.lMaxBaselineExt * 2L) - 1L)	+ 1L;
   }
else
   pbc->aptl[1].y = pbc->aptl[2].y = prcl->yTop	- 1L;

		       /* Calculate the	bottom edge of the bar chart	*/
		       /* making sure that the horizontal scale	text is	*/
		       /* taken	into consideration			*/
if ( pbc->cHorzText )
   {
   pbc->rclHorzText = *prcl;
   pbc->rclHorzText.yTop = (pbc->aptl[0].y = pbc->aptl[3].y = prcl->yBottom + fm.lMaxBaselineExt * 3L) - ((fm.lMaxBaselineExt *	3L) / 2L) - 1L;
   }
else
   pbc->aptl[0].y = pbc->aptl[3].y = prcl->yBottom;

		       /* Calculate the	left edge of the bar chart	*/
		       /* making sure that the vertical	scale text is	*/
		       /* taken	into consideration			*/
if ( pbc->cVertText )
   {
		       /* Since	the vertical text is vertically		*/
		       /* oriented, set	the font for the text and get	*/
		       /* the font metrics to allow the	proper		*/
		       /* calculation of the left edge of the bar chart	*/

   GpiSetCharBox(hPS = WinGetPS(hWnd), &pbc->sizfx);
   GpiCreateLogFont(hPS, (PSTR8)NULL, 4L, &pbc->fat);
   GpiSetCharSet(hPS, 4L);

   GpiSetCharAngle(hPS,	&gradl);
   GpiQueryFontMetrics(hPS, sizeof(FONTMETRICS), &fm);
   WinReleasePS(hPS);

   pbc->rclVertText = *prcl;
   pbc->rclVertText.xRight = (pbc->aptl[0].x = pbc->aptl[1].x =	prcl->xLeft + (fm.lMaxBaselineExt * 2L)) - 1L;
   }
else
   pbc->aptl[0].x = pbc->aptl[1].x = prcl->xLeft;

		       /* Determine the	high water mark	of the values	*/

for ( i	= 0, ul	= 0UL; i < pbc->cItems;	i++ )
   if (	pbc->pbci[i].ulValue > ul )
       ul = pbc->pbci[i].ulValue;

ul = ulNiceScale(ul);
		       /* Calculate the	number of vertical scale items	*/

pbc->cScales = (lScale = (cyBar	= (pbc->aptl[1].y - 1) - (pbc->aptl[0].y + 1)) / cyFont);

for ( i	= pbc->cScales;	i > 0; i-- )
   if (	(ul % i) == 0 )
       {
       lScale =	ul / (pbc->cScales = i);
       break;
       }
		       /* Form the scale values	and placement		*/
		       /* positions					*/

if ( pbc->cScales != 0 )
   {
		       /* Form the scale values				*/

   pbc->psi = (PSCALEITEM)malloc(++pbc->cScales	* sizeof(SCALEITEM));
   for ( i = 0;	i < pbc->cScales; i++ )
       strcpy(pbc->psi[i].pszLabel = (PSZ)malloc((pbc->psi[i].cLabel = strlen(_ltoa(i *	lScale,	szBuffer, 10)))	+ 1), szBuffer);

		       /* Get the width	of the largest scale item	*/

   GpiQueryTextBox(hPS = WinGetPS(hWnd), pbc->psi[i - 1].cLabel, pbc->psi[i - 1].pszLabel, 5L, rgptl);
   WinReleasePS(hPS);
		       /* Calculate the	scale value positions		*/

   cyScaleText = rgptl[TXTBOX_TOPLEFT].y - rgptl[TXTBOX_BOTTOMLEFT].y;
   for ( i = 0;	i < pbc->cScales; i++ )
       {
       pbc->psi[i].rcl.xRight  = (pbc->psi[i].rcl.xLeft	  = pbc->aptl[0].x) + rgptl[TXTBOX_CONCAT].x + cxAvgChar - 1L;
       pbc->psi[i].rcl.yTop    = (pbc->psi[i].rcl.yBottom = ((cyBar * i) / (pbc->cScales - 1)) + pbc->aptl[0].y	- (cyScaleText / 2L)) +	cyScaleText;
       }
   pbc->aptl[0].x += rgptl[TXTBOX_CONCAT].x + cxAvgChar;
   pbc->aptl[1].x += rgptl[TXTBOX_CONCAT].x + cxAvgChar;
   }
		       /* Form the 3D shading points for the chart	*/
		       /* area						*/

pbc->aptl[4] = pbc->aptl[0];
pbc->aptl[5].x = pbc->aptl[6].x	= pbc->aptl[0].x + 1L;
pbc->aptl[5].y = pbc->aptl[8].y	= pbc->aptl[0].y + 1L;
pbc->aptl[7].x = pbc->aptl[8].x	= pbc->aptl[2].x - 1L;
pbc->aptl[6].y = pbc->aptl[7].y	= pbc->aptl[1].y - 1L;

		       /* Calculate the	rectangle for the bar chart	*/

pbc->rcl.xLeft	 = pbc->aptl[0].x + 1L;
pbc->rcl.xRight	 = pbc->aptl[2].x - 1L;
pbc->rcl.yBottom = pbc->aptl[0].y + 1L;
pbc->rcl.yTop	 = pbc->aptl[1].y - 1L;

if ( pbc->cItems )
   {
		       /* Calculate the	bar widths assuming the	gaps	*/
		       /* between the bars is the width	of a bar	*/

   xStart = (cxBar = (pbc->rcl.xRight -	pbc->rcl.xLeft)	/ (pbc->cItems * 2)) / 2L + pbc->aptl[0].x + 1L;

		       /* For each bar chart item, determine the bar	*/
		       /* drawing points				*/

   for ( i = 0;	i < pbc->cItems; i++ )
       {
       pbc->pbci[i].aptl[2].x =	pbc->pbci[i].aptl[3].x = (pbc->pbci[i].aptl[0].x = pbc->pbci[i].aptl[1].x = xStart) + cxBar;
       xStart += (cxBar	* 2L);
       pbc->pbci[i].aptl[0].y =	pbc->pbci[i].aptl[3].y = pbc->aptl[0].y	+ 1L;
       pbc->pbci[i].aptl[1].y =	pbc->pbci[i].aptl[2].y = pbc->aptl[0].y	+ (pbc->pbci[i].ulValue	* cyBar) / ul;
       pbc->pbci[i].rcl.xLeft	= pbc->pbci[i].aptl[0].x + 1L;
       pbc->pbci[i].rcl.xRight	= pbc->pbci[i].aptl[2].x;
       pbc->pbci[i].rcl.yBottom	= pbc->pbci[i].aptl[0].y + 1L;
       pbc->pbci[i].rcl.yTop	= pbc->pbci[i].aptl[2].y;

       pbc->pbci[i].rclLabel.xLeft   = pbc->pbci[i].aptl[0].x -	(cxBar / 2L);
       pbc->pbci[i].rclLabel.xRight  = pbc->pbci[i].aptl[3].x +	(cxBar / 2L);
       pbc->pbci[i].rclLabel.yBottom = pbc->pbci[i].aptl[0].y -	cyFont;
       pbc->pbci[i].rclLabel.yTop    = pbc->pbci[i].aptl[0].y -	1L;
       }
   }
}
#pragma	subtitle("   Bar Chart Control DLL - CTLDATA Decoding Procedure")
#pragma	page ( )

/* --- DecodeCTLDATA ----------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	calculate the sizes and	positions	*/
/*     of the various elements that are	used to	make up	a 3D text	*/
/*     field.								*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND	 hWnd; = Window	Handle					*/
/*     PRECTL	 prcl; = Control Rectangle Pointer			*/
/*     PBARCHART pbc;  = Bar Chart Data	Pointer				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID DecodeCTLDATA(PBARCHART pbc, PBARCHARTCDATA	pbccd)

{
PBYTE  pb;			   /* Scale Text Pointer		*/
PULONG pulValues;		   /* Values Pointer			*/
register INT i,	n;		   /* Loop Counter			*/

		       /* Get the horizontal scale title text if	*/
		       /* present					*/

if ( (pbc->cHorzText = pbccd->cbHorzTitle) != 0UL )
   memcpy(pbc->pszHorzText = (PSZ)malloc(pbc->cHorzText), PSZHORZTITLE(pbccd), pbccd->cbHorzTitle);

		       /* Get the vertical scale title text if present	*/

if ( (pbc->cVertText = pbccd->cbVertTitle) != 0UL )
   memcpy(pbc->pszVertText = (PSZ)malloc(pbc->cVertText), PSZVERTTITLE(pbccd), pbccd->cbVertTitle);

		       /* Get the items	of the bar chart along with the	*/
		       /* item labels					*/

if ( (pbc->cItems = pbccd->cItems) != 0UL )
   {
   pbc->pbci = (PBARCHARTITEM)malloc(pbc->cItems * sizeof(BARCHARTITEM));
   pulValues = PULONGVALUES(pbccd);
   pb =	PBYTELIST(pbccd);
   for ( i = n = 0; i <	pbccd->cItems; i++ )
       {
       pbc->pbci[i].ulValue = pulValues[i];
       pbc->pbci[i].pszLabel = (PSZ)malloc(pbc->pbci[i].cLabel = strlen(&pb[n])	+ 1);
       memcpy(pbc->pbci[i].pszLabel, &pb[n], pbc->pbci[i].cLabel);
       n += pbc->pbci[i].cLabel;
       }
   }
}
#pragma	subtitle("   Bar Chart Control DLL - Image Font Selection Function")
#pragma	page( )

/* --- lSelectFont ------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	determine the lMatch component for	*/
/*     a requested image font.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     CHAR  *pszFacename; = Font Face Name				*/
/*     LONG  lPoints;	   = Points Size Requested			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     lSelectFont = lMatch Number for Requested Font			*/
/*									*/
/* --------------------------------------------------------------------	*/

static LONG lSelectFont(CHAR *pszFacename, LONG	lPoints)

{
HDC	     hDC;		   /* Display Device Context Handle	*/
HPS	     hPS;		   /* Presentation Space Handle		*/
LONG	     cFonts;		   /* Fonts Count			*/
LONG	     lFontsTotal = 0L;	   /* Fonts Total Count			*/
LONG	     lMatch = 1L;	   /* Font Match Value			*/
LONG	     lXDeviceRes;	   /* x	Device Resolution		*/
LONG	     lYDeviceRes;	   /* y	Device Resolution		*/
PFONTMETRICS pfm;		   /* Font Metrics Pointer		*/
register INT i;			   /* Loop Counter			*/

		       /* Get the height of the	screen in pels		*/

DevQueryCaps(hDC = GpiQueryDevice(hPS =	WinGetScreenPS(HWND_DESKTOP)), CAPS_HORIZONTAL_FONT_RES, 1L, &lXDeviceRes);
DevQueryCaps(hDC, CAPS_VERTICAL_FONT_RES,   1L,	&lYDeviceRes);

		       /* Get the number of fonts for the face name	*/
		       /* provided and allocate	space for the font	*/
		       /* metrics for the different font sizes and	*/
		       /* devices of the font				*/

pfm = (PFONTMETRICS)malloc(sizeof(FONTMETRICS) * (cFonts = GpiQueryFonts(hPS, QF_PUBLIC, pszFacename,
									 &lFontsTotal, sizeof(FONTMETRICS), (PFONTMETRICS)NULL)));

		       /* Make a pointer for the memory	allocated for	*/
		       /* the font metrics and get the font metrics for	*/
		       /* the number of	fonts for the face name		*/
		       /* provided					*/

GpiQueryFonts(hPS, QF_PUBLIC, pszFacename, &cFonts, sizeof(FONTMETRICS), pfm);

		       /* Release the presentation space acquired to	*/
		       /* determine the	screen height and to get the	*/
		       /* font metrics					*/
WinReleasePS(hPS);
		       /* Adjust the point size	to correspond to the	*/
		       /* the nominal point size that is contained	*/
		       /* within the font metrics structure		*/
lPoints	*= 10;
		       /* Loop through the font	metrics	returned to	*/
		       /* locate the desired font by matching the x and	*/
		       /* y device resolution of the font and the point	*/
		       /* size						*/

for ( i	= 0; i < (INT)cFonts; i++ )
   if (	(pfm[i].sXDeviceRes == (SHORT)lXDeviceRes) && (pfm[i].sYDeviceRes == (SHORT)lYDeviceRes) &&
	((LONG)pfm[i].sNominalPointSize	== lPoints) )
       {
		       /* Font found, get the match value to allow the	*/
		       /* exact	font to	be selected by the calling	*/
		       /* application					*/

       lMatch =	pfm[i].lMatch;
       break;
       }
		       /* Release the memory allocated for the font	*/
		       /* metrics array					*/
DosFreeMem(pfm);
		       /* Return the match value to the	calling		*/
		       /* application					*/
return(lMatch);
}
#pragma	subtitle("   Bar Chart Control DLL - Control Window Procedure")
#pragma	page ( )

/* --- BarChartWndProc --------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	handle the messages sent to the		*/
/*     installed control.  The window procedure	is designed to		*/
/*     allow for multiple instances and	to be totally re-entrant.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Window Handle					*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     BarChartWndProc = Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY BarChartWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)

{
FONTMETRICS    fm;		   /* Font Metrics			*/
HPS	       hPS;		   /* Presentation Space Handle		*/
PBARCHART      pbc;		   /* Text Field Structure Pointer	*/
PBARCHARTCDATA pbccd;		   /* Bar Chart	CTLDATA	Pointer		*/
PBYTE	       pb;		   /* List Pointer			*/
PCREATESTRUCT  pcrst;		   /* Create Structure Pointer		*/
PULONG	       pulValues;	   /* Values List Pointer		*/
PWNDPARAMS     pwprm;		   /* Window Parameters	Pointer		*/
RECTL	       rcl;		   /* Rectangle	Holder			*/
register INT i,	n;		   /* Loop Counter			*/

switch ( msg )
   {

/************************************************************************/
/************************************************************************/
/*									*/
/* Part	1: Control creation						*/
/*									*/
/************************************************************************/
/************************************************************************/

   /*********************************************************************/
   /*  Creation	sequence						*/
   /*********************************************************************/

   case	WM_CREATE :
		       /* Create a heap	for the	control	using a		*/
		       /* segment					*/

       memset(pbc = (PBARCHART)malloc(sizeof(BARCHART)), 0, sizeof(BARCHART));

		       /* Save the address of the text string pointer	*/
		       /* in the control's reserved memory to allow it  */
		       /* to be	referenced as required by the control	*/

       WinSetWindowPtr(hWnd, QWW_CDATA,	(PVOID)pbc);

		       /* Get the control's creation structure address  */
		       /* to copy the default text of the control to	*/
		       /* the memory in	the heap			*/

       pcrst = (PCREATESTRUCT)PVOIDFROMMP(mp2);

		       /* Save the owner window	handle			*/

       pbc->hwndOwner =	pcrst->hwndOwner;

		       /* Check	to see if any CTLDATA defined for the	*/
		       /* control and if so, decode it			*/
       if ( mp1	)
	   DecodeCTLDATA(pbc, (PBARCHARTCDATA)mp1);

		       /* Since	the vertical scale text	is vertical,	*/
		       /* need to form the size	box for	the text based	*/
		       /* on a defined text size, therefore, get the	*/
		       /* font info for	Helv and base the size of the	*/
		       /* scalable vertical font on it			*/

       pbc->fat.usRecordLength = (USHORT)sizeof(FATTRS);
       pbc->fat.usCodePage     = (USHORT)850;
       pbc->fat.lMatch	       = lSelectFont(memcpy(pbc->fat.szFacename, "Helv", 5UL), 8L);
       GpiCreateLogFont(hPS = WinGetPS(hWnd), (PSTR8)NULL, 4L, &pbc->fat);
       GpiSetCharSet(hPS, 4L);

       GpiQueryFontMetrics(hPS,	sizeof(FONTMETRICS), &fm);

       pbc->sizfx.cx = MAKEFIXED(fm.lEmHeight, 0);
       pbc->sizfx.cy = MAKEFIXED(fm.lEmInc, 0);

       GpiSetCharSet(hPS, 0L);
       GpiDeleteSetId(hPS, 4L);
       WinReleasePS(hPS);

       pbc->fat.usRecordLength	= (USHORT)sizeof(FATTRS);
       pbc->fat.usCodePage	= (USHORT)850;
       pbc->fat.fsFontUse	= (USHORT)(FATTR_FONTUSE_OUTLINE | FATTR_FONTUSE_TRANSFORMABLE);
       memcpy(pbc->fat.szFacename, "Helvetica",	10);

		       /* Get the title	text if	present			*/

       if ( pcrst->pszText )
	   strcpy(pbc->pszTitleText = (PSZ)malloc(pbc->cTitleText = strlen(pcrst->pszText) + 1), pcrst->pszText);

		       /* Initialize the size of the control		*/
       rcl.xLeft   =
       rcl.yBottom = 0L;

		       /* If the control has an	initial	size, determine	*/
		       /* the display locations	of the bar chart	*/

       if ( (rcl.xRight= pcrst->cx) && (rcl.yTop = pcrst->cy) )
	   CalcSize(hWnd, &rcl,	pbc);

		       /* Get the presentation parameters for the bar	*/
		       /* chart	if any are defined otherwise use	*/
		       /* default colours				*/

       pbc->lClrText	   = lGetPresParam(hWnd, PP_FOREGROUNDCOLOR, PP_FOREGROUNDCOLORINDEX, SYSCLR_OUTPUTTEXT);
       pbc->lClrBackground = lGetPresParam(hWnd, PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, SYSCLR_DIALOGBACKGROUND);
       pbc->lClrBorder	   = lGetPresParam(hWnd, PP_BORDERCOLOR,     PP_BORDERCOLORINDEX,     SYSCLR_BUTTONDARK);
       pbc->lClrBar	   = lGetPresParam(hWnd, PP_BARCOLOR,	     PP_BARCOLORINDEX,	      RGB_RED);
       break;

/************************************************************************/
/************************************************************************/
/*									*/
/* Part	2: Sizing and Placement						*/
/*									*/
/************************************************************************/
/************************************************************************/

   /*********************************************************************/
   /*  Control being resized						*/
   /*********************************************************************/

   case	WM_SIZE	:
		       /* Check	to see if the control has a size and if	*/
		       /* the case, determine the display locations of	*/
		       /* the bar chart					*/

       if ( ((LONG)(SHORT)SHORT1FROMMP(mp2) > 0L) && ((LONG)(SHORT)SHORT2FROMMP(mp2) > 0L) )
	   {
	   WinQueryWindowRect(hWnd, &rcl);
	   CalcSize(hWnd, &rcl,	(PBARCHART)WinQueryWindowPtr(hWnd, QWW_CDATA));
	   }
       else
		       /* Since	the control does not have any size,	*/
		       /* it doesn't need to be drawn, therefore, set   */
		       /* the draw flag	to false to prevent		*/
		       /* unncessary drawing attempts			*/

	   pbc->fDraw =	FALSE;
       break;

/************************************************************************/
/************************************************************************/
/*									*/
/* Part	3: Focus/Enable	Management					*/
/*									*/
/************************************************************************/
/************************************************************************/


/************************************************************************/
/************************************************************************/
/*									*/
/* Part	4: Text	Input/Output and Presentation Parameter	Interface	*/
/*									*/
/************************************************************************/
/************************************************************************/

   /*********************************************************************/
   /*  Process window parameters setting				*/
   /*********************************************************************/

   case	WM_SETWINDOWPARAMS :

		       /* Get the address for the windows parameters	*/
		       /* structure					*/

       pwprm = (PWNDPARAMS)PVOIDFROMMP(mp1);

		       /* Check	to see if the text for the control is	*/
		       /* being	set					*/

       if ( pwprm->fsStatus & WPM_TEXT )
	   {
		       /* Get the address of the control info from the	*/
		       /* control's reserved memory                     */

	   pbc = (PBARCHART)WinQueryWindowPtr(hWnd, QWW_CDATA);

		       /* Check	to see if any text is being set		*/

	   if (	pwprm->cchText )
	       {
	       if ( pbc->cTitleText )
		   free(pbc->pszTitleText);
	       memcpy(pbc->pszTitleText	= (PSZ)malloc(pwprm->cchText + 1), pwprm->pszText, pbc->cTitleText = pwprm->cchText + 1);
	       }
	   else
	       if ( pbc->cTitleText )
		   {
		       /* No text is being set,	clear any existing text	*/

		   free(pbc->pszTitleText);
		   pbc->cTitleText = 0UL;
		   }

	   if (	pbc->fDraw )
	       WinInvalidateRect(hWnd, &pbc->rclTitleText, FALSE);
	   return(MRFROMLONG(TRUE));
	   }
		       /* Check	to see if new control data is being	*/
		       /* specified					*/

       if ( (pwprm->fsStatus & WPM_CTLDATA) && pwprm->pCtlData )
	   {
	   DecodeCTLDATA(pbc, (PBARCHARTCDATA)pwprm->pCtlData);
	   if (	pbc->fDraw )
	       WinInvalidateRect(hWnd, (PRECTL)NULL, FALSE);
	   return(MRFROMLONG(TRUE));
	   }
       break;

   /*********************************************************************/
   /*  Process window parameters query					*/
   /*********************************************************************/

   case	WM_QUERYWINDOWPARAMS :

		       /* Get the address for the windows parameters	*/
		       /* structure					*/

       pwprm = (PWNDPARAMS)PVOIDFROMMP(mp1);

		       /* Determine the	type of	query			*/

       if ( pwprm->fsStatus & (WPM_TEXT	| WPM_CCHTEXT) )
	   {
		       /* Get the address of the control info from the	*/
		       /* control's reserved memory                     */

	   pbc = (PBARCHART)WinQueryWindowPtr(hWnd, QWW_CDATA);

	   if (	pbc->cTitleText	)
	       {
	       pwprm->cchText =	pbc->cTitleText	- 1UL;
	       memcpy(pwprm->pszText, pbc->pszTitleText, pbc->cTitleText);
	       }
	   else
	       pwprm->cchText =	0;
	   }
       else
	   if (	pwprm->fsStatus	& WPM_TEXT )
	       {
		       /* Get the address of the control info from the	*/
		       /* control's reserved memory                     */

	       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);

		       /* Copy the text	from the string	to the		*/
		       /* structure					*/

	       memcpy(pwprm->pszText, pbc->pszTitleText, pbc->cTitleText);
	       }
	   else
	       if ( pwprm->fsStatus & WPM_CCHTEXT )
		   {
		       /* Text length being asked for, get the address	*/
		       /* of the text string stored in the heap		*/

		   pbc = (PBARCHART)WinQueryWindowPtr(hWnd, QWW_CDATA);

		       /* Place	the length the string within the	*/
		       /* structure					*/

		   if (	pbc->cTitleText	)
		       pwprm->cchText =	pbc->cTitleText	- 1UL;
		   else
		       pwprm->cchText =	0;
		   }
	       else
		   if (	pwprm->fsStatus	& (WPM_CTLDATA | WPM_CBCTLDATA)	)
		       {
		       for ( i = n = 0;	i < pbc->cItems; i++ )
			   n +=	(INT)pbc->pbci[i].cLabel;
		       pwprm->cbCtlData	= pbc->cHorzText + pbc->cVertText + pbc->cItems	* sizeof(ULONG)	+
					  n + sizeof(BARCHARTCDATA) - 1L;

		       pbccd = (PBARCHARTCDATA)pwprm->pCtlData;
		       pbccd->cb = pwprm->cbCtlData;

		       /* Get the horizontal scale title text if	*/
		       /* present					*/

		       if ( (pbccd->cbHorzTitle	= pbc->cHorzText) != 0UL )
			   memcpy(PSZHORZTITLE(pbccd), pbc->pszHorzText, pbccd->cbHorzTitle);

		       /* Get the vertical scale title text if present	*/

		       if ( (pbccd->cbVertTitle	= pbc->cVertText) != 0UL )
			   memcpy(PSZVERTTITLE(pbccd), pbc->pszVertText, pbccd->cbVertTitle);

		       /* Get the items	of the bar chart along with the	*/
		       /* item labels					*/

		       if ( (pbccd->cItems = pbc->cItems) != 0UL )
			   {
			   pulValues = PULONGVALUES(pbccd);
			   pb =	PBYTELIST(pbccd);
			   for ( i = n = 0; i <	pbccd->cItems; i++ )
			       {
			       pulValues[i] = pbc->pbci[i].ulValue;
			       memcpy(&pb[n], pbc->pbci[i].pszLabel, pbc->pbci[i].cLabel);
			       n += pbc->pbci[i].cLabel;
			       }
			   }
		       }
		   else
		       /* Check	to see if the control data values are	*/
		       /* being	requested				*/

		       if ( pwprm->fsStatus & WPM_CTLDATA )
			   {
			   pbccd = (PBARCHARTCDATA)pwprm->pCtlData;
			   for ( i = n = 0; i <	pbc->cItems; i++ )
			       n += (INT)pbc->pbci[i].cLabel;
			   pbccd->cb = pbc->cHorzText +	pbc->cVertText + pbc->cItems * sizeof(ULONG) + n + sizeof(BARCHARTCDATA) - 1L;

		       /* Get the horizontal scale title text if	*/
		       /* present					*/

			   if (	(pbccd->cbHorzTitle = pbc->cHorzText) != 0UL )	
			       memcpy(PSZHORZTITLE(pbccd), pbc->pszHorzText, pbccd->cbHorzTitle);

		       /* Get the vertical scale title text if present	*/

			   if (	(pbccd->cbVertTitle = pbc->cVertText) != 0UL )
			       memcpy(PSZVERTTITLE(pbccd), pbc->pszVertText, pbccd->cbVertTitle);

		       /* Get the items	of the bar chart along with the	*/
		       /* item labels					*/

			   if (	(pbccd->cItems = pbc->cItems) != 0UL )
			       {
			       pulValues = PULONGVALUES(pbccd);
			       pb = PBYTELIST(pbccd);
			       for ( i = n = 0;	i < pbccd->cItems; i++ )
				   {
				   pulValues[i]	= pbc->pbci[i].ulValue;
				   memcpy(&pb[n], pbc->pbci[i].pszLabel, pbc->pbci[i].cLabel);
				   n +=	pbc->pbci[i].cLabel;
				   }
			       }
			   }
		       else

		       /* Check	to see if the size of the control data	*/
		       /* is being requested				*/

			   if (	pwprm->fsStatus	& WPM_CBCTLDATA	)
			       {	
			       for ( i = n = 0;	i < pbc->cItems; i++ )
				   n +=	(INT)pbc->pbci[i].cLabel;
			       pwprm->cbCtlData	= pbc->cHorzText + pbc->cVertText + pbc->cItems	* sizeof(ULONG)	+
						  n + sizeof(BARCHARTCDATA) - 1L;
			       }
			   else
			       return(WinDefWindowProc(hWnd, msg, mp1, mp2));
       return(MRFROMLONG(TRUE));

   /*********************************************************************/
   /*  Presentation parameters changed,	record the changes		*/
   /*********************************************************************/

   case	WM_PRESPARAMCHANGED :

		       /* Get the address of the control info from the	*/
		       /* control's reserved memory                     */

       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);

		       /* Get the new presentation parameter colour for	*/
		       /* the presentation parameter that has changed.	*/
		       /* Get the colour as a RGB value	so as to be	*/
		       /* able to get an exact value and not an		*/
		       /* approximation	which could happen if the	*/
		       /* presentation parameter was set as a RGB but	*/
		       /* queried as an	index.	When WinQueryPresParam	*/
		       /* returns a 0, it indicates that no		*/
		       /* presentation parameter set and the default	*/
		       /* colours should be used.			*/

       switch (	LONGFROMMP(mp1)	)
	   {
		       /* System colour	change				*/
	   case	0 :
	       pbc->lClrText	   = lGetPresParam(hWnd, PP_FOREGROUNDCOLOR, PP_FOREGROUNDCOLORINDEX, SYSCLR_OUTPUTTEXT);
	       pbc->lClrBackground = lGetPresParam(hWnd, PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, SYSCLR_DIALOGBACKGROUND);
	       pbc->lClrBorder	   = lGetPresParam(hWnd, PP_BORDERCOLOR,     PP_BORDERCOLORINDEX,     SYSCLR_BUTTONDARK);
	       break;

	   case	PP_FOREGROUNDCOLOR :
	   case	PP_FOREGROUNDCOLORINDEX	:
	       pbc->lClrText = lGetPresParam(hWnd, PP_FOREGROUNDCOLOR, PP_FOREGROUNDCOLORINDEX,	SYSCLR_OUTPUTTEXT);
	       break;

	   case	PP_BACKGROUNDCOLOR :
	   case	PP_BACKGROUNDCOLORINDEX	:
	       pbc->lClrBackground = lGetPresParam(hWnd, PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, SYSCLR_DIALOGBACKGROUND);
	       break;

	   case	PP_BORDERCOLOR :
	   case	PP_BORDERCOLORINDEX :
	       pbc->lClrBorder = lGetPresParam(hWnd, PP_BORDERCOLOR, PP_BORDERCOLORINDEX, SYSCLR_BUTTONDARK);
	       break;

	   case	PP_BARCOLOR :
	   case	PP_BARCOLORINDEX :
	       pbc->lClrBar = lGetPresParam(hWnd, PP_BARCOLOR, PP_BARCOLORINDEX, RGB_RED);
	       break;

	   default :
	       return(0L);
	   }
       if ( pbc->fDraw )
	   WinInvalidateRect(hWnd, (PRECTL)NULL, FALSE);
       break;

   /*********************************************************************/
   /*  System colours changed, record the changes			*/
   /*********************************************************************/

   case	WM_SYSCOLORCHANGE :

		       /* Get the address of the control info from the	*/
		       /* control's reserved memory                     */

       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);
       pbc->lClrText	   = lGetPresParam(hWnd, PP_FOREGROUNDCOLOR, PP_FOREGROUNDCOLORINDEX, SYSCLR_OUTPUTTEXT);
       pbc->lClrBackground = lGetPresParam(hWnd, PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, SYSCLR_DIALOGBACKGROUND);
       pbc->lClrBorder	   = lGetPresParam(hWnd, PP_BORDERCOLOR,     PP_BORDERCOLORINDEX,     SYSCLR_BUTTONDARK);
       if ( pbc->fDraw )
	   WinInvalidateRect(hWnd, (PRECTL)NULL, FALSE);
       break;

/************************************************************************/
/************************************************************************/
/*									*/
/* Part	5: Mouse interface						*/
/*									*/
/************************************************************************/
/************************************************************************/

   /*********************************************************************/
   /*  Hit test	request							*/
   /*********************************************************************/

   case	WM_HITTEST :
		       /* Mouse	being passed over the control, imply	*/
		       /* that the control is transparent to the	*/
		       /* system					*/

       return(MRFROMLONG(HT_TRANSPARENT));

/************************************************************************/
/************************************************************************/
/*									*/
/* Part	6: Keyboard interface						*/
/*									*/
/************************************************************************/
/************************************************************************/

/************************************************************************/
/************************************************************************/
/*									*/
/* Part	7: Control Message interface					*/
/*									*/
/************************************************************************/
/************************************************************************/

   case	BCM_SETVERTSCALETITLE :

		       /* Get the address of the bar chart data	from	*/
		       /* the control's reserved memory                 */

       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);

       if (  pbc->cVertText )
	   {
	   free(pbc->pszVertText);
	   pbc->cVertText = 0UL;
	   }
		       /* Get the horizontal scale title text if	*/
		       /* present					*/

       if ( (pbc->cVertText = strlen((PSZ)PVOIDFROMMP(mp1))) !=	0UL )
	   {
	   ++pbc->cVertText;
	   memcpy(pbc->pszVertText = (PSZ)malloc(pbc->cVertText), (PSZ)PVOIDFROMMP(mp1), pbc->cVertText);
	   }
       if ( pbc->fDraw )
	   WinInvalidateRect(hWnd, &pbc->rclVertText, FALSE);
       break;

   case	BCM_SETHORZSCALETITLE :

		       /* Get the address of the bar chart data	from	*/
		       /* the control's reserved memory                 */

       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);

       if (  pbc->cHorzText )
	   {
	   free(pbc->pszHorzText);
	   pbc->cHorzText = 0UL;
	   }
		       /* Get the horizontal scale title text if	*/
		       /* present					*/

       if ( (pbc->cHorzText = strlen((PSZ)PVOIDFROMMP(mp1))) !=	0UL )
	   {
	   ++pbc->cHorzText;
	   memcpy(pbc->pszHorzText = (PSZ)malloc(pbc->cHorzText), (PSZ)PVOIDFROMMP(mp1), pbc->cHorzText);
	   }
       if ( pbc->fDraw )
	   WinInvalidateRect(hWnd, &pbc->rclHorzText, FALSE);
       break;

   case	BCM_SETHORZSCALE :

		       /* Get the address of the bar chart data	from	*/
		       /* the control's reserved memory                 */

       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);

		       /* Get the items	of the bar chart along with the	*/
		       /* item labels					*/

       if ( LONGFROMMP(mp1) == pbc->cItems )
	   if (	pbc->cItems )
	       pbc->pbci = (PBARCHARTITEM)realloc(pbc->pbci, (pbc->cItems = LONGFROMMP(mp1)) * sizeof(BARCHARTITEM));
	   else
	       pbc->pbci = (PBARCHARTITEM)malloc((pbc->cItems =	LONGFROMMP(mp1)) * sizeof(BARCHARTITEM));
       pb = (PBYTE)PVOIDFROMMP(mp2);
       for ( i = n = 0;	i < pbc->cItems; i++ )
	   {
	   if (	pbc->pbci[i].cLabel )
	       pbc->pbci[i].pszLabel = (PSZ)realloc(pbc->pbci[i].pszLabel, pbc->pbci[i].cLabel = strlen(&pb[n])	+ 1);
	   else
	       pbc->pbci[i].pszLabel = (PSZ)malloc(pbc->pbci[i].cLabel = strlen(&pb[n])	+ 1);
	   memcpy(pbc->pbci[i].pszLabel, &pb[n], pbc->pbci[i].cLabel);
	   n +=	pbc->pbci[i].cLabel;
	   }
       if ( pbc->fDraw )
	   WinInvalidateRect(hWnd, (PRECTL)NULL, FALSE);
       break;

   case	BCM_SETVALUES :

		       /* Get the address of the bar chart data	from	*/
		       /* the control's reserved memory                 */

       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);
       if ( LONGFROMMP(mp1) == pbc->cItems )
	   if (	pbc->cItems )
	       pbc->pbci = (PBARCHARTITEM)realloc(pbc->pbci, (pbc->cItems = LONGFROMMP(mp1)) * sizeof(BARCHARTITEM));
	   else
	       pbc->pbci = (PBARCHARTITEM)malloc((pbc->cItems =	LONGFROMMP(mp1)) * sizeof(BARCHARTITEM));
       pulValues= (PULONG)PVOIDFROMMP(mp2);
       for ( i = 0; i <	pbc->cItems; i++ )
	   pbc->pbci[i].ulValue	= pulValues[i];
       if ( pbc->fDraw )
	   WinInvalidateRect(hWnd, (PRECTL)NULL, FALSE);
       break;

/************************************************************************/
/************************************************************************/
/*									*/
/* Part	8: Painting							*/
/*									*/
/************************************************************************/
/************************************************************************/

   /*********************************************************************/
   /*  Erase control background						*/
   /*********************************************************************/

   case	WM_ERASEBACKGROUND :
       return(MRFROMLONG(TRUE));

   /*********************************************************************/
   /*  Control painting							*/
   /*********************************************************************/

   case	WM_PAINT :
		       /* Get the address of the bar chart data	from	*/
		       /* the control's reserved memory                 */

       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);

       if ( pbc->fDraw )
	   {
	   GpiCreateLogColorTable(hPS =	WinBeginPaint(hWnd, (HPS)NULL, (PRECTL)NULL), 0L, LCOLF_RGB, 0L, 0L, (PLONG)NULL);
	   WinFillRect(hPS, &pbc->rcl, CLR_WHITE);

	   GpiSetColor(hPS, RGB_DARKGRAY);
	   GpiMove(hPS,	&pbc->aptl[0]);
	   GpiPolyLine(hPS, 2L,	&pbc->aptl[1]);

	   GpiSetColor(hPS, RGB_WHITE);
	   GpiMove(hPS,	&pbc->aptl[2]);
	   GpiPolyLine(hPS, 2L,	&pbc->aptl[3]);

	   GpiSetColor(hPS, RGB_PALEGRAY);
	   GpiMove(hPS,	&pbc->aptl[7]);
	   GpiPolyLine(hPS, 2L,	&pbc->aptl[8]);

	   GpiSetColor(hPS, RGB_BLACK);
	   GpiMove(hPS,	&pbc->aptl[5]);
	   GpiPolyLine(hPS, 2L,	&pbc->aptl[6]);

	   if (	pbc->cTitleText	)
	       WinDrawText(hPS,	pbc->cTitleText, pbc->pszTitleText, &pbc->rclTitleText,	pbc->lClrText, pbc->lClrBackground,
			   DT_VCENTER |	DT_CENTER | DT_ERASERECT);

	   if (	pbc->cHorzText )
	       WinDrawText(hPS,	pbc->cHorzText,	pbc->pszHorzText, &pbc->rclHorzText, pbc->lClrText, pbc->lClrBackground,
			   DT_VCENTER |	DT_CENTER | DT_ERASERECT);
	   if (	pbc->cVertText )
	       {
	       GpiSetCharBox(hPS, &pbc->sizfx);
	       GpiCreateLogFont(hPS, (PSTR8)NULL, 4L, &pbc->fat);
	       GpiSetCharSet(hPS, 4L);

	       GpiSetCharAngle(hPS, &gradl);
	       WinDrawText(hPS,	pbc->cVertText,	pbc->pszVertText, &pbc->rclVertText, pbc->lClrText, pbc->lClrBackground,
			   DT_VCENTER |	DT_CENTER | DT_ERASERECT);
	       GpiSetCharSet(hPS, 0L);
	       GpiDeleteSetId(hPS, 4L);
	       }
	   if (	pbc->cItems )
	       for ( i = 0; i <	pbc->cItems; i++ )
		   {
		   WinFillRect(hPS, &pbc->pbci[i].rcl, pbc->lClrBar);
		   GpiMove(hPS,	&pbc->pbci[i].aptl[3]);
		   GpiPolyLine(hPS, 4L,	pbc->pbci[i].aptl);
		   WinDrawText(hPS, pbc->pbci[i].cLabel, pbc->pbci[i].pszLabel,	&pbc->pbci[i].rclLabel,	pbc->lClrText, pbc->lClrBackground,
			       DT_VCENTER | DT_CENTER |	DT_ERASERECT);
		   }
	   if (	pbc->cScales )
	       for ( i = 0; i <	pbc->cScales; i++ )
		   WinDrawText(hPS, pbc->psi[i].cLabel,	pbc->psi[i].pszLabel, &pbc->psi[i].rcl,	pbc->lClrText, pbc->lClrBackground,
			       DT_VCENTER | DT_CENTER |	DT_ERASERECT);
	   WinEndPaint(hPS);
	   }
       else
	   WinEndPaint(WinBeginPaint(hWnd, (HPS)NULL, (PRECTL)NULL));
       break;

/************************************************************************/
/************************************************************************/
/*									*/
/* Part	9: Control being destroyed					*/
/*									*/
/************************************************************************/
/************************************************************************/

   /*********************************************************************/
   /*  Control being destroy, perform necessary	cleanup			*/
   /*********************************************************************/

   case	WM_DESTROY :
		       /* Get the address of the bar chart data	from	*/
		       /* the control's reserved memory                 */

       pbc = (PBARCHART)WinQueryWindowPtr(hWnd,	QWW_CDATA);

       if ( pbc->cTitleText )
	   free(pbc->pszTitleText);
       if ( pbc->cHorzText )
	   free(pbc->pszHorzText);

       if ( pbc->cVertText )
	   free(pbc->pszVertText);
       if ( pbc->cScales )
	   {
	   for ( i = 0;	i < pbc->cScales; i++ )
	       free(pbc->psi[i].pszLabel);
	   free(pbc->psi);
	   }
       if ( pbc->cItems	)
	   free(pbc->pbci);
       free(pbc);
       break;
		       /* Default message processing			*/
   default :
       return(WinDefWindowProc(hWnd, msg, mp1, mp2));
   }

return(0L);
}
