//  KhuCvApp.cpp: implementation of CKhuCvApp
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#include "KhuCvApp.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif

#include <wx/display.h>


bool CKhuCvApp::OnInit()
{
    int language = wxLANGUAGE_KOREAN;
    m_locale.Init(language);

    m_pMainFrame = new CMainFrame(wxT("KhuCV App"));
    
#ifndef  __APPLE__ 
    m_pMainFrame->Maximize(true);
#endif

    m_pMainFrame->Show();

    m_pMainFrame->DragAcceptFiles(true);
    
    wxSize MainFrameSize = m_pMainFrame->GetSize();
    
    
    wxPoint DlgPos;
    wxSize DlgSize(350, 500);
    
//#ifdef __APPLE__
    //DlgPos = {0, 100+480};//{5, wxDisplay().GetGeometry().GetHeight()-DlgSize.y};
//#else
    DlgPos = {5, MainFrameSize.y-200-DlgSize.y};
//#endif

    
    m_pMainDialog = new CMainDialog(m_pMainFrame, wxID_ANY, wxT("Main Dialog"), DlgPos, DlgSize, wxDEFAULT_DIALOG_STYLE);
    m_pMainDialog->Show();

    while (m_PrintVector.size() > 0) {
        for (auto msg : m_PrintVector) {
            int Num = m_pMainDialog->GetPrintListBox()->GetCount();
            m_pMainDialog->GetPrintListBox()->InsertItems(1, &msg, Num);
            m_pMainDialog->GetPrintListBox()->SetSelection(Num);
        }
        m_PrintVector.clear();
    }
   
    return true;
}

CKhuCvApp::~CKhuCvApp() {
    
}

IMPLEMENT_APP(CKhuCvApp)



void DisplayImage(cv::Mat cvImage, int nPosX, int nPosY, bool bErase, bool bClearPos) {
    CMainFrame* pMainFrame = wxGetApp().m_pMainFrame;
    cv::Mat cvCloneImage = cvImage.clone();

    CKcImage kcImage;
    if (cvCloneImage.type() != CV_8UC3) {
        if (cvCloneImage.channels() == 1) {
            cv::Mat cvNewImage(cvCloneImage.rows, cvCloneImage.cols, CV_8UC3);
            cv::normalize(cvCloneImage, cvCloneImage, 0, 255, cv::NORM_MINMAX);
            cvCloneImage.convertTo(cvCloneImage, CV_8UC1);
            cv::cvtColor(cvCloneImage, cvNewImage, cv::COLOR_GRAY2BGR);
            kcImage = CKcImage(cvNewImage, wxPoint(nPosX, nPosY));
        }
        else if(cvCloneImage.channels() == 3) {
            cv::Mat cvNewImage;
            cv::Mat BGR[3];

            cv::split(cvCloneImage, BGR);
            for(int i = 0 ; i < 3 ; ++i) {
                //cv::normalize(BGR[i], BGR[i], 0, 255, cv::NORM_MINMAX);
                BGR[i].convertTo(BGR[i], CV_8UC1);
            }
            cv::merge(BGR, 3, cvNewImage);

            kcImage = CKcImage(cvNewImage, wxPoint(nPosX, nPosY));
        }
        else
            return;
    }
    else
        kcImage = CKcImage(cvCloneImage, wxPoint(nPosX, nPosY));

    if(bClearPos) {
        auto NewEnd = std::remove_if(pMainFrame->m_ImageList.begin(), pMainFrame->m_ImageList.end(), [=](CKcImage kcImage){return kcImage.pos == wxPoint(nPosX, nPosY);});
        pMainFrame->m_ImageList.erase(NewEnd, pMainFrame->m_ImageList.end());
    }

    pMainFrame->m_ImageList.push_back(kcImage);
    pMainFrame->m_pClientView->m_nLastSelImageNum = pMainFrame->m_ImageList.size()-1;
    pMainFrame->m_pClientView->Refresh(bErase);
    pMainFrame->m_pClientView->Update();
}

CKcImage GetLastSelImage() {
    CMainFrame* pMainFrame = wxGetApp().m_pMainFrame;

    int nCurrentGrabImageNum = pMainFrame->m_pClientView->m_nLastSelImageNum;

    if (nCurrentGrabImageNum < 0) {

        if(pMainFrame->m_ImageList.size() > 0)
            return pMainFrame->m_ImageList[pMainFrame->m_ImageList.size()-1];

        return CKcImage();
    }
    else if (nCurrentGrabImageNum >= pMainFrame->m_ImageList.size()) {

        if (pMainFrame->m_ImageList.size() > 0)
            return pMainFrame->m_ImageList[pMainFrame->m_ImageList.size() - 1];

        return CKcImage();
    }

    return pMainFrame->m_ImageList[nCurrentGrabImageNum];
}

CMainDialog* GetMainDialog() {
    return wxGetApp().m_pMainDialog;
}

void DlgPrintf(const char* ptr, ...) {
    unsigned int Num;

    char ach[1024];
    va_list args;
    va_start(args, ptr);
    vsnprintf(ach, 1024, ptr, args);

    wxString msg = ach;
    
    if (wxGetApp().m_pMainDialog) {
        CMainDialog* pMainDialog = wxGetApp().m_pMainDialog;

        while (wxGetApp().m_PrintVector.size() > 0) {
            for (auto msg : wxGetApp().m_PrintVector) {
                Num = pMainDialog->GetPrintListBox()->GetCount();
                pMainDialog->GetPrintListBox()->InsertItems(1, &msg, Num);
                pMainDialog->GetPrintListBox()->SetSelection(Num);
            }
            wxGetApp().m_PrintVector.clear();
        }
        Num = pMainDialog->GetPrintListBox()->GetCount();
        pMainDialog->GetPrintListBox()->InsertItems(1, &msg, Num);
        pMainDialog->GetPrintListBox()->SetSelection(Num);
    }
    else {
        wxGetApp().m_PrintVector.push_back(msg);
    }
}
