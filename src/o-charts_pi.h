/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  o-charts Plugin
 * Author:   David Register
 *
 ***************************************************************************
 *   Copyright (C) 2021 by David S. Register   *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#ifndef _OCHARTSPI_H_
#define _OCHARTSPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "ocpn_types.h"
#include "ocpn_plugin.h"
#include "config.h"
#include "color_types.h"

#include "wx/socket.h"
#include <wx/fileconf.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/html/htmlwin.h>

#include "TexFont.h"

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    11

#include "ocpn_plugin.h"

#include <algorithm>          // for std::sort
#include <string>
#include <map>

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif

#ifdef __ANDROID__
#define WXC_FROM_DIP(x) x
#else
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif
#endif

WX_DECLARE_STRING_HASH_MAP( wxString, OKeyHash );

enum {
    ID_BUTTONCELLIMPORT,
    ID_NOTEBOOK
};

#define ID_ENABLE_REBUILD       23543

//      Private logging functions
//void ScreenLogMessage(wxString s);
//void HideScreenLog(void);
//void ClearScreenLog(void);
//void ClearScreenLogSeq(void);

bool processChartinfo(const wxString &chart_file, wxString status = _T("---"));
extern void showChartinfoDialog( void );

class   o_charts_pi;
class   OCPNPermitList;
class   OCPNCertificateList;
class   o_charts_pi_event_handler;
class   oesencPanel;
class   shopPanel;
class   OESENC_HTMLMessageDialog;


class ChartInfoItem {
public:
    ChartInfoItem(){};
    ~ChartInfoItem(){};

    wxString config_string;
    wxString display_string;
    bool bShown;
    bool bAccessed;
};

class ChartSetEULA{
public:
    ChartSetEULA() {b_sessionShown = false; b_onceShown = false;}
    ~ChartSetEULA() {};

    wxString fileName;
    int npolicyShow;
    bool b_sessionShown;
    bool b_onceShown;
};
WX_DECLARE_OBJARRAY(ChartSetEULA *, EULAArray);


//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

class o_charts_pi : public opencpn_plugin_117
{
public:
      o_charts_pi(void *ppimgr);
      ~o_charts_pi();

//    The required PlugIn Methods
    int Init(void);
    bool DeInit(void);

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    int GetPlugInVersionPatch();
    int GetPlugInVersionPost();
    wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();
    bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
    bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);

    wxArrayString GetDynamicChartClassNameArray();

    void OnSetupOptions();
    void OnCloseToolboxPanel(int page_sel, int ok_apply_cancel);
    void ShowPreferencesDialog( wxWindow* parent );
    void SetColorScheme(ColorScheme cs);

    void SetPluginMessage(wxString &message_id, wxString &message_body);
    void Set_FPR();

    bool SaveConfig( void );

    wxString GetCertificateDir();

    wxStaticText        *m_up_text;
    wxStaticText        *m_ip_text;
    wxStaticText        *m_fpr_text;

    void OnNewFPRClick( wxCommandEvent &event );
    void OnShowFPRClick( wxCommandEvent &event );

    void ProcessChartManageResult(wxString result);
    shopPanel           *m_shoppanel;

    wxScrolledWindow    *m_pOptionsPage;

private:
    bool ScrubChartinfoList( void );
    bool Is_oeSENC_pi_Enabled();
    bool Is_oeRNC_pi_Enabled();

    int ProcessCellPermit( wxString &permit, bool b_confirm_existing );

    bool LoadConfig( void );

    int pi_error( wxString msg );

    wxArrayString     m_class_name_array;

    wxBitmap          *m_pplugin_icon;

    o_charts_pi_event_handler *m_event_handler;

    OCPNPermitList      *m_permit_list;
    wxButton            *m_buttonImportPermit;
    wxButton            *m_buttonRemovePermit;
    wxButton            *m_buttonNewUP;
    wxButton            *m_buttonImportCells;
    wxButton            *m_buttonNewIP;

    wxString            m_userpermit;

    wxString            m_last_enc_root_dir;

    OCPNCertificateList *m_cert_list;
    wxButton            *m_buttonImportCert;

    bool                m_bSSE26_shown;
    TexFont             m_TexFontMessage;

    oesencPanel         *m_oesencpanel;
    wxBitmap            m_panelBitmap;
};



class oesencPrefsDialog : public wxDialog
{
private:

protected:
    wxStdDialogButtonSizer* m_sdbSizer1;
    wxButton* m_sdbSizer1OK;
    wxButton* m_sdbSizer1Cancel;

public:


    oesencPrefsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("o-charts_pi Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE );
    ~oesencPrefsDialog();
    void OnPrefsOkClick(wxCommandEvent& event);

    wxButton *m_buttonNewFPR, *m_buttonNewDFPR;
    wxButton *m_buttonShowFPR;
    wxButton *m_buttonClearSystemName;
    wxButton *m_buttonClearCreds;
    wxButton *m_buttonSendStatus;
    wxStaticText *m_fpr_text;
    wxStaticText *m_nameTextBox;
    wxButton *m_buttonShowEULA;
    wxCheckBox *m_cbEnableRebuild;
    DECLARE_EVENT_TABLE()



};

// An Event handler class to catch events from UI dialog
class o_charts_pi_event_handler : public wxEvtHandler
{
public:

    o_charts_pi_event_handler(o_charts_pi *parent);
    ~o_charts_pi_event_handler();

    void OnNewFPRClick( wxCommandEvent &event );
    void OnNewDFPRClick( wxCommandEvent &event );
    void OnShowFPRClick( wxCommandEvent &event );
    void onTimerEvent(wxTimerEvent &event);
    void OnGetHWIDClick( wxCommandEvent &event );
    void OnManageShopClick( wxCommandEvent &event );
    void OnClearSystemName( wxCommandEvent &event );
    void OnShowEULA( wxCommandEvent &event );
    void OnClearCredentials( wxCommandEvent &event );
    void OnSendStatus( wxCommandEvent &event );

private:
    void processArbResult( wxString result );

    o_charts_pi  *m_parent;

    wxTimer     m_eventTimer;
    int         m_timerAction;

    DECLARE_EVENT_TABLE()

};



class InfoWin: public wxWindow
{
public:
    InfoWin( wxWindow *parent, const wxString&s = _T(""), bool show_gauge = true );
    ~InfoWin();

    void SetString(const wxString &s);
    const wxString& GetString(void) { return m_string; }

    void SetPosition( wxPoint pt ){ m_position = pt; }
    void SetWinSize( wxSize sz ){ m_size = sz; }
    void Realize( void );
    wxSize GetWinSize( void ){ return m_size; }
    void OnPaint( wxPaintEvent& event );
    void OnEraseBackground( wxEraseEvent& event );
    void OnTimer( wxTimerEvent& event );

    wxStaticText *m_pInfoTextCtl;
    wxGauge   *m_pGauge;
    wxTimer     m_timer;

private:

    wxString m_string;
    wxSize m_size;
    wxPoint m_position;
    bool m_bGauge;

    DECLARE_EVENT_TABLE()
};

class InfoWinDialog: public wxDialog
{
public:
    InfoWinDialog( wxWindow *parent, const wxString&s = _T(""), bool show_gauge = true );
    ~InfoWinDialog();

    void SetString(const wxString &s);
    const wxString& GetString(void) { return m_string; }

    void SetPosition( wxPoint pt ){ m_position = pt; }
    void SetWinSize( wxSize sz ){ m_size = sz; }
    void Realize( void );
    wxSize GetWinSize( void ){ return m_size; }
    void OnPaint( wxPaintEvent& event );
    void OnEraseBackground( wxEraseEvent& event );
    void OnTimer( wxTimerEvent& event );

    wxStaticText *m_pInfoTextCtl;
    wxGauge   *m_pGauge;
    wxTimer     m_timer;

private:

    wxString m_string;
    wxSize m_size;
    wxPoint m_position;
    bool m_bGauge;

    DECLARE_EVENT_TABLE()
};


//      Constants

#define ID_DIALOG 10001

#define SYMBOL_ABOUT_TITLE _("o-charts_pi Information")

#define xID_OK          10009
#define xID_CANCEL      10010

#define ID_NOTEBOOK_HELP 10002


class o_charts_pi_about: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( about )
    DECLARE_EVENT_TABLE()

public:
    explicit o_charts_pi_about( );
    explicit o_charts_pi_about( wxWindow* parent,
                    wxWindowID id = ID_DIALOG,
                    const wxString& caption = SYMBOL_ABOUT_TITLE,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxSize(500, 500),
                    long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    explicit o_charts_pi_about( wxWindow* parent,
                              wxString fileName,
                              wxWindowID id = ID_DIALOG,
                              const wxString& caption = SYMBOL_ABOUT_TITLE,
                              const wxPoint& pos = wxDefaultPosition,
                              const wxSize& size = wxSize(500, 500),
                              long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    bool Create( wxWindow* parent,
                 wxWindowID id = ID_DIALOG,
                 const wxString& caption = SYMBOL_ABOUT_TITLE,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxSize(500, 500),
                 long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void RecalculateSize( void );
    void SetOKMode();

private:
    void CreateControls( void );
    void Populate( void );
    void OnXidOkClick( wxCommandEvent& event );
    void OnXidRejectClick( wxCommandEvent& event );
    void OnPageChange(wxNotebookEvent& event);
    void OnClose( wxCloseEvent& event );

    wxWindow *m_parent;
    bool m_btips_loaded;

    wxPanel* itemPanelAbout;
    wxPanel* itemPanelAuthors;
    wxPanel* itemPanelLicense;
    wxPanel* itemPanelTips;

    wxTextCtrl *pAuthorTextCtl;
    wxTextCtrl *pLicenseTextCtl;
    wxNotebook *pNotebook;
    wxHtmlWindow *pAboutHTMLCtl;
    wxHtmlWindow *pLicenseHTMLCtl;
    wxHtmlWindow *pAuthorHTMLCtl;

    wxString m_fileName;
    wxButton* closeButton;
    wxButton* rejectButton;

    //wxSize m_displaySize;

};

///////////////////////////////////////////////////////////////////////////////
/// Class oesencPanel
///////////////////////////////////////////////////////////////////////////////
class oesencPanel : public wxPanel
{
private:

protected:
    wxButton* m_bManageCharts;
    wxButton* m_bVisitOcharts;
    wxButton* m_bCreateHWID;

    // Virtual event handlers, overide them in your derived class
    //virtual void DoHelp( wxCommandEvent& event ) { event.Skip(); }
    virtual void ManageCharts( wxCommandEvent& event );
    virtual void VisitOCharts( wxCommandEvent& event );
    virtual void CreateHWID( wxCommandEvent& event );


public:
    oesencPanel( o_charts_pi* plugin, wxWindow* parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );

    virtual ~oesencPanel();
    oesencPanel() { }
};




/** Implementing oesencPanel */
class oesencPanelImpl : public oesencPanel
{
    friend class o_charts_pi;
private:

protected:
    // Handlers for oesencPanel events.

    void ManageCharts( wxCommandEvent& event );
    void VisitOCharts( wxCommandEvent& event );

#if 0
    void            SetSource( int id );
    void            SelectSource( wxListEvent& event );
    void            AddSource( wxCommandEvent& event );
    void            DeleteSource( wxCommandEvent& event );
    void            EditSource( wxCommandEvent& event );
    void            UpdateChartList( wxCommandEvent& event );
    void            OnDownloadCharts( wxCommandEvent& event );
    void            DownloadCharts( );
    void            DoHelp( wxCommandEvent& event )
    {
        #ifdef __WXMSW__
        wxLaunchDefaultBrowser( _T("file:///") + *GetpSharedDataLocation() + _T("plugins/chartdldr_pi/data/doc/index.html") );
        #else
        wxLaunchDefaultBrowser( _T("file://") + *GetpSharedDataLocation() + _T("plugins/chartdldr_pi/data/doc/index.html") );
        #endif
    }
    void            UpdateAllCharts( wxCommandEvent& event );
    void            OnShowLocalDir( wxCommandEvent& event );
    void            OnPaint( wxPaintEvent& event );
    void            OnLeftDClick( wxMouseEvent& event );

    void            CleanForm();
    void            FillFromFile( wxString url, wxString dir, bool selnew = false, bool selupd = false );

    void            OnContextMenu( wxMouseEvent& event );
    void            SetBulkUpdate( bool bulk_update );
#endif

public:
    oesencPanelImpl() { }
    ~oesencPanelImpl();
    oesencPanelImpl( o_charts_pi* plugin, wxWindow* parent, wxWindowID id = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );

private:
    DECLARE_DYNAMIC_CLASS( oesencPanelImpl )
    DECLARE_EVENT_TABLE()
};


class pi_HTMLMessage: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( about )
    DECLARE_EVENT_TABLE()

public:
    explicit pi_HTMLMessage( );
    explicit pi_HTMLMessage( wxWindow* parent,
                    wxWindowID id = ID_DIALOG,
                    const wxString& caption = SYMBOL_ABOUT_TITLE,
                    const wxString& message = _T("Default message"),
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxSize(500, 500),
                    long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    explicit pi_HTMLMessage( wxWindow* parent,
                              wxString fileName,
                              wxWindowID id = ID_DIALOG,
                              const wxString& caption = SYMBOL_ABOUT_TITLE,
                              const wxString& message = _T("Default message"),
                              const wxPoint& pos = wxDefaultPosition,
                              const wxSize& size = wxSize(500, 500),
                              long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    bool Create( wxWindow* parent,
                 wxWindowID id = ID_DIALOG,
                 const wxString& caption = SYMBOL_ABOUT_TITLE,
                 const wxString& message = _T("Default message"),
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxSize(500, 500),
                 long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void RecalculateSize( void );
    void SetOKMode();
    void SetText( wxString text ){ m_text = text; }

private:
    void CreateControls( void );
    void Populate( void );
    void OnXidOkClick( wxCommandEvent& event );
    void OnXidRejectClick( wxCommandEvent& event );
    void OnPageChange(wxNotebookEvent& event);
    void OnClose( wxCloseEvent& event );

    wxWindow *m_parent;
    bool m_btips_loaded;

    wxPanel* itemPanelAbout;
    wxPanel* itemPanelAuthors;
    wxPanel* itemPanelLicense;
    wxPanel* itemPanelTips;

    wxTextCtrl *pAuthorTextCtl;
    wxTextCtrl *pLicenseTextCtl;
    wxNotebook *pNotebook;
    wxHtmlWindow *pAboutHTMLCtl;
    wxHtmlWindow *pLicenseHTMLCtl;
    wxHtmlWindow *pAuthorHTMLCtl;

    wxString m_fileName;
    wxButton* closeButton;
    wxButton* rejectButton;
    wxString m_text;

    //wxSize m_displaySize;

};

#endif          // Guard


