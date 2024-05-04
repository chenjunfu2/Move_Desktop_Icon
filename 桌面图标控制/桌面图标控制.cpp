// 桌面图标控制.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include <Commctrl.h>
#include <time.h>
#include <random>

//获取桌面ListView
HWND GetDesktopListView(void)
{
	HWND hWorkerW = NULL, hSHELLDLL_DefView = NULL, hSysListView32 = NULL;
	DWORD dwCurrent = 0;
	struct
	{
		const char *cpClassName;
		const char *cpWindowName;
	}stFindInfo[]=
	{
		{"WorkerW","",},//win10其它
		{"Progman","Program Manager",},//win7、win10专业版
	};
	const DWORD dwFindInfoSize = sizeof(stFindInfo) / sizeof(stFindInfo[0]);

	while (true)
	{
		hWorkerW = FindWindowExA(NULL, hWorkerW, stFindInfo[dwCurrent].cpClassName, stFindInfo[dwCurrent].cpWindowName);
		if (hWorkerW == NULL)
		{
			if (++dwCurrent < dwFindInfoSize)
			{
				continue;
			}
			else
			{
				return NULL;
			}
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
	if (hDesktopListView == NULL)
	{
		MessageBoxA(NULL, "NULL", "GetDesktopListView", MB_OK);
		return -1;
	}

	//设置桌面对齐方式（取消对齐）
	DWORD dwExStyle = ListView_GetExtendedListViewStyle(hDesktopListView);
	dwExStyle &= ~LVS_EX_SNAPTOGRID;
	ListView_SetExtendedListViewStyle(hDesktopListView, dwExStyle);//LVS_AUTOARRANGE

	//获取桌面图标数和桌面客户区大小
	const int iItemCount = ListView_GetItemCount(hDesktopListView);
	const int iDesktopX = GetSystemMetrics(SM_CXFULLSCREEN);
	const int iDesktopY = GetSystemMetrics(SM_CYFULLSCREEN);

	std::mt19937 csRandom((unsigned int)time(NULL));//初始化随机数种子
	std::uniform_int_distribution<int> iRandItem(0, (iItemCount - 1));//选择任意一个桌面图标
	std::uniform_int_distribution<int> iRandItemX(0, (iDesktopX - 1));//选择任意一个新的随机图标X位置
	std::uniform_int_distribution<int> iRandItemY(0, (iDesktopY - 1));//选择任意一个新的随机图标Y位置

	while (true)
	{	
		//随机移动
		ListView_SetItemPosition(hDesktopListView, iRandItem(csRandom), iRandItemX(csRandom), iRandItemY(csRandom));
	}

	return 0;
}

//移动图标0到坐标100 100的位置：ListView_SetItemPosition(hDesktopListView, 0, 100, 100);
//重绘所有图标：ListView_RedrawItems(hDesktopListView, 0, ListView_GetItemCount(hDesktopListView) - 1);
//更新窗口：UpdateWindow(hDesktopListView);