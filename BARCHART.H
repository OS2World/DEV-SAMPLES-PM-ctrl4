
/* barchart.h	   Created:    1996-02-20  Revised:  1996-02-20		*/

/* PMCX	Control	Example	Constants Header				*/

/* Copyright ¸ 1989-1996  Prominare Inc.  All Rights Reserved.		*/


/* --- Miscellaneous Constants ----------------------------------------	*/

#define	DID_HELP	      3	   /* Help Push	Button ID		*/
#define	DID_FONTCLR	     11	   /* Font & Colour Push Button	ID	*/
#define	DID_BIDI	     12	   /* Bidi Push	Button ID		*/

#define	CB_VISIBLE	    256	   /* Visible Check Box	ID		*/
#define	CB_GROUP	    257	   /* Group Check Box ID		*/
#define	CB_DISABLED	    258	   /* Disabled Check Box ID		*/
#define	CB_TABSTOP	    259	   /* Tab Stop Check Box ID		*/

#define	IDS_VISIBLE	    512	   /* Visible Style String		*/
#define	IDS_GROUP	    513	   /* Group Style String		*/
#define	IDS_DISABLED	    514	   /* Disabled Style String		*/
#define	IDS_TABSTOP	    515	   /* Tab Stop Style String		*/

/* --- Control Dialogue	-----------------------------------------------	*/

#define	DLG_CTRLUSER	   1000	   /* User Defined Dialog		*/
#define	EF_TEXT		   4051	   /* Text Edit	Field			*/
#define	IDBX_SYMBOLVALUE   4053	   /* ID Value Edit Field		*/
#define	EF_ITEMS	   5001	   /* Items Count Entry	Field		*/
#define	MLE_VALUES	   5002	   /* Values MLE			*/
#define	EF_VERTTITLE	   5003	   /* Vertical Title			*/
#define	EF_HORZTITLE	   5004	   /* Horizontal Title			*/
#define	MLE_HORZSCALE	   5005	   /* Horizontal Title			*/

/* --- Bar Chart CTLDATA ----------------------------------------------	*/

typedef	struct _BARCHARTCDATA	   /* bccd */
   {
   ULONG   cb;			   /* Structure	Size			*/
   ULONG   ulVersion;		   /* Structure	Version	Number		*/
   ULONG   cItems;		   /* Item Count			*/
   ULONG   cbVertTitle;		   /* Vertical Title Text Size		*/
   ULONG   cbHorzTitle;		   /* Horizontal Title Text Size	*/
   CHAR	   szVertTitle[1];	   /* Vertical Title			*/
/* CHAR	   szHorzTitle[1]; */	   /* Horizontal Title			*/
/* ULONG   aulValues[1]; */	   /* Values Array			*/
/* BYTE	   abList[1]; */	   /* List Array			*/
   } BARCHARTCDATA, *PBARCHARTCDATA;

#define	PSZVERTTITLE(pbccd)	(PSZ)((*(PBARCHARTCDATA)(pbccd)).szVertTitle)
#define	PSZHORZTITLE(pbccd)	(PSZ)((*(PBARCHARTCDATA)(pbccd)).szVertTitle + (*(PBARCHARTCDATA)(pbccd)).cbVertTitle)
#define	PULONGVALUES(pbccd)	(PULONG)((*(PBARCHARTCDATA)(pbccd)).szVertTitle	+ (*(PBARCHARTCDATA)(pbccd)).cbVertTitle +\
					 (*(PBARCHARTCDATA)(pbccd)).cbHorzTitle)
#define	PBYTELIST(pbccd)	(PBYTE)((*(PBARCHARTCDATA)(pbccd)).szVertTitle + (*(PBARCHARTCDATA)(pbccd)).cbVertTitle	+\
					 (*(PBARCHARTCDATA)(pbccd)).cbHorzTitle	+ (*(PBARCHARTCDATA)(pbccd)).cItems * sizeof(ULONG))

/* --- Window Information Structures ----------------------------------	*/

typedef	struct _BARCHARTITEM	   /* bci */
   {
   ULONG   ulValue;		   /* Value				*/
   ULONG   cLabel;		   /* Label Size			*/
   PSZ	   pszLabel;		   /* Label				*/
   RECTL   rclLabel;		   /* Label Rectangle			*/
   RECTL   rcl;			   /* Fill Rectangle			*/
   POINTL  aptl[4];		   /* Outline Points			*/
   } BARCHARTITEM, *PBARCHARTITEM ;

typedef	struct _SCALEITEM	   /* si */
   {
   ULONG   cLabel;		   /* Label Size			*/
   PSZ	   pszLabel;		   /* Label				*/
   RECTL   rcl;			   /* Text Rectangle			*/
   } SCALEITEM,	*PSCALEITEM ;

typedef	struct _BARCHART	   /* bc */
   {
   HWND		 hwndOwner;	   /* Owner Window Handle		*/
   BOOL		 fDraw;		   /* Draw Flag				*/
   POINTL	 aptl[9];	   /* Chart Outline Points		*/
   RECTL	 rcl;		   /* Chart Area Rectangle		*/
   ULONG	 cItems;	   /* Item Count			*/
   ULONG	 cTitleText;	   /* Title Text Length			*/
   PSZ		 pszTitleText;	   /* Title Text Pointer		*/
   RECTL	 rclTitleText;	   /* Title Text Display Area		*/
   ULONG	 cHorzText;	   /* Horizontal Title Text Length	*/
   PSZ		 pszHorzText;	   /* Horizontal Title Text Pointer	*/
   RECTL	 rclHorzText;	   /* Horizontal Title Text Display Area*/
   ULONG	 cVertText;	   /* Vertical Title Text Length	*/
   PSZ		 pszVertText;	   /* Vertical Title Text Pointer	*/
   RECTL	 rclVertText;	   /* Vertical Title Text Display Area	*/
   LONG		 lClrText;	   /* Text Colour			*/
   LONG		 lClrBackground;   /* Background Colour			*/
   LONG		 lClrBorder;	   /* Border Colour			*/
   LONG		 lClrBar;	   /* Bar Colour			*/
   FATTRS	 fat;		   /* Vertical Font Attributes		*/
   SIZEF	 sizfx;		   /* Font Size				*/
   ULONG	 cScales;	   /* Scale Item Count			*/
   PSCALEITEM	 psi;		   /* Scale Item Array			*/
   PBARCHARTITEM pbci;		   /* Bar Chart	Items Pointer		*/
   } BARCHART, *PBARCHART ;


/* --- Control Messages	-----------------------------------------------	*/

#define	BCM_SETVERTSCALETITLE (WM_USER+512)
/*     mp1 = MPFROMP(pszVertTitle);	   */
/*     mp2 = 0;				   */
#define	BCM_SETHORZSCALETITLE (WM_USER+513)
/*     mp1 = PVOIDFROMMP(pszHorzTitle);	   */
/*     mp2 = 0;				   */
#define	BCM_SETHORZSCALE      (WM_USER+514)
/*     mp1 = MPFROMLONG(cItems);	   */
/*     mp2 = MPFROMP(apszScaleLabels);	   */
#define	BCM_SETVALUES	      (WM_USER+515)
/*     mp1 = MPFROMLONG(cItems);	   */
/*     mp2 = MPFROMP(aulValues);	   */

/* --- Presentation Parameters ----------------------------------------	*/

#define	PP_BARCOLOR	  (PP_USER + 0UL)
#define	PP_BARCOLORINDEX  (PP_USER + 1UL)
