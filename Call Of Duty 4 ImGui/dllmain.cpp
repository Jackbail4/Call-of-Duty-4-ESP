#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "CodStruct.h"

#include "WorldToScreen.h"
#include "Draw.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx9.h"
#include "Imgui/imgui_impl_win32.h"

#include "detours.h"
#pragma comment(lib, "detours.lib")

namespace Settings {
	namespace Fnd {
		bool DrawLines;
		bool DrawBox;
		bool DrawName;
		bool DrawDistance;
		float Color[3];
		int Thickness = 1;
	}
	namespace Enm {
		bool DrawLines;
		bool DrawBox;
		bool DrawName;
		bool DrawDistance;
		float Color[3];
		int Thickness = 1;
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef LRESULT(_stdcall* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef HRESULT(_stdcall* EndScene)(IDirect3DDevice9* pDevice);

EndScene pEndScene;
WNDPROC oWndProc;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool show =true, init = false;

HRESULT __stdcall hkEndScene(IDirect3DDevice9* pDevice) {
	if (!init) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle* style = &ImGui::GetStyle();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(FindWindow(0, "Call of Duty 4"));
		ImGui_ImplDX9_Init(pDevice);

		init = true;
	}

	ImGui_ImplDX9_Init(pDevice);
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().MouseDrawCursor = show;
	if (show) {
		ImGui::Begin("Call Of Duty 4 ESP", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

		ImGui::Text("Friendly");
		ImGui::BeginChild("Friendly", ImVec2{ 300,150 }, true);
		ImGui::Checkbox("Draw Line", &Settings::Fnd::DrawLines);
		ImGui::Checkbox("Draw 3D Box", &Settings::Fnd::DrawBox);
		ImGui::Checkbox("Draw Name", &Settings::Fnd::DrawName);
		ImGui::Checkbox("Draw Distance", &Settings::Fnd::DrawDistance);
		ImGui::ColorEdit3("ESP Color", Settings::Fnd::Color);
		ImGui::SliderInt("Thickness", &Settings::Fnd::Thickness, 0, 5);
		ImGui::EndChild();

		ImGui::Text("Enemy");
		ImGui::BeginChild("Enemy", ImVec2{ 300,150 }, true);
		ImGui::Checkbox("Draw Line", &Settings::Enm::DrawLines);
		ImGui::Checkbox("Draw 3D Box", &Settings::Enm::DrawBox);
		ImGui::Checkbox("Draw Name", &Settings::Enm::DrawName);
		ImGui::Checkbox("Draw Distance", &Settings::Enm::DrawDistance);
		ImGui::ColorEdit3("ESP Color", Settings::Enm::Color);
		ImGui::SliderInt("Thickness", &Settings::Enm::Thickness, 0, 5);
		ImGui::EndChild();
		ImGui::End();
	}

	int LocalPlayerId = *(int*)0x1280500;
	int LocalTeam = *(int*)(0x831270 + LocalPlayerId * 0x4CC + 0x1C);

	for (int i = 0; i < CgInfo->MaxEntities; i++) {
		Ent* Entity = (Ent*)(0x831270 + i * 0x4CC);
		if (!Entity->Valid) { continue; }
		ImVec2 HeadPos2D;
		w2s::WorldToScreen(Vec3{ Entity->Position.x,Entity->Position.y, Entity->Position.z + 65 }, HeadPos2D);
		ImVec2 Pos2D;
		w2s::WorldToScreen(Vec3{ Entity->Position.x,Entity->Position.y, Entity->Position.z }, Pos2D);
		if (Entity->Team == LocalTeam) {
			if (Settings::Fnd::DrawBox) {
				Draw::Draw3DBox(Entity->Position, ImColor{ Settings::Fnd::Color[0], Settings::Fnd::Color[1] , Settings::Fnd::Color[2] , 255.f }, Settings::Fnd::Thickness);
			}
			if (Settings::Fnd::DrawDistance) {
				float dist = w2s::Get3dDistance(Entity->Position, LocalPlayer->HeadPos);
				std::string distText = "[" + std::to_string((int)dist); distText += "]";
				Draw::DrawName(Pos2D, ImColor{ Settings::Fnd::Color[0], Settings::Fnd::Color[1] , Settings::Fnd::Color[2] , 255.f }, distText.c_str());
			}
			if (Settings::Fnd::DrawName) {
				Draw::DrawName(HeadPos2D, ImColor{ Settings::Fnd::Color[0], Settings::Fnd::Color[1] , Settings::Fnd::Color[2] ,  255.f }, Entity->Name);
			}
			if (Settings::Fnd::DrawLines) {
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ (float)RefDef->ScreenX / 2, 0.f }, HeadPos2D, ImColor{ Settings::Fnd::Color[0], Settings::Fnd::Color[1] , Settings::Fnd::Color[2] ,  255.f }, Settings::Enm::Thickness);
			}
		}
		else {
			if (Settings::Enm::DrawBox) {
				Draw::Draw3DBox(Entity->Position, ImColor{ Settings::Enm::Color[0], Settings::Enm::Color[1] , Settings::Enm::Color[2] ,  255.f }, Settings::Enm::Thickness);
			}
			if (Settings::Enm::DrawDistance) {
				float dist = w2s::Get3dDistance(Entity->Position, LocalPlayer->HeadPos);
				std::string distText = "[" + std::to_string((int)dist); distText += "]";
				Draw::DrawName(Pos2D, ImColor{ Settings::Enm::Color[0], Settings::Enm::Color[1] , Settings::Enm::Color[2] , 255.f }, distText.c_str());
			}
			if (Settings::Enm::DrawName) {
				Draw::DrawName(HeadPos2D, ImColor{ Settings::Enm::Color[0], Settings::Enm::Color[1] , Settings::Enm::Color[2] ,  255.f }, Entity->Name);
			}
			if (Settings::Enm::DrawLines) {
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ (float)RefDef->ScreenX / 2, (float)RefDef->ScreenY }, Pos2D, ImColor{ Settings::Enm::Color[0], Settings::Enm::Color[1] , Settings::Enm::Color[2] , 255.f }, Settings::Enm::Thickness);
			}
		}
	}
	
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		show = !show;
	}

	return pEndScene(pDevice);
}

void MainThread(HMODULE hModule) {
	HWND GameHwnd = FindWindow(0,"Call of Duty 4");

	oWndProc = (WNDPROC)SetWindowLongPtr(GameHwnd, GWL_WNDPROC, (LONG_PTR)WndProc);

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	IDirect3DDevice9* pDevice = nullptr;
	D3DPRESENT_PARAMETERS D3Dparams;
	D3Dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3Dparams.hDeviceWindow = GameHwnd;
	D3Dparams.Windowed = true;
	HRESULT Device = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3Dparams.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3Dparams, &pDevice);
	if (Device != S_OK) {
		D3Dparams.Windowed = false;
		pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3Dparams.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3Dparams, &pDevice);
		if (Device != S_OK) {
			return;
		}
	}

	void** vTable = *(void***)(pDevice);
	pEndScene = (EndScene)DetourFunction((PBYTE)vTable[42], (PBYTE)hkEndScene);

	pDevice->Release();
	pD3D->Release();

	while (!GetAsyncKeyState(VK_F11));
	DetourRemove((PBYTE)pEndScene, (PBYTE)hkEndScene);

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPARAM lParam) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, 0);
	}
}
