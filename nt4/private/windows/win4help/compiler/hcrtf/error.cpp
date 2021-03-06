/*****************************************************************************
*																			 *
*  ERROR.C																	 *
*																			 *
*  Copyright (C) Microsoft Corporation 1990-1995							 *
*  All Rights reserved. 													 *
*																			 *
*****************************************************************************/

#include "stdafx.h"

#include <stdarg.h>
#include "..\hwdll\coutput.h"
#include <search.h>

extern COutput* pLogFile;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************************************

	FUNCTION:	VReportError

	PURPOSE:

	PARAMETERS:
		errnum
		perr
		pszExtra

	RETURNS:	TRUE if errnum != 0

	COMMENTS:
		Overhaul of previous compiler help errors. With system, you have
		notes, warnings, and errors. The user can turn off notes in the .hpj
		file. Errors always terminate. Within each category, there are
		errors which contain topic or line references, and those which
		don't. See hce.h for the ranges.

	MODIFICATION DATES:
		14-Oct-1993 [ralphw]

***************************************************************************/

void _cdecl VReportError(int errnum, PERR perr, ...)
{
	va_list marker;
	int 	iNumber;

	SUPRESS_ERROR* pCurError;
	if (pdrgSupress && pdrgSupress->Count() &&
			(pCurError = (SUPRESS_ERROR*) bsearch(&errnum,
			pdrgSupress->GetBasePtr(), pdrgSupress->Count(),
			sizeof(SUPRESS_ERROR), CompareIntPointers))) {
		pCurError->count++;
		return;
	}

	va_start(marker, perr);

	if (errnum < HCERR_WARNINGS)
		errcount.cNotes++;
	else if (errnum < HCERR_ERRORS)
		errcount.cWarnings++;

	if (!errnum || (options.fSupressNotes && errnum < HCERR_WARNINGS) ||
			iflags.fTrusted)
		return;

	// Print Warning/Error number

	PSTR pszErrType;
	if (errnum < HCERR_WARNINGS)
		pszErrType = (PSTR) GetStringResource(IDS_NOTE);
	else if (errnum < HCERR_ERRORS)
		pszErrType = (PSTR) GetStringResource(IDS_WARNING);
	else
		pszErrType = (PSTR) GetStringResource(IDS_ERROR);

	if (perr->ep == epCnt)		  // no tab
		wsprintf(szParentString, "HC%d: %s", errnum, pszErrType);
	else
		wsprintf(szParentString, "    HC%d: %s: ", errnum, pszErrType);
	SendStringToParent(szParentString);

	if (pLogFile)
		pLogFile->outstring(szParentString);

	// Print line/topic and file name if the error number is within valid range

	PSTR psz;

	if (errnum < HCERR_NOTE_NOTOPIC ||
			(errnum > HCERR_WARNINGS && errnum < HCERR_WARN_NOTOPIC) ||
			(errnum > HCERR_ERRORS && errnum < HCERR_ERROR_NOTOPIC)) {
		switch (perr->ep) {
		case epLine:
			psz = (PSTR) GetStringResource(IDS_LINE);
			iNumber = perr->iLine;
			break;

		case epTopic:
			psz = (PSTR) GetStringResource(IDS_TOPIC);
			iNumber = perr->iTopic;
			break;

		case epOffset:
			ASSERT(FALSE);	  // Not Yet Implemented
			break;

		default:
		case epNoFile:
			iNumber = 0;
			break;
	  }

	  /*
	   * Don't print out "topic...0" if iNumber is 0. This is primarily
	   * for errors in bitmap files.
	   */

	  if (iNumber != 0) {
		wsprintf(szParentString, "%s #%d of ", psz, iNumber);
		SendStringToParent(szParentString);
		if (pLogFile)
		  pLogFile->outstring(szParentString);
	  }
	  if (perr->ep != epNoFile && perr->ep != epCnt) {
		  wsprintf(szParentString, "%s : ", perr->lpszFile);
		  SendStringToParent(szParentString);
		  if (pLogFile)
			pLogFile->outstring(szParentString);
	  }
	}
	SendStringToParent("\r\n\t");
	if (pLogFile)
		pLogFile->outstring("\r\n\t");

	CMem memMsg(4096); // handle huge error messages...

	vsprintf(memMsg, GetStringResource((UINT) errnum), marker);
	strcat(memMsg, txtEol);
	SendStringToParent(memMsg);

	ASSERT(strlen(memMsg) < 4095);

	if (pLogFile != NULL)
		pLogFile->outstring(memMsg);

	if (errnum >= HCERR_ERRORS)
		HardExit();

	return;
}

/***************************************************************************

	FUNCTION:	CompareError

	PURPOSE:	Compares two integer pointers

	PARAMETERS:
		val1
		val2

	RETURNS:

	COMMENTS:

	MODIFICATION DATES:
		31-May-1995 [ralphw]

***************************************************************************/

int __cdecl CompareHashPointers(const void *val1, const void *val2)
{
	if (*(HASH*) val1 < *(HASH*) val2)
		return -1;
	else if (*(HASH*) val1 > *(HASH*) val2)
		return 1;
	else
		return 0;
}

int __cdecl CompareIntPointers(const void *val1, const void *val2)
{
	return *(int*) val1 - *(int*) val2;
}
