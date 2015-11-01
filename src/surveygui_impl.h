/******************************************************************************
 * $Id: surveygui_impl.h,v 1.0 2011/02/26 01:54:37 nohal Exp $
 *
 * Project:  OpenCPN
 * Purpose:  Survey Plugin
 * Author:   Pavel Kalian
 *
 ***************************************************************************
 *   Copyright (C) 2012 by Pavel Kalian   *
 *   $EMAIL$   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#ifndef _SURVEYGUI_IMPL_H_
#define _SURVEYGUI_IMPL_H_

#include "surveygui.h"
#include "survey_pi.h"
#include "ProfileWin.h"

#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/numdlg.h> 

using namespace std;

class survey_pi;
class soundingdata;

class SurveyCfgDlg : public SurveyCfgDlgDef
{
public:
      SurveyCfgDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Survey preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 496,587 ), long style = wxDEFAULT_DIALOG_STYLE );
};

class SurveyDlg : public SurveyDlgDef
{
public:
      SurveyDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Survey"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,550 ), long style = wxDEFAULT_DIALOG_STYLE );
	  void SurveyDlg::LoadSurvey_0();
	  void OnSurveySelection( wxCommandEvent& event );
      void LoadFromFile( wxCommandEvent& event );
	  void OnSurveyRecordToggleNMEA(wxCommandEvent& event);
	  void RecordNMEA(wxCommandEvent& event) ;

	 // void OnRecordToggleNMEA(wxCommandEvent& event);
	 // void StopRecordNMEA(wxCommandEvent& event);
	void OnNewSurvey( wxCommandEvent& event );
	void OnDeleteSurvey( wxCommandEvent& event );
	void OnSurveyProperties( wxCommandEvent& event );
	void OnZoomToSurvey( wxCommandEvent& event );
	void OnMergeSurvey( wxCommandEvent& event );
	void OnImportSurvey( wxCommandEvent& event );
	void OnExportSurvey( wxCommandEvent& event );
	void OnSurveyCancelClick( wxCommandEvent& event );
	void OnSurveyOkClick( wxCommandEvent& event );
	void IsProfileSelected( wxNotebookEvent& event );
	void ProfileShown( wxPaintEvent& event );
    survey_pi *plugin;
	vector<soundingdata> mysoundings;

	//void SetNMEASentence(wxString &sentence);

private:
	wxString          m_ifilename;
	wxString          m_ofilename;
	int               m_interval;
	wxTextFile        m_istream;
	wxFile            m_ostream;


};

#endif