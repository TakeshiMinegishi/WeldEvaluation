#ifndef __Message
#define __Message	1

#define READ_RESULT_INIT	0		// �����J�n���b�Z�[�W
#define READ_RESULT_READ	1		// �����i�����b�Z�[�W
#define READ_RESULT_STOP	2		// ������~���N�G�X�g���b�Z�[�W
#define READ_RESULT_END		3		// �����I�����b�Z�[�W


#define WM_UPDATEREQUEST_PROPPAGE		(WM_USER + 200)
#define ID_UPDATE_STATUS				(WM_UPDATEREQUEST_PROPPAGE + 1)
#define WM_WBSCAN_EXISTCHECK			(ID_UPDATE_STATUS+1)
#define WM_WBSCAN_REQUES				(WM_WBSCAN_EXISTCHECK+1)
#define WM_SCAN_REQUEST					(WM_WBSCAN_REQUES+1)
#define WM_VIEW_CHANGE_REQUEST			(WM_SCAN_REQUEST+1)
#define WM_CHANGE_REGIST				(WM_VIEW_CHANGE_REQUEST+1)
#define WM_ANALYSE_REQUEST				(WM_CHANGE_REGIST+1)
#define WM_RESIST_REGIST				(WM_ANALYSE_REQUEST+1)
#define WM_IMAGE_OUTPUT_REGIST			(WM_RESIST_REGIST+1)
#define WM_IMAGE_SCALING				(WM_IMAGE_OUTPUT_REGIST+1)
#define WM_IMAGE_MOVEING				(WM_IMAGE_SCALING+1)
#define WM_READ_RESULT_STATUS			(WM_IMAGE_MOVEING+1)
#define WM_SPECTRUM_GRAPH_REQUEST		(WM_READ_RESULT_STATUS+1)
#define WM_AREA_SPECTRUM_GRAPH_REQUEST	(WM_SPECTRUM_GRAPH_REQUEST+1)
#define WM_SPECTRUME_CLOSE_REQUEST		(WM_AREA_SPECTRUM_GRAPH_REQUEST+1)
#define WM_AREA_SPECTRUM_GRAPH_SET		(WM_SPECTRUME_CLOSE_REQUEST+1)

#endif