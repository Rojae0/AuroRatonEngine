#pragma once

#include "../system/renderdoc_app.h"

// RenderDoc API 포인터
extern RENDERDOC_API_1_1_2* g_rdocApi;

// 1. RenderDoc DLL 로드 및 API 초기화
bool InitRenderDoc();

// 2. ImGui 렌더링 루프 내부에서 호출할 함수
void RenderImGuiRenderDocMenu();