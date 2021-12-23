#include <Windows.h>
#include <iostream>
#include <string>v
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "WorldToScreen.h"
#include "Entity.h"
#include "Draw.h"
#include "Imgui/imconfig.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx9.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/imstb_rectpack.h"
#include "Imgui/imstb_textedit.h"
#include "Imgui/imstb_truetype.h"

#include "detours.h"
#pragma comment(lib, "detours.lib")

namespace Offsets {
	uintptr_t Fov = 0x78F610;
	uintptr_t ViewAngles = 0x793698;
	uintptr_t HeadPos = 0x7936A4;
	uintptr_t FootPos = 0x38F4BC;
	uintptr_t EntityList = 0x831270;
	uintptr_t LocalPlayerId = 0x1280500;
	uintptr_t ThirdPerson = 0x21C777C;
}

namespace Settings {
	namespace Fnd {
		bool DrawLines;
		bool DrawBox;
		bool DrawName;
		float Color[3];
	}
	namespace Enm {
		bool DrawLines;
		bool DrawBox;
		bool DrawName;
		float Color[3];
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
Vec3x3 matrix;

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
		ImGui::BeginChild("Friendly", ImVec2{300,120}, true);
		ImGui::Checkbox("Draw Line", &Settings::Fnd::DrawLines);
		ImGui::Checkbox("Draw Box", &Settings::Fnd::DrawBox);
		ImGui::Checkbox("Draw Name", &Settings::Fnd::DrawName);
		ImGui::ColorEdit3("ESP Color", Settings::Fnd::Color);
		ImGui::EndChild();

		ImGui::Text("Enemy");
		ImGui::BeginChild("Enemy", ImVec2{ 300,120 }, true);
		ImGui::Checkbox("Draw Line", &Settings::Enm::DrawLines);
		ImGui::Checkbox("Draw Box", &Settings::Enm::DrawBox);
		ImGui::Checkbox("Draw Name", &Settings::Enm::DrawName);
		ImGui::ColorEdit3("ESP Color", Settings::Enm::Color);
		ImGui::EndChild();
		ImGui::End();
	}
	
	Vec2 Fov = *(Vec2*)Offsets::Fov;
	Vec3 Angles = *(Vec3*)Offsets::ViewAngles;
	Vec3 HeadPos = *(Vec3*)Offsets::HeadPos;
	int LocalPlayerId = *(int*)Offsets::LocalPlayerId;
	int LocalTeam = *(int*)(Offsets::EntityList + LocalPlayerId * 0x4CC + 0x1C);

	for (int i = 0; i != 20; i++) {
		Ent* Entity = (Ent*)(Offsets::EntityList + i * 0x4CC);
		if (Entity->Valid) {
			Vec3 Foot3D = Entity->HeadPos; Foot3D.z -= 50;
			ImVec2 Foot2D, Head2D;
			if (!w2s::WorldToScreen(Entity->HeadPos, HeadPos, Head2D, matrix, Angles, ImGui::GetIO().DisplaySize, Fov)) { continue; }
			if (!w2s::WorldToScreen(Foot3D, HeadPos, Foot2D, matrix, Angles, ImGui::GetIO().DisplaySize, Fov)) { continue; }
			if (Entity->Team == LocalTeam) {
				if (Settings::Fnd::DrawBox) { Draw::DrawBox(Head2D, Foot2D, ImColor{ Settings::Fnd::Color[0],Settings::Fnd::Color[1],Settings::Fnd::Color[2] }, 3.f); }
				if (Settings::Fnd::DrawLines) { Draw::DrawLine(Foot2D, ImColor{ Settings::Fnd::Color[0],Settings::Fnd::Color[1],Settings::Fnd::Color[2] }, 3.f); }
				if (Settings::Fnd::DrawName) { Draw::DrawName(Head2D, ImColor{ Settings::Fnd::Color[0],Settings::Fnd::Color[1],Settings::Fnd::Color[2] }, Entity->Name); }
			}
			else {
				if (Settings::Enm::DrawBox) {
					if (Settings::Enm::DrawBox) { Draw::DrawBox(Head2D, Foot2D, ImColor{ Settings::Enm::Color[0],Settings::Enm::Color[1],Settings::Enm::Color[2] }, 3.f); }
					if (Settings::Enm::DrawLines) { Draw::DrawLine(Foot2D, ImColor{ Settings::Enm::Color[0],Settings::Enm::Color[1],Settings::Enm::Color[2] }, 3.f); }
					if (Settings::Enm::DrawName) { Draw::DrawName(Head2D, ImColor{ Settings::Enm::Color[0],Settings::Enm::Color[1],Settings::Enm::Color[2] }, Entity->Name); }
				}
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
