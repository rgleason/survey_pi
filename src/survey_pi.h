/******************************************************************************
 * $Id: survey_pi.h,v 1.0 2011/02/26 01:54:37 nohal Exp $
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

#ifndef _SURVEYPI_H_
#define _SURVEYPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/fileconf.h>
#include <wx/hashmap.h>
#include <wx/glcanvas.h>
#include	<wx/dc.h>
#include <wx/dcmemory.h>
#include <map>
#include "tinyxml.h"
#include <wx/imaglist.h>
#include <wx/withimages.h>
#include <wx/listctrl.h>
#include "cross.xpm"
#include "circle.xpm"
#include "square.xpm"
#include <sstream>

//#include "SurveyOverlayFactory.h"

#define     PLUGIN_VERSION_MAJOR    1
#define     PLUGIN_VERSION_MINOR    1

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    8

#define METERS  1
#define FEET    2
#define FATHOMS 3

//World Mercator
#define PROJECTION 3395


#include "ocpn_plugin.h"
#include "surveygui_impl.h"
#include "SurveyOverlayFactory.h"

#include "nmea0183/nmea0183.h"
#include <wx/tokenzr.h>
#include <wx/textfile.h>

#include <algorithm>

#include "libspatialite-amalgamation-3.0.1/headers/spatialite/sqlite3.h"
#include "libspatialite-amalgamation-3.0.1/headers/spatialite.h"
#include "libspatialite-amalgamation-3.0.1/headers/spatialite/gaiageo.h"

#include "tcmgr.h"
#include <vector>
#include <locale>


using namespace std;

class GPXTimeAndZDA;
class soundingdata;


class GPXTimeAndZDA{
public:
	wxString m_date, m_time, m_ZDA;

};




//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define SURVEY_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class survey_pi : public opencpn_plugin_18
{
public:
      survey_pi(void *ppimgr);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

//    The required override PlugIn Methods
      int GetToolbarToolCount(void);
      void ShowPreferencesDialog( wxWindow* parent );

      void OnToolbarToolCallback(int id);
	  void SetSettings();

//    Optional plugin overrides
      void SetColorScheme(PI_ColorScheme cs);
      void SetNMEASentence(wxString &sentence);

//    The override PlugIn Methods
      bool RenderOverlay(wxDC &pmdc, PlugIn_ViewPort *vp);
      bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
	 // bool RenderGLSurveyOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
	  bool GetSurveySoundings(int as);

//    Other public methods
      void SetSurveyDialogX    (int x){ m_survey_dialog_x = x;};
      void SetSurveyDialogY    (int x){ m_survey_dialog_y = x;}

      void OnSurveyDialogClose();

      bool              ImportHydromagic(wxString filename);
	  bool              ImportXYZ(wxString filename);
	  bool              ImportCSV(wxString filename);
      void              ExportHydromagic(int survey_id, wxString filename);
	  void              ExportXYZ(int survey_id, wxString filename);
	  void              ExportCSV(int survey_id, wxString filename);
      int               CreateSurvey(wxString name);
      void              DeleteSurvey(int id);
	  void              DeleteSounding(int id);
      void              FillSurveyDropdown();
	  void              FillSurveyGrid();
	  void              FillSurveyInfo();

      int               InsertSounding(double depth, double lat, double lon, double tide = 0.0, time_t timestamp = 0, int projection = PROJECTION);
      wxArrayString     GetSurveyList();
      int               GetSurveyId(wxString survey_name);

	  wxString          GetSurveyName(int survey_id);
	  wxString          GetSurveyMaxDepth(int survey_id);
	  wxString          GetSurveyMinDepth(int survey_id);
	  wxString			GetSurveyNumSoundings(int survey_id);
	  wxString			GetSurveyAreaSoundings(int survey_id);

      int               GetActiveSurveyId(){ return m_activesurvey; }
      void              SetActiveSurveyId(int id){ m_activesurvey = id; }
      void              SetActiveSurvey(wxString name){ m_activesurveyname = name; m_activesurvey = GetSurveyId(name); }
	  vector<soundingdata>  SetTable(int i);

	  wxArrayString     SetSoundings(int i);
	  void				ParseNMEASentence(wxString sentence);
	  void				timeToZDA(wxString myGPXTime);
	  GPXTimeAndZDA     myTZDA;
	  wxString			DDMLatToDecimal(wxString myString);
	  wxString			DDMLonToDecimal(wxString myString);

	  bool				b_gotdepth;
	  bool				b_gotposn;
	  soundingdata		mydata;
	  vector<soundingdata> all_soundingdata;
	  int               numsoundings;
	  bool				m_recording;
	  bool              m_survey_trace;
	  //SurveyDlg        *m_pSurveyDialog;
	  SurveyMergeDlgDef *m_pSurveyMerge;
	  SurveyPropDlgDef *m_pSurveyProp;
	  double           viewscale;
	  bool              dbMergeSurveys(int survey1, int survey2);
	  double			m_latprev;
	  double       	  m_lonprev;

	  vector<soundingdata>mySurveySoundings;

	  //SurveyOverlayFactory *GetSurveyOverlayFactory(){ return m_pSurveyOverlayFactory; }
	  //SurveyOverlayFactory *m_pSurveyOverlayFactory;
	  //SurveyOverlayFactory *GetSurveyOverlayFactory(){ return m_pSurveyOverlayFactory; };
	  //SurveyOverlayFactory *m_pSurveyOverlayFactory;

	  SurveyOverlayFactory *GetSurveyOverlayFactory(){ return m_pSurveyOverlayFactory; }


private:


	  SurveyOverlayFactory *m_pSurveyOverlayFactory;
	  SurveyDlg *m_pSurveyDialog;

	 // wxDC *m_pdc;
	 // wxGraphicsContext *m_gdc;


	  NMEA0183          m_NMEA0183;                 // Used to parse NMEA Sentences
	 // survey_pi         *dlg;
      sqlite3          *m_database;
      //sqlite3_stmt     *m_stmt;
      int               ret;
      char             *err_msg;
      bool              b_dbUsable;

      wxFileConfig     *m_pconfig;
      wxWindow         *m_parent_window;
      bool              LoadConfig(void);
      bool              SaveConfig(void);

      void              StoreSounding(double depth);
      double            m_lat, m_lon, m_cog;

      wxDateTime        m_lastPosReport;

	  int				m_trimcount;
      //bool PointInLLBox(PlugIn_ViewPort *vp, double x, double y);
      void DrawSounding(wxDC &dc, int x, int y, double depth, long sounding_id, long survey_id);



      int               m_survey_dialog_x, m_survey_dialog_y;
      int               m_display_width, m_display_height;
      bool              m_bRenderOverlay;
	  int               m_iOpacity;
	  int               m_iUnits;

	  bool              m_bCalcTide;
	  wxString          m_sCorrection;
	  bool              m_bRenderAllSurveys;
	  bool              m_bConnectSoundings;
	  bool				m_bRenderSoundingText;
	  bool              m_bRenderWithTide;
	  int               m_iSoundingShape;
	  bool              m_bUseSymbol;
	  bool              m_bUseDepthColours;
	  wxString          m_sSoundingColor;
	  wxString          m_sConnectorColor;
	  wxString          m_sFont;
	  wxString          m_sFontColor;
	  double            m_fLOA;
	  double            m_fBeam;
	  double            m_fSounderBow;
	  double            m_fWaterlineOffset;
	  double            m_fGPSBow;
	  double            m_fGPSPort;
	  double            m_fMinDistance;
	  double            m_fAutoNewDistance;

	  int               mLastX, mLastY;
	  long              mLastSdgId, mLastSurveyId;

      int               m_leftclick_tool_id;

      int               m_activesurvey;
      wxString          m_activesurveyname;
      int               m_projection;

      short             mPriPosition, mPriDepth;
     // int               mLastX, mLastY;
     // long              mLastSdgId, mLastSurveyId;

      bool              dbQuery(wxString sql);
      void              dbGetTable(wxString sql, char ***results, int &n_rows, int &n_columns);
      void              dbFreeResults(char **results);
      int               dbGetIntNotNullValue(wxString sql);
      wxString          dbGetStringValue(wxString sql);
      void              ImportHydromagicTrack(TiXmlElement *track);

	  // Tide parts
	  void LoadHarmonics();
	  double GetPortTideInfo(double lat, double lon, wxDateTime inTime);
	  TCMgr           *ptcmgr;
	  wxString         g_SData_Locn;
	  wxString        *pTC_Dir;

	  std::map < double, wxImage > m_labelCache;
	  std::map < wxString, wxImage > m_labelCacheText;
};

#endif
