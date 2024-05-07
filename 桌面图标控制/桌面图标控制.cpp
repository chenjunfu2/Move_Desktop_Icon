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
			if (++dwCurrent < dwFindInfoSize)//如果第一组尝试失败则切换下一组
			{
				continue;
			}
			else//如果所有组都尝试失败则返回无法找到
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
		//else {continue;}
	}

	return NULL;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	while (true)
	{
		//查找桌面ListView
		HWND hDesktopListView;
		while ((hDesktopListView = GetDesktopListView()) == NULL)//如果查找失败则一直重试直到成功
		{
			Sleep(100); //(失败原因比如文件资源管理器可能没有启动或正在启动，桌面窗口没有创建等)
		}

		//获取桌面图标数和桌面客户区大小
		int iItemCount;
		int iDesktopX;
		int iDesktopY;
		while (	(iItemCount = ListView_GetItemCount(hDesktopListView)) <= 0 ||
				(iDesktopX = GetSystemMetrics(SM_CXFULLSCREEN)) <= 0 ||
				(iDesktopY = GetSystemMetrics(SM_CYFULLSCREEN)) <= 0)//如果数据错误则一直重试直到成功
		{
			Sleep(100);//资源管理器发生重启，窗口还未创建成功，等待一会重试
		}

		//设置随机数发生器
		std::mt19937 csRandom((unsigned int)time(NULL));//初始化随机数种子
		std::uniform_int_distribution<int> iRandItemI(0, (iItemCount - 1));//选择任意一个桌面图标下标
		std::uniform_int_distribution<int> iRandItemX(0, (iDesktopX - 1));//选择任意一个新的随机图标X位置
		std::uniform_int_distribution<int> iRandItemY(0, (iDesktopY - 1));//选择任意一个新的随机图标Y位置

		//设置桌面对齐方式（取消自动对齐）//LVS_EX_SNAPTOGRID
		DWORD dwExStyle = ListView_GetExtendedListViewStyle(hDesktopListView);
		dwExStyle &= ~LVS_EX_SNAPTOGRID;
		ListView_SetExtendedListViewStyle(hDesktopListView, dwExStyle);
		//设置桌面图标自动排列（取消自动排列）//LVS_AUTOARRANGE
		LONG lStyle = GetWindowLongPtrA(hDesktopListView, GWL_STYLE);
		lStyle &= ~LVS_AUTOARRANGE;
		SetWindowLongPtrA(hDesktopListView, GWL_STYLE, lStyle);

		//循环随机移动
		while (true)
		{
			//设置随机图标到随机位置
			if (ListView_SetItemPosition(hDesktopListView, iRandItemI(csRandom), iRandItemX(csRandom), iRandItemY(csRandom)) == FALSE)
			{
				//移动失败则退出循环重新查找桌面句柄
				break;
			}
		}
	}

	return 0;
}

//移动图标0到坐标100 100的位置：ListView_SetItemPosition(hDesktopListView, 0, 100, 100);
//重绘所有图标：ListView_RedrawItems(hDesktopListView, 0, ListView_GetItemCount(hDesktopListView) - 1);
//更新窗口：UpdateWindow(hDesktopListView);