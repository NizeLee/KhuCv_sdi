//  ChildFrame.h: interface of CChildFrame (child frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once



class CChildFrame : public wxMDIChildFrame
{
public:
	CChildFrame(wxMDIParentFrame* parent, wxWindowID id, const wxString& title);

//	void DispalyLastImage();
//	void ClearAllImages();

//	CClientView* m_pClientView;
//	std::vector<CKcImage> m_ImageList;

	DECLARE_EVENT_TABLE();
};

