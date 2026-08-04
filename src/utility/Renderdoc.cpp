#include "Renderdoc.h"
#include <windows.h>
#include "imgui.h"
#include <iostream>

// RenderDoc API 포인터
RENDERDOC_API_1_1_2* g_rdocApi = nullptr;

// 1. RenderDoc DLL 로드 및 API 초기화
bool InitRenderDoc()
{
    // RenderDoc이 설치된 경로의 DLL을 로드하거나, 프로젝트 폴더에 복사한 DLL을 로드합니다.
    HMODULE mod = LoadLibraryA("renderdoc.dll");

    if (!mod)
    {
        // 기본 설치 경로 예시 (필요시 경로 수정)
        mod = LoadLibraryA("C:/Program Files/RenderDoc/renderdoc.dll");
    }

    if (mod)
    {
        pRENDERDOC_GetAPI getAPI = reinterpret_cast<pRENDERDOC_GetAPI>(GetProcAddress(mod, "RENDERDOC_GetAPI"));

        if (getAPI)
        {
            int ret = getAPI(eRENDERDOC_API_Version_1_1_2, (void**)&g_rdocApi);

            return (ret == 1);
        }
    }

    return false;
}

// 2. ImGui 렌더링 루프 내부에서 호출할 함수
void RenderImGuiRenderDocMenu()
{
    ImGui::Begin("Graphics Debugger");

    if (g_rdocApi == nullptr)
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "RenderDoc API is not initialized!");
        if (ImGui::Button("Retry Init RenderDoc"))
        {
            InitRenderDoc();
        }
    }
    else
    {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "RenderDoc API Connected");

        // [버튼 1] Frame Capture 실행
        if (ImGui::Button("Trigger Capture"))
        {
            std::cout << "captured!" << "\n";
            // 현재 프레임 캡처 요청
            g_rdocApi->TriggerCapture();
        }

        ImGui::SameLine();

        // [버튼 2] RenderDoc Replay UI 실행
        if (ImGui::Button("Open RenderDoc UI"))
        {
            // RenderDoc GUI 프로그램(qrenderdoc.exe)을 실행하고 현재 응용 프로그램에 연결합니다.

            if (!g_rdocApi->IsTargetControlConnected())
            {
                // 1: Connect to target, nullptr: use default working directory
                g_rdocApi->LaunchReplayUI(1, nullptr);
            }
        }
    }

    ImGui::End();
}