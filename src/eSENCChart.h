/******************************************************************************
 * Project:  OpenCPN
 * Purpose:
 * Author:   David Register
 *******************************************************************************/


#ifndef _OEUSENCCH_H_
#define _OEUSENCCH_H_

#include "wx/wxprec.h"
#include <wx/wfstream.h>
#include <wx/dynarray.h>

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/process.h>
#include "wx/socket.h"


class VE_Element;
class VC_Element;
class connector_segment;
class S57Obj;
class LUPrec;
class ViewPort;
class Extended_Geometry;

#include "o-charts_pi.h"
#include "s52s57.h"
#include "viewport.h"
#include "Osenc.h"
#include <vector>

//WX_DECLARE_HASH_MAP( unsigned int, VE_Element *, wxIntegerHash, wxIntegerEqual, VE_Hash );
//WX_DECLARE_HASH_MAP( unsigned int, VC_Element *, wxIntegerHash, wxIntegerEqual, VC_Hash );

WX_DEFINE_ARRAY_DOUBLE(double, ArrayOfSortedDoubles);


class PI_S57Light;

WX_DECLARE_OBJARRAY(PI_S57Light,      ArrayOfLights);

#ifndef PI
#define PI        3.1415926535897931160E0      /* pi */
#endif

// Declare the Array of S57Obj
WX_DECLARE_OBJARRAY(S57Obj, ArrayOfS57Obj);
WX_DECLARE_OBJARRAY(S57Obj *, ArrayOfS57ObjPtr);

// And also a list
WX_DECLARE_LIST(S57Obj, ListOfS57Obj);


WX_DECLARE_LIST(ObjRazRules, ListOfObjRazRules);


enum
{
    BUILD_SENC_OK,
    BUILD_SENC_NOK_RETRY,
    BUILD_SENC_NOK_PERMANENT
};

// typedef struct  {
//     double x;
//     double y;
// } MyPoint;

typedef enum InitReturn
{
      INIT_OK = 0,
      INIT_FAIL_RETRY,        // Init failed, retry suggested
      INIT_FAIL_REMOVE,       // Init failed, suggest remove from further use
      INIT_FAIL_NOERROR       // Init failed, request no explicit error message
}_InitReturn;

//extern "C" int G_PtInPolygon(MyPoint *, int, float, float) ;

#ifndef BPP
#define BPP 24
#endif

//------------------------------------------------------------------------------
//    Simple stream cipher input stream
//------------------------------------------------------------------------------
class CryptInputStream
{
public:
    CryptInputStream ( wxInputStream *stream );
    CryptInputStream ( wxInputStream &stream );
    virtual ~CryptInputStream();

    wxInputStream &Read(void *buffer, size_t bufsize);
    char GetC();
    bool Eof();
    size_t Ungetch(const char* buffer, size_t size);
    void Rewind();

    void SetCryptBuffer( unsigned char *buffer, size_t cbsize );

    wxInputStream *m_parent_stream;
    bool m_owns;

    unsigned char  *m_cbuf;
    size_t      m_cbuf_size;
    size_t      m_cb_offset;
    unsigned char *m_outbuf;

};

class PI_S57Light
{
public:
    wxArrayString attributeNames;
    wxArrayString attributeValues;
    wxString position;
    bool hasSectors;
};

// ----------------------------------------------------------------------------
// eSENCChart Definition
// ----------------------------------------------------------------------------

#ifndef __ANDROID__
class  eSENCChart : public PlugInChartBaseExtendedPlus2
#else
class  eSENCChart : public PlugInChartBaseExtended
#endif
{

    public:
      //    Public methods

      eSENCChart();
      virtual ~eSENCChart();

//      void ChartBaseBSBCTOR(void);
//      void ChartBaseBSBDTOR(void);
      wxString GetFileSearchMask(void);

      bool ProcessHeader(Osenc &senc);
      int reduceLOD(double LOD_meters, int nPoints, double *source,
                       wxPoint2DDouble **dest, int *maskIn, int **maskOut);

      //    Accessors

      virtual wxBitmap *GetThumbnail(int tnx, int tny, int cs);

      double GetNormalScaleMin(double canvas_scale_factor, bool b_allow_overzoom);
      double GetNormalScaleMax(double canvas_scale_factor, int canvas_width);


      virtual int Init( const wxString& name, int init_flags );

      virtual int latlong_to_pix_vp(double lat, double lon, int &pixx, int &pixy, PlugIn_ViewPort& vp);
      virtual int vp_pix_to_latlong(PlugIn_ViewPort& vp, int pixx, int pixy, double *lat, double *lon);
      virtual void latlong_to_chartpix(double lat, double lon, double &pixx, double &pixy);
      virtual void chartpix_to_latlong(double pixx, double pixy, double *plat, double *plon);

      wxBitmap &RenderRegionView(const PlugIn_ViewPort& VPoint, const wxRegion &Region);
      wxBitmap &RenderRegionViewOnDCNoText(const PlugIn_ViewPort& VPoint, const wxRegion &Region);
      bool RenderRegionViewOnDCTextOnly(wxMemoryDC& dc, const PlugIn_ViewPort& VPoint, const wxRegion &Region);

      bool RenderViewOnDC(wxMemoryDC& dc, const PlugIn_ViewPort& VPoint);

      int RenderRegionViewOnGL( const wxGLContext &glc, const PlugIn_ViewPort& VPoint,
                                const wxRegion &Region, bool b_use_stencil );
      int RenderRegionViewOnGLNoText( const wxGLContext &glc, const PlugIn_ViewPort& VPoint,
                                      const wxRegion &Region, bool b_use_stencil );
      int RenderRegionViewOnGLTextOnly( const wxGLContext &glc, const PlugIn_ViewPort& VPoint,
                                        const wxRegion &Region, bool b_use_stencil );

      void ClearPLIBTextList();

      virtual bool AdjustVP(PlugIn_ViewPort &vp_last, PlugIn_ViewPort &vp_proposed);
      virtual double GetNearestPreferredScalePPM(double target_scale_ppm);

      virtual bool IsRenderDelta(PlugIn_ViewPort &vp_last, PlugIn_ViewPort &vp_proposed);

      void GetValidCanvasRegion(const PlugIn_ViewPort& VPoint, wxRegion  *pValidRegion);

      virtual bool GetChartExtent(ExtentPI *pext);

      void SetColorScheme(int cs, bool bApplyImmediate);

      void ComputeSourceRectangle(const PlugIn_ViewPort &vp, wxRect *pSourceRect);
      wxRect GetSourceRect();

      wxImage *GetImage();

      double GetRasterScaleFactor();

      int GetCOVREntries(){ return  m_nCOVREntries; }
      int GetCOVRTablePoints(int iTable) { return m_pCOVRTablePoints[iTable]; }
      int  GetCOVRTablenPoints(int iTable){ return m_pCOVRTablePoints[iTable]; }
      float *GetCOVRTableHead(int iTable){ return m_pCOVRTable[iTable]; }

      int GetNoCOVREntries(){ return  m_nNoCOVREntries; }
      int GetNoCOVRTablePoints(int iTable) { return m_pNoCOVRTablePoints[iTable]; }
      int  GetNoCOVRTablenPoints(int iTable){ return m_pNoCOVRTablePoints[iTable]; }
      float *GetNoCOVRTableHead(int iTable){ return m_pNoCOVRTable[iTable]; }

      int GetNativeScale(){ return m_Chart_Scale;}

      ListOfPI_S57Obj *GetObjRuleListAtLatLon(float lat, float lon, float select_radius,
                                                                 PlugIn_ViewPort *VPoint);
      wxString CreateObjDescriptions( ListOfPI_S57Obj* obj_list );
      wxString GetObjectAttributeValueAsString( PI_S57Obj *obj, int iatt, wxString curAttrName );
      static wxString GetAttributeDecode( wxString& att, int ival );

      ListOfPI_S57Obj *GetLightsObjRuleListVisibleAtLatLon(
        float lat, float lon, PlugIn_ViewPort *VPoint);

      wxString          m_extended_error;

      struct chart_context     *m_this_chart_context;

//       virtual VE_Hash&  Get_ve_hash(void){ return m_ve_hash; }
//       virtual VC_Hash&  Get_vc_hash(void){ return m_vc_hash; }

      virtual std::unordered_map<unsigned, VE_Element *> &Get_ve_hash(void) {
        return m_ve_hash;
      }
      virtual std::unordered_map<unsigned, VC_Element *> &Get_vc_hash(void) {
        return m_vc_hash;
      }

      virtual void GetPointPix(ObjRazRules *rzRules, float rlat, float rlon, wxPoint *r);
      virtual void GetPointPix(ObjRazRules *rzRules, wxPoint2DDouble *en, wxPoint *r, int nPoints);
      virtual void GetPixPoint(int pixx, int pixy, double *plat, double *plon, ViewPort *vpt);

      float *GetLineVertexBuffer( void ){ return m_line_vertex_buffer; }
      ListOfS57Obj *GetAssociatedObjects( S57Obj *obj );
      double GetCalculatedSafetyContour(void){ return m_next_safe_cnt; }

      bool        m_b2pointLUPS;
      bool        m_b2lineLUPS;

      //  Object arrays used by S52PLIB TOPMAR rendering logic
      wxArrayPtrVoid *pFloatingATONArray;
      wxArrayPtrVoid *pRigidATONArray;

      sm_parms    vp_transform;

      ViewPort    m_cvp;

      double      m_ref_lat;
      double      m_ref_lon;

      int  m_uSENCExpireDaysRemaining;
      int  m_uSENCGraceDaysAllowed;
      int  m_uSENCGraceDaysRemaining;

protected:
//    Methods
      PI_InitReturn     CreateHeaderDataFromeSENC(void);

      bool              InitFrom_ehdr( wxString &efn );
      PI_InitReturn     FindOrCreateSenc( const wxString& name );
      int               BuildSENCFile( const wxString& FullPath_os63, const wxString& SENCFileName );
      int               BuildRAZFromSENCFile( const wxString& FullPath, wxString &Key, int ctype );
//      int               _insertRules(S57Obj *obj);
      int               _insertRules( S57Obj *obj, LUPrec *LUP, eSENCChart *pOwner );

      PI_InitReturn     PostInit( int flags, int cs );
      void              SetVPParms(const PlugIn_ViewPort &vpt);
      void              ResetPointBBoxes(const PlugIn_ViewPort &vp_last, const PlugIn_ViewPort &vp_this);
      void              SetLinePriorities(void);
      unsigned char     *GetSENCCryptKeyBuffer( const wxString& FullPath, size_t* bufsize );

      void              FreeObjectsAndRules();

      void              BuildLineVBO( void );
      void              AssembleLineGeometry( void );
      void              UpdateLUPs( eSENCChart *pOwner );

        // Rendering
      bool DoRenderViewOnDC(wxMemoryDC& dc, const PlugIn_ViewPort& VPoint, bool force_new_view);
      bool DoRenderRegionViewOnDC(wxMemoryDC& dc, const PlugIn_ViewPort& VPoint, const wxRegion &Region, bool b_overlay);
      int DCRenderRect(wxMemoryDC& dcinput, const PlugIn_ViewPort& vp, wxRect *rect);
      bool DCRenderLPB(wxMemoryDC& dcinput, const PlugIn_ViewPort& vp, wxRect* rect);
      bool DCRenderText( wxMemoryDC& dcinput, const PlugIn_ViewPort& vp );

      wxBitmap *GetCloneBitmap();
      bool IsCacheValid(){ return (pDIB != NULL); }
      void InvalidateCache();

//       void SetClipRegionGL( const wxGLContext &glc, const PlugIn_ViewPort& VPoint,
//                             const wxRegion &Region, bool b_render_nodta, bool b_useStencil );
//       void SetClipRegionGL( const wxGLContext &glc, const PlugIn_ViewPort& VPoint, const wxRect &Rect,
//                             bool b_render_nodta, bool b_useStencil );
      bool DoRenderRectOnGL( const wxGLContext &glc, const ViewPort& VPoint, wxRect &rect, bool b_useStencil );
      bool DoRender2RectOnGL( const wxGLContext &glc, const ViewPort& VPoint1, wxRect &rect1,
                              const ViewPort& VPoint2, wxRect &rect2, bool b_useStencil );
      bool DoRenderRectOnGLTextOnly( const wxGLContext &glc, const ViewPort& VPoint, wxRect &rect, bool b_useStencil );

      void UpdateLUPsOnStateChange( void );
      void ClearRenderedTextCache();

      //  Query
      bool DoesLatLonSelectObject( float lat, float lon, float select_radius, S57Obj *obj );
      bool IsPointInObjArea( float lat, float lon, float select_radius, S57Obj *obj );
      int GetLineFeaturePointArray(S57Obj *obj, void **ret_array);


      wxString Get_eHDR_Name( const wxString& name000 );
      wxString Build_eHDR( const wxString& name000 );

      void BuildDepthContourArray( void );
      void SetSafetyContour(void);

      wxString GetAttributeValueAsString(S57attVal *pAttrVal, wxString AttrName);

      Extended_Geometry *buildExtendedGeom( S57Obj *obj );

      int               my_fgets( char *buf, int buf_len_max, CryptInputStream &ifs );

      wxBitmap          *m_pBMPThumb;

      double            m_ppm_avg;

      wxRegion          m_last_Region;
      wxString          m_lastColorScheme;
      wxRect            m_last_vprect;

      wxBitmap          *m_pCloneBM;
      wxMask            *m_pMask;

      PlugIn_ViewPort    m_last_vp;
//      PixelCache        *pDIB;
      wxBitmap          *pDIB;

      //  SM Projection parms, stored as convenience to expedite pixel conversions
      double    m_easting_vp_center, m_northing_vp_center;
      double    m_pixx_vp_center, m_pixy_vp_center;
      double    m_view_scale_ppm;

      long              m_plib_state_hash;
      bool              m_bLinePrioritySet;

      long              m_sync_cmd_pid;
      ExtentPI          m_FullExtent;
      bool              m_bExtentSet;

      wxFileName        m_SENCFileName;
      wxString          m_senc_dir;

      wxString          m_full_base_path;               // From metadata (os63) file
      wxString          m_cell_permit;

      //    Chart region coverage information
      //    Charts may have multiple valid regions within the lat/lon box described by the chart extent
      //    The following table structure contains this embedded information

      //    ENC charts often contain multiple entries

      int         m_nCOVREntries;                       // number of coverage table entries
      int         *m_pCOVRTablePoints;                  // int table of number of points in each coverage table entry
      float       **m_pCOVRTable;                       // table of pointers to list of floats describing valid COVR

      int         m_nNoCOVREntries;                       // number of NoCoverage table entries
      int         *m_pNoCOVRTablePoints;                  // int table of number of points in each NoCoverage table entry
      float       **m_pNoCOVRTable;                       // table of pointers to list of floats describing valid NOCOVR

//       VE_Hash     m_ve_hash;
//       VC_Hash     m_vc_hash;

      std::unordered_map<unsigned, VE_Element *> m_ve_hash;
      std::unordered_map<unsigned, VC_Element *> m_vc_hash;

      float      *m_line_vertex_buffer;
      size_t      m_vbo_byte_length;
      int         m_LineVBO_name;


      ObjRazRules *razRules[PRIO_NUM][LUPNAME_NUM];


      //  DEPCNT VALDCO array members
      int         m_nvaldco;
      int         m_nvaldco_alloc;
      double       *m_pvaldco_array;
      double      m_next_safe_cnt;
      double      m_set_mar_safety_contour;

      wxDateTime  m_date000;                    // extracted from DSID:ISDT
      wxString    m_edtn000;                    // extracted from DSID:EDTN


      wxArrayString     m_up_file_array;
      int               m_latest_update;
      int               m_base_edtn;
      bool              m_bcrypt_buffer_OK;
      unsigned char     *m_crypt_buffer;
      size_t            m_crypt_size;

      double            m_next_safe_contour;
      bool              m_bexpired;

      std::vector<connector_segment *> m_pcs_vector;
      std::vector<VE_Element *> m_pve_vector;

      wxStringHashMap   m_TXTDSC_map;           // maps file names to content
      int               m_sencReadVersion;

};

// ----------------------------------------------------------------------------
// oeuSENCChart Definition
// ----------------------------------------------------------------------------
class  oeuSENCChart : public eSENCChart
{
DECLARE_DYNAMIC_CLASS(oeuSENCChart)

    public:
      //    Public methods

      oeuSENCChart();
      virtual ~oeuSENCChart();

      wxString GetFileSearchMask(void){return _T("*.oesenc");}
      int Init( const wxString& name, int init_flags );

};


// ----------------------------------------------------------------------------
// oeEVCChart Definition
// ----------------------------------------------------------------------------
class  oeuEVCChart : public eSENCChart
{
DECLARE_DYNAMIC_CLASS(oeuEVCChart)

    public:
      //    Public methods

      oeuEVCChart();
      virtual ~oeuEVCChart();

      wxString GetFileSearchMask(void) { return _T("*.oeevc"); }
      int Init( const wxString& name, int init_flags );


};


// ----------------------------------------------------------------------------
// oesuChart Definition
// ----------------------------------------------------------------------------
class  oesuChart : public eSENCChart
{
DECLARE_DYNAMIC_CLASS(oesuChart)

    public:
      //    Public methods

      oesuChart();
      virtual ~oesuChart();

      wxString  GetFileSearchMask(void) { return _T("*.oesu"); }

      int       Init( const wxString& name, int init_flags );
      PI_InitReturn CreateHeaderDataFromeSENC(void);
      PI_InitReturn PostInit( int flags, int cs );
      bool CreateChartInfoFile( wxString chartName, bool forceCreate = false );

    private:

      wxString  m_rKey;

      std::string m_chartInfo,  m_chartInfoEdition, m_chartInfoExpirationDate;
      std::string m_chartInfoShow, m_chartInfoEULAShow, m_chartInfoDisappearingDate;


};


class PI_S57ObjX : public PI_S57Obj
{
public:

    //  Public Methods
    PI_S57ObjX();
    ~PI_S57ObjX();
    PI_S57ObjX(char *first_line, CryptInputStream *scl, int senc_file_version );

    //      wxString GetAttrValueAsString ( char *attr );
    //      int GetAttributeIndex( const char *AttrSeek );
    wxString GetAttrValueAsString( const char *AttrName );

    // Private Methods
private:
    bool IsUsefulAttribute(char *buf);
    //      int my_fgets( char *buf, int buf_len_max, wxInputStream& ifs );
    int my_bufgetlx( char *ib_read, char *ib_end, char *buf, int buf_len_max );

    int GetAttributeIndex( const char *AttrSeek );

};






//----------------------------------------------------------------------------------
//          Classes used to create arrays of geometry elements
//----------------------------------------------------------------------------------

class PI_VE_Element
{
public:
    int         index;
    int         nCount;
    double      *pPoints;
    int         max_priority;
    size_t      vbo_offset;
//    wxBoundingBox BBox;
};

class PI_VC_Element
{
public:
    int         index;
    double      *pPoint;
};


WX_DECLARE_OBJARRAY(PI_VE_Element, PI_ArrayOfVE_Elements);
WX_DECLARE_OBJARRAY(PI_VC_Element, PI_ArrayOfVC_Elements);



// class PI_connector_segment
// {
// public:
//     void *start;
//     void *end;
//     SegmentType type;
//     int vbo_offset;
//     int max_priority;
// };

//----------------------------------------------------------------------------------
//      SENC Server Process container
//----------------------------------------------------------------------------------

class ServerProcess: public wxProcess
{
public:
    ServerProcess();
    ~ServerProcess();

    void OnTerminate(int pid, int status);
    wxString    m_outstring;
    bool        term_happened;

};


//----------------------------------------------------------------------------------
//      SENC Client
//----------------------------------------------------------------------------------
#define SOCKET_ID             5001
#define DS_SERVERSOCKET_ID       5002
#define DS_ACTIVESERVERSOCKET_ID 5003

class SENCclient: public wxInputStream
{
public:
    SENCclient();
    ~SENCclient();

    void Attach( const wxString& FullPath );
    int Open(void);
    void Close();
    wxString GetServerOutput();

    int reset(void);
    int NetRead( void *destination, size_t length, size_t *read_actual);
    //    int UnRead( char *destination, int length);
    //    int fgets( char *destination, int max_length);

    // Over ride methods from the base class
    size_t OnSysRead(void *buffer, size_t size);
    bool Eof() const;

    wxString m_senc_file;

    wxSocketClient      *m_sock;
    bool                m_private_eof;

    ServerProcess       *m_sproc;
    long                m_server_pid;
    bool                m_OK;

};

#endif




