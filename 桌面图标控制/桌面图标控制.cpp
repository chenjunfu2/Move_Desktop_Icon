// 桌面图标控制.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include <Commctrl.h>

//获取桌面ListView
HWND GetDesktopListView(void)
{
	HWND hWorkerW = NULL, hSHELLDLL_DefView = NULL, hSysListView32 = NULL;
	while (true)
	{
		hWorkerW = FindWindowExA(NULL, hWorkerW, "WorkerW", "");
		if (hWorkerW == NULL)
		{
			continue;
		}

		hSHELLDLL_DefView = FindWindowExA(hWorkerW, NULL, "SHELLDLL_DefView", "");
		if (hSHELLDLL_DefView == NULL)
		{
			continue;
		}

		hSysListView32 = FindWindowExA(hSHELLDLL_DefView, NULL, "SysListView32", "FolderView");
		if (hSysListView32 != NULL)
		{
			return hSysListView32;
		}
		else
		{
			return NULL;
		}
	}

	return NULL;
}




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	HWND hDesktopListView = GetDesktopListView();

	//设置桌面对齐方式（取消对齐）
	DWORD dwExStyle = ListView_GetExtendedListViewStyle(hDesktopListView);
	dwExStyle &= ~LVS_EX_SNAPTOGRID;
	ListView_SetExtendedListViewStyle(hDesktopListView, dwExStyle);//LVS_AUTOARRANGE

	//获取桌面图标数和桌面客户区大小
	const int iItemCount = ListView_GetItemCount(hDesktopListView);
	const int iDesktopX = GetSystemMetrics(SM_CXFULLSCREEN);
	const int iDesktopY = GetSystemMetrics(SM_CYFULLSCREEN);

	while (true)
	{
		//随机一个图标下标不大于等于iItemCount,随机一个x和y坐标分别不大于等于iDesktopX和iDesktopY
		//然后传递给ListView_SetItemPosition进行图标移动，接着再次循环，就能让桌面图标随机移动

		//移动图标0到坐标100 100的位置：ListView_SetItemPosition(hDesktopListView, 0, 100, 100);
		//重绘所有图标：ListView_RedrawItems(hDesktopListView, 0, ListView_GetItemCount(hDesktopListView) - 1);
		//更新窗口：UpdateWindow(hDesktopListView);

		break;
	}


	return 0;
}