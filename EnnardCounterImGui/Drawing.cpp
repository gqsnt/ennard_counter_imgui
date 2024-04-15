#include "Drawing.h"

#include <chrono>

#include "Hook.h"

BOOL Drawing::bInit = FALSE; // Status of the initialization of ImGui.
bool Drawing::bDisplay = true; // Status of the menu display.
bool Drawing::bSetPos = false; // Status to update ImGui window size / position.
ImVec2 Drawing::vWindowPos = {1645, 250}; // Last ImGui window position.
ImVec2 Drawing::vWindowSize = {0, 0}; // Last ImGui window size.


bool Drawing::isStarted = false;
std::chrono::time_point<std::chrono::steady_clock> Drawing::globalTimer = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::steady_clock>  Drawing::intervalTimer = std::chrono::high_resolution_clock::now();
int  Drawing::step = 0;




/**
    @brief : Hook of the EndScene function.
    @param  D3D9Device : Current Direct3D9 Device Object.
    @retval : Result of the original EndScene function.
**/
HRESULT Drawing::hkEndScene(const LPDIRECT3DDEVICE9 D3D9Device)
{

	if (!Hook::pDevice)
		Hook::pDevice = D3D9Device;

	if (!bInit)
		InitImGui(D3D9Device);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDisplay = !bDisplay;

	if (GetAsyncKeyState(VK_F1) & 1)
	{
		if (!isStarted)
		{
			isStarted = true;
			globalTimer = std::chrono::steady_clock::now();
			intervalTimer = std::chrono::steady_clock::now();
			step = 0;
		}
		else
		{
			isStarted = false;
		}
	}

	if (GetAsyncKeyState(VK_END) & 1)
	{
		Hook::UnHookDirectX();
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, Hook::hDDLModule, 0, nullptr);
		return Hook::oEndScene(D3D9Device);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (bDisplay)
	{
		ImGui::Begin("Ennard Counter", &bDisplay);
		{
			ImGui::SetWindowSize({225, 100}, ImGuiCond_Once);

			
			ImGui::SetWindowPos(vWindowPos);

			//vWindowPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };

			ImGui::Text("Press F1 to start the timer");

			auto current_time = std::chrono::steady_clock::now();
			auto global_duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - globalTimer);
			auto interval_duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - intervalTimer);

			if (isStarted)
			{
				if (step == 0 && interval_duration.count() > 90
					|| step == 1 && interval_duration.count() > 270
					|| step == 2 && interval_duration.count() > 90
					|| step == 3 && interval_duration.count() > 90)
				{
					intervalTimer = std::chrono::steady_clock::now();
					step++;
				}else if (step == 4 && interval_duration.count() > 90)
				{
					isStarted = false;
				}

				auto count = interval_duration.count() -1;

				long long interval = 0;
				if (step == 0 )
				{
					interval = 9 - (count % 10);
				}
				else if (step == 1)
				{
					interval = 5- (count %6);
				}
				else if (step == 2)
				{
					interval = 2-(count %3);
				}
				else if (step == 3)
				{
					interval = 1-(count %2);
				}

				ImGui::Text("Global Timer: %02lld:%02lld", global_duration.count() / 60 , global_duration.count()%60);
				ImGui::Text("Interval Timer: %02lld", interval);
				if (interval == 0){
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Ennard is coming");
				}
			}

		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Hook::oEndScene(D3D9Device);
}

/**
    @brief : function that init ImGui for rendering.
    @param pDevice : Current Direct3D9 Device Object given by the hooked function.
**/
void Drawing::InitImGui(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DDEVICE_CREATION_PARAMETERS CP;
	pDevice->GetCreationParameters(&CP);
	Hook::window = CP.hFocusWindow;
	Hook::HookWindow();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Hook::window);
	ImGui_ImplDX9_Init(pDevice);

	bInit = TRUE;
}
