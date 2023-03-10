//  KhuCvApp.h: interface of CKhuCvApp
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/clipbrd.h>
#include <wx/listctrl.h>
#include <wx/dir.h>
#include <wx/filehistory.h>
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <vector>

#include "ClientView.h"
#include "MainFrame.h"
#include "MainDialog.h"

class CKhuCvApp : public wxApp
{
public:
    CKhuCvApp() : wxApp() {
        m_pMainFrame = nullptr;
        m_pMainDialog = nullptr;
    }
    CMainFrame* m_pMainFrame;
    CMainDialog* m_pMainDialog;
    wxLocale m_locale;

    std::vector<wxString> m_PrintVector;

    ~CKhuCvApp();
    virtual bool OnInit();
};

void DisplayImage(cv::Mat cvImage, int nPosX, int nPosY, bool bErase, bool bClearPos);
CKcImage GetLastSelImage();
CMainDialog* GetMainDialog();
void DlgPrintf(const char* ptr, ...);
void DrawTextOnImage(cv::Mat& cvImage, const std::string& str, int x, int y, unsigned char R = 255, unsigned char G = 0, unsigned char B = 0, int pointSize = 15);