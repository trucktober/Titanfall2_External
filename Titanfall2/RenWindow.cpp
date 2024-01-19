#include "includes.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> 
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void HelpMarker(const char* desc, const char* Marker) {
    ImGui::TextDisabled(Marker);
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

LRESULT CALLBACK readKBD(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT* temp = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
    if (temp->vkCode == VK_LCONTROL && wParam == WM_KEYDOWN) {
        tog.ToggleMenu = !tog.ToggleMenu;
    }
    if (temp->vkCode == VK_SPACE && wParam == WM_KEYDOWN) {
        Stog.addJump = true;
    }
    else if (temp->vkCode == VK_SPACE && wParam == WM_KEYUP) {
        Stog.addJump = false;
    }
    if (temp->vkCode == VK_LSHIFT && wParam == WM_KEYDOWN) {
        Stog.activatePTF = true;
    }
    else if (temp->vkCode == VK_LSHIFT && wParam == WM_KEYUP) {
        Stog.activatePTF = false;
    }
    if (temp->vkCode == 0x57 && wParam == WM_KEYDOWN) {
        Stog.w = true;
    }
    else if (temp->vkCode == 0x57 && wParam == WM_KEYUP) {
        Stog.w = false;
    }
    if (temp->vkCode == 0x44 && wParam == WM_KEYDOWN) {
        Stog.d = true;
    }
    else if (temp->vkCode == 0x44 && wParam == WM_KEYUP) {
        Stog.d = false;
    }
    if (temp->vkCode == 0x53 && wParam == WM_KEYDOWN) {
        Stog.s = true;
    }
    else if (temp->vkCode == 0x53 && wParam == WM_KEYUP) {
        Stog.s = false;
    }
    if (temp->vkCode == 0x41 && wParam == WM_KEYDOWN) {
        Stog.a = true;
    }
    else if (temp->vkCode == 0x41 && wParam == WM_KEYUP) {
        Stog.a = false;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    const char* glsl_version = "#version 130";

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_FLOATING, GL_TRUE);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GL_TRUE);
    
    uintptr_t width = GetSystemMetrics(SM_CXSCREEN);
    uintptr_t height = GetSystemMetrics(SM_CYSCREEN);

    GLFWwindow* window = glfwCreateWindow(width, height, "", nullptr, nullptr);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GL_FALSE);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsWildZazzy();
    ImGui::SetWindowFocus();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 230.f, 200.f });
    ImGui::GetStyle().WindowBorderSize = 0.f;

    mem m;
    Matrix VM;
    Rotate r;
    MSG msg;
    HHOOK KBDHook = SetWindowsHookEx(WH_KEYBOARD_LL, readKBD, NULL, 0);  
    Vec2 Pos;
    Vec2 HPos;
    Vec3 HOrigin = {};
    uintptr_t T2 = m.process(L"Titanfall2.exe");
    float FOVmax = m.readmem<float>(addr.FOVmax);
    float FOV = m.readmem<float>(addr.FOV);
    static int radarPos = 0;
    while (!glfwWindowShouldClose(window))

    {
        uintptr_t localPlayer = m.readmem<uintptr_t>(addr.localplayer);

        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        if (tog.ToggleMenu && !Stog.singleplayer) {
            ImGui::Begin("Not Fortnite");



            if (ImGui::BeginTabBar("Cheats")) {

                if (ImGui::BeginTabItem("ESP")) {
                    ImGui::SeparatorText("ESP");
                    ImGui::Checkbox("Fortnite Snapline", &tog.SnapLine);
                    ImGui::SameLine();
                    if (ImGui::TreeNodeEx("Color  ")) {
                        ImGui::SameLine(NULL, 14.f);
                        ImGui::ColorEdit3("Snapline Color", Stog.Line_color, ImGuiColorEditFlags_NoInputs);
                        ImGui::TreePop();
                    }

                    ImGui::Checkbox("Fortnite Box", &tog.ESP);
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Color ")) {
                        ImGui::SameLine(NULL, 14.f);
                        ImGui::ColorEdit3("Box Color", Stog.Box_color, ImGuiColorEditFlags_NoInputs);
                        ImGui::TreePop();
                    }
                    ImGui::Checkbox("Fortnite Bone", &tog.BoneESP);
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Color")) {
                        ImGui::SameLine(NULL, 14.f);
                        ImGui::ColorEdit3("Bone Color", Stog.Bone_color, ImGuiColorEditFlags_NoInputs);
                        ImGui::TreePop();
                    }
                    ImGui::SeparatorText("Chams");
                    ImGui::Checkbox("Fortnite Chams?", &tog.Chams);
                    ImGui::SeparatorText("Options");
                    if (ImGui::TreeNode("Snaplines")) {
                        ImGui::Checkbox("Show NPC", &tog.Lines_NPC_ESP);
                        ImGui::Checkbox("Show Friendly NPC", &tog.Lines_F_NPC_ESP);
                        ImGui::Checkbox("Show Pilot", &tog.Lines_Pilot_ESP);
                        ImGui::Checkbox("Show Friendly Pilot", &tog.Lines_F_Pilot_ESP);
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Box")) {
                        ImGui::Checkbox("Show NPC", &tog.Box_NPC_ESP);
                        ImGui::Checkbox("Show Friendly NPC", &tog.Box_F_NPC_ESP);
                        ImGui::Checkbox("Show Pilot", &tog.Box_Pilot_ESP);
                        ImGui::Checkbox("Show Friendly Pilot", &tog.Box_F_Pilot_ESP);
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Bone")) {
                        ImGui::Checkbox("Show NPC", &tog.Bone_NPC_ESP);
                        ImGui::Checkbox("Show Friendly NPC", &tog.Bone_F_NPC_ESP);
                        ImGui::Checkbox("Show Pilot", &tog.Bone_Pilot_ESP);
                        ImGui::Checkbox("Show Friendly Pilot", &tog.Bone_F_Pilot_ESP);
                        ImGui::TreePop();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Aimbot")) {
                    ImGui::SeparatorText("Aimbot");
                    ImGui::Checkbox("Fortnite Bone Aimbot?", &tog.Aimbot);
                    ImGui::Indent();
                    if (!tog.Aimbot) { tog.aimBody = false; tog.aimHead = false; tog.aimTog = false; }
                    else if (!tog.aimTog) { tog.aimHead = true, tog.aimTog = true; }
                    if (ImGui::Checkbox("Fortnite Aim Head?", &tog.aimHead)) { tog.aimBody = false; }
                    if (ImGui::Checkbox("Fortnite Aim Body?", &tog.aimBody)) { tog.aimHead = false; }
                    ImGui::Unindent();
                    ImGui::SeparatorText("Aimbot Options");
                    ImGui::Checkbox("Show FOV Circle", &tog.AimbotCirc);
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Color   ")) {
                        ImGui::SameLine(NULL, 14.f);
                        ImGui::ColorEdit3("Circle Color", Stog.Circ_color, ImGuiColorEditFlags_NoInputs);
                        ImGui::TreePop();
                    }
                    ImGui::SliderFloat("FOV", &tog.aimFOV, 1.f, 150.f, "%1.f");
                    ImGui::SliderFloat("Smoothing", &tog.smoothVal, 1.f, 50.f, "%1.f");
                    ImGui::SliderFloat("Distance", &tog.distance, 1.f, 5000.f, "%10.f");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Misc")) {
                    ImGui::SeparatorText("Client");
                    ImGui::SliderFloat("FOV", &FOV, 1, 2.1f, "%.1f");
                    ImGui::SeparatorText("Singleplayer");
                    ImGui::Checkbox("Enable Singeplayer", &Stog.singleplayer);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Menu Options")) {
                    ImGui::SeparatorText("Style");
                    ImGui::ShowStyleSelector("Style");
                    ImGui::EndTabItem();

                }
                ImGui::EndTabBar();

            }

            ImGui::End();
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GL_FALSE);

        }
        else {
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GL_TRUE);
        }

        if (Stog.singleplayer && tog.ToggleMenu) {

            ImGui::Begin("Not Fortnite Singleplayer", &Stog.singleplayer);
            if (ImGui::BeginTabBar("Cheats")) {
                if (ImGui::BeginTabItem("Movement")) {
                    ImGui::SeparatorText("Jump");
                    ImGui::Checkbox("Fly Jump", &Stog.infJump);
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Multiplier")) {
                        ImGui::SliderFloat("Jump Multiplier", &Stog.jumpMul, 1.5f, 25.f, "%0.1f");
                        ImGui::TreePop();
                    }
                    ImGui::Checkbox("Long Jump", &Stog.longJump);
                    ImGui::Checkbox("Infinite Trust", &Stog.infThrust);
                    ImGui::SeparatorText("Flight");
                    ImGui::Checkbox("Point to Fly", &Stog.PTF); ImGui::SameLine(); HelpMarker("Default Keybind 'Shift'", "(?)");
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Speed ")) {
                        ImGui::SliderFloat("PTF Speed", &Stog.ptfSpeed, 1.f, 25.f, "%0.1f");
                        ImGui::TreePop();

                    }
                    ImGui::Checkbox("Fly", &Stog.Fly); ImGui::SameLine(); HelpMarker("'Control' = Down | 'Space' = Up", "(?)");
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Speed")) {
                        ImGui::SliderFloat("Fly Speed", &Stog.flySpeed, 1.f, 25.f, "%0.1f");
                        ImGui::TreePop();
                    }
                    ImGui::SeparatorText("Misc");
                    ImGui::Checkbox("No Clip", &Stog.noClip);
                    ImGui::Checkbox("Speed Hack", &Stog.Speed);

                    ImGui::Checkbox("Test", &Stog.test);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Player")) {
                    ImGui::SeparatorText("Player");
                    ImGui::Checkbox("Infinite Health", &Stog.infHealth);
                    ImGui::Checkbox("Hide Player", &Stog.hidePlayer); ImGui::SameLine(); HelpMarker("Throw Gernade to Reset", "(?)");
                    ImGui::SeparatorText("Weapon");
                    ImGui::Checkbox("Infinite Ammo", &Stog.infAmmo);
                    ImGui::Checkbox("Rapid Fire", &Stog.rapidFire); ImGui::SameLine(); HelpMarker("Fire animation will be disabled", "(!)");
                    ImGui::Checkbox("Fast Swap", &Stog.fastSwap);
                    ImGui::Checkbox("No Spread", &Stog.noSpread);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Aimbot")) {
                    ImGui::SeparatorText("Aimbot");
                    ImGui::Checkbox("Aimbot", &Stog.S_Aimbot);
                    ImGui::Indent();
                    if (ImGui::Checkbox("Aim Head", &Stog.entEye)) { Stog.entBody = false; }
                    if (ImGui::Checkbox("Aim Body", &Stog.entBody)) { Stog.entEye = false; }
                    if (!Stog.S_Aimbot) { Stog.S_Aimbot_Tog = false; Stog.entBody = false; Stog.entEye = false; }
                    else if (!Stog.S_Aimbot_Tog) { Stog.S_Aimbot_Tog = true; Stog.entEye = true; }
                    ImGui::Unindent();
                    ImGui::SeparatorText("Aimbot Options");
                    ImGui::Checkbox("Show FOV Circle", &Stog.AimCirc);
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Color   ")) {
                        ImGui::SameLine(NULL, 14.f);
                        ImGui::ColorEdit3("Circle Color", Stog.Circ_color, ImGuiColorEditFlags_NoInputs);
                        ImGui::TreePop();
                    }
                    ImGui::SliderFloat("FOV", &tog.aimFOV, 1.f, 150.f, "%1.f");
                    ImGui::SliderFloat("Smoothing", &tog.smoothVal, 1.f, 50.f, "%1.f");
                    ImGui::SliderFloat("Distance", &tog.distance, 1.f, 5000.f, "%10.f");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("ESP")) {
                    ImGui::SeparatorText("ESP");
                    ImGui::Checkbox("Snaplines", &Stog.S_Snapline);
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Color  ")) {
                        ImGui::SameLine(NULL, 14.f);
                        ImGui::ColorEdit3("Snapline Color", Stog.Line_color, ImGuiColorEditFlags_NoInputs);
                        ImGui::TreePop();
                    }
                    ImGui::Checkbox("Box", &Stog.S_boxESP);
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Color ")) {
                        ImGui::SameLine(NULL, 14.f);
                        ImGui::ColorEdit3("Box Color", Stog.Box_color, ImGuiColorEditFlags_NoInputs);
                        ImGui::TreePop();
                    }
                    ImGui::Checkbox("Bone", &Stog.S_boneESP);
                    ImGui::SameLine();
                    if (ImGui::TreeNode("Color")) {
                        ImGui::SameLine(NULL, 14.f);
                        ImGui::ColorEdit3("Bone Color", Stog.Bone_color, ImGuiColorEditFlags_NoInputs);
                        ImGui::TreePop();
                    }
                    ImGui::SeparatorText("Radar");
                    ImGui::Checkbox("Radar", &Stog.radar2D); if (Stog.radar2D) { Stog.radarFill = true; } else { Stog.radarFill = false; };
                    if (ImGui::TreeNode("Options")) {
                        ImGui::SeparatorText("Radar Visability");
                        ImGui::SliderFloat("Transparency", &Stog.radarAlpha, .01f, 10.0f, "%.01f");
                        ImGui::SeparatorText("Radar Dimensions");
                        
                        if (ImGui::Combo("Position", &radarPos, "Top Left\0Top Right\0Bottom Left\0Bottom Right\0Middle ( kill yourself)\0")) {
                            switch (radarPos) {
                            case 0: Stog.radarPos = { -.875f, .7f }; break;
                            case 1: Stog.radarPos = { .875f, .7f }; break;
                            case 2: Stog.radarPos = { -.875f, -.69f }; break;
                            case 3: Stog.radarPos = { .875f, -.69f }; break;
                            case 4: Stog.radarPos = { .0f, .0f }; break;
                            }           
                        }
                        ImGui::SliderFloat("Radar Size", &Stog.radarCirc, 0.1f, .30f, "%.2f");
                        ImGui::SeparatorText("Radar Rotation");
                        if (ImGui::Checkbox("Rotate Enemys", &Stog.radarRotaed)) { Stog.radarNotRotated = !Stog.radarNotRotated; };
                        if (ImGui::Checkbox("Rotate FOV", &Stog.radarNotRotated)) { Stog.radarRotaed = !Stog.radarRotaed; };
                        ImGui::SeparatorText("Radar Game Settings");
                        ImGui::Checkbox("Show NPC", &Stog.radarNPC);
                        ImGui::Checkbox("Show Friendly NPC", &Stog.radarNPC_F);
                        ImGui::Checkbox("Show Pilot", &Stog.radarPilot);
                        ImGui::Checkbox("Show Friendly Pilot", &Stog.radarPilot_F);
                        ImGui::SeparatorText("Custom Settings");
                        ImGui::Checkbox("FNBR GOD Settings", &Stog.radarMenuEX);
                        ImGui::TreePop();
                    }
                    ImGui::EndTabItem();
                }


                ImGui::EndTabBar();
            }
            ImGui::End();
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GL_FALSE);
        }
        if (Stog.radarMenuEX && tog.ToggleMenu) {
           
            if (ImGui::Begin("Custom Radar Settings", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {     
                ImGui::SetWindowSize({ 550.f, 500.f });
            if (ImGui::BeginTabBar("Settings")) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 2.f));            
                ImGui::SameLine(187.f); 
                if (ImGui::Button("Exit")) { Stog.radarMenuEX = false; }; 
                ImGui::PopStyleVar();
                ImGui::ShowDemoWindow();
                if (ImGui::BeginTabItem("Aesthetic")) {
                    ImGui::SeparatorText("Radar");
                    ImGui::Checkbox("Show Radar", &Stog.radar2D); if (Stog.radar2D) { Stog.radarFill = true; }
                    else { Stog.radarFill = false; };
                    ImGui::SeparatorText("Position");
                    ImGui::SliderFloat("X-Axis", &Stog.radarPos.x, -1.f, 1.f, "%.2f");                  
                    ImGui::SliderFloat("Y-Axis", &Stog.radarPos.y, -1.f, 1.f, "%.2f");
                    ImGui::SeparatorText("Sizing");
                    ImGui::SliderFloat("Circumference", &Stog.radarCirc, 0.01f, 5.f, "%.3f");
                    ImGui::SliderFloat("Player Size", &Stog.PlayerSize, .01f, 100.0f, "%.01f");
                    ImGui::SliderFloat("Pilot Size", &Stog.PilotSize, .01f, 100.0f, "%.01f");
                    ImGui::SliderFloat("NPC Size", &Stog.NPCSize, .01f, 100.0f, "%.01f");
                    ImGui::SeparatorText("Radar Visability");
                    ImGui::SliderFloat("Background Transparency", &Stog.radarAlpha, .01f, 10.0f, "%.01f");
                    ImGui::SliderFloat("Border Transparency", &Stog.radarBorderAlpha, .01f, 10.0f, "%.01f");
                    ImGui::SeparatorText("Options");
                    ImGui::Checkbox("Show Border", &Stog.radarBorder);
                    ImGui::Checkbox("Show Backround", &Stog.radarBackground);
                    ImGui::Checkbox("Show Player Dot", &Stog.radarLocalPlayer);
                    ImGui::Checkbox("Show Edge Dots", &Stog.radarEdgeDot);
                    ImGui::Checkbox("Show NPC", &Stog.radarNPC);
                    ImGui::Checkbox("Show Friendly NPC", &Stog.radarNPC_F);
                    ImGui::Checkbox("Show Pilot", &Stog.radarPilot);
                    ImGui::Checkbox("Show Friendly Pilot", &Stog.radarPilot_F);
                    ImGui::SeparatorText("Color");
                    ImGui::ColorEdit3("Radar Background Color", Stog.radar_Circ_color, ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit3("Player Color", Stog.player_dot_color, ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit3("NPC Color", Stog.NPC_dot_color, ImGuiColorEditFlags_NoInputs);       
                    if (Stog.radarBorder) { ImGui::ColorEdit3("Border Color", Stog.Border_color, ImGuiColorEditFlags_NoInputs); };
                        ImGui::EndTabItem();
                    }
                if (ImGui::BeginTabItem("Gameplay")) {
                    ImGui::SeparatorText("Radar Game Settings");
                    ImGui::SliderFloat("Render Distance", &Stog.radarRenderDist, 1.f, 10000.f, "%.f");
                    ImGui::SeparatorText("Radar Rotation");
                    if (ImGui::Checkbox("Rotate Enemys", &Stog.radarRotaed)) { Stog.radarNotRotated = !Stog.radarNotRotated; };
                    if (ImGui::Checkbox("Rotate FOV", &Stog.radarNotRotated)) { Stog.radarRotaed = !Stog.radarRotaed; };
                    

                    ImGui::EndTabItem();
                }
                
              }
                ImGui::EndTabBar();
            }           
            ImGui::End();
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GL_FALSE);
        }


        m.VirtProtc(addr.FOVmax, 2.4f);
        m.writemem(addr.FOV, FOV);



        //if (tog.SnapLine) {
        //    Matrix VM = m.readmem<Matrix>(addr.VM);
        //    for (uintptr_t i = 0; i < 100; ++i) {
        //        uintptr_t entity = m.readmem<uintptr_t>(addr.entList + i * 0x20);
        //        uintptr_t player = m.readmem<uintptr_t>(entity + off.playerName);
        //        uintptr_t isAlive = m.readmem<int>(entity + off.isAlive);
        //        if (entity == NULL) { break; }
        //        if (player != 125780153691248) { break; }
        //        if (entity == localPlayer) { continue; }
        //        if (isAlive != 1296) { continue; }
        //        Vec3 Origin = m.readmem<Vec3>(entity + off.playerOrgiin);
        //        if (WTS(Origin, Pos, VM.VM)) {
        //            DrawLines(Pos);
        //        }
        //    }
        //}

        if (tog.SnapLine) {
            Matrix VM = m.readmem<Matrix>(addr.VM);
            
            for (uintptr_t i = 0; i < 100; ++i) {
                uintptr_t entAddr = addr.npsEntList + i * 0x8;
                uintptr_t entity = m.readmem<uintptr_t>(entAddr);
                uintptr_t player = m.readmem<uintptr_t>(entity + off.playerName);
                uintptr_t isAlive = m.readmem<int>(entity + off.isAlive);
                uintptr_t isNPC = m.readmem<long long>(m.findDMA(entAddr, Soff.entName));
                if (entity == NULL) { break; }
                if (entity == localPlayer) { continue; }
               
                bool isPilot = (isAlive == 1296) && (player == 125780153691248);
                bool isRelevantNPC = (isNPC == 7236281242589818990) || (isNPC == 7162254423226544238);

                if ((tog.Lines_Pilot_ESP && isPilot) || (tog.Lines_NPC_ESP && isRelevantNPC)) {
                    Vec3 Origin = m.readmem<Vec3>(entity + off.playerOrgiin);
                    if (WTS(Origin, Pos, VM.VM)) {
                        DrawLines(Pos);
                    }
                }
                }
            }
        

        if (tog.ESP) {
            Matrix VM = m.readmem<Matrix>(addr.VM);
            for (uintptr_t i = 0; i < 200; ++i) {
                uintptr_t entAddr = addr.npsEntList + i * 0x8;
                uintptr_t entity = m.readmem<uintptr_t>(entAddr);
                uintptr_t player = m.readmem<uintptr_t>(entity + off.playerName);
                uintptr_t isAlive = m.readmem<int>(entity + off.isAlive);
                uintptr_t entEyePosAddr = m.findDMA(addr.npsEntList + i * 0x8, off.entEyePos);
                uintptr_t entEyePos = m.readmem<float>(entEyePosAddr);
                uintptr_t isNPC = m.readmem<long long>(m.findDMA(entAddr, Soff.entName));
                uintptr_t team = m.readmem<int>(entity + off.team);
                uintptr_t localPlayer_Team = m.readmem<int>(localPlayer + off.team);
                if (entity == NULL) { break; }              
                if (entity == localPlayer) { continue; }
                
                Vec3 Origin = m.readmem<Vec3>(entity + off.playerOrgiin);
                Vec3 headOrigin = m.readmem<Vec3>(entity + off.playerOrgiin);
                headOrigin.z = entEyePos;
                int health = m.readmem<int>(entity + off.playerHealth);
                if (health > 100) { continue; }
               
                bool isPilot = (isAlive == 1296) && (player == 125780153691248) && (localPlayer_Team != team);
                bool isRelevantNPC = (isNPC == 7236281242589818990) || (isNPC == 7162254423226544238) && (localPlayer_Team != team);
                bool FriendlyPilot = (isAlive == 1296) && (player == 125780153691248) && (localPlayer_Team == team);
                bool FriendlyNPC = (isNPC == 7236281242589818990) || (isNPC == 7162254423226544238) && (localPlayer_Team == team);

                if ((tog.Box_F_NPC_ESP && FriendlyPilot) || (tog.Box_Pilot_ESP && isPilot) || (tog.Box_F_NPC_ESP && FriendlyNPC) || (tog.Box_NPC_ESP && isRelevantNPC)) { // check for grunt health and normalize
                    if (WTS(Origin, Pos, VM.VM) && WTS(headOrigin, HPos, VM.VM)) {
                        DrawBox(health, Pos, HPos);
                    }
                }
            }
        }

        if (tog.BoneESP) {

            for (uintptr_t i = 0; i < 100; ++i) {
                uintptr_t entAddr = addr.npsEntList + i * 0x8;
                uintptr_t entity = m.readmem<uintptr_t>(entAddr);
                uintptr_t player = m.readmem<uintptr_t>(entity + off.playerName);
                uintptr_t isAlive = m.readmem<int>(entity + off.isAlive);
                uintptr_t isNPC = m.readmem<long long>(m.findDMA(entAddr, Soff.entName));
                uintptr_t team = m.readmem<int>(entity + off.team);
                uintptr_t localPlayer_Team = m.readmem<int>(localPlayer + off.team);
                Vec3 Origin = m.readmem<Vec3>(entity + off.playerOrgiin);
                if (entity == NULL) { break; }
                if (entity == localPlayer) { continue; }
                               
                uintptr_t boneMatrix = m.readmem<uintptr_t>(entity + off.boneMatrix);
                int health = m.readmem<int>(entity + off.playerHealth);

                bool isPilot = (isAlive == 1296) && (player == 125780153691248) && (localPlayer_Team != team);
                bool isRelevantNPC = (isNPC == 7236281242589818990) || (isNPC == 7162254423226544238) && (localPlayer_Team != team);
                bool FriendlyPilot = (isAlive == 1296) && (player == 125780153691248) && (localPlayer_Team == team);
                bool FriendlyNPC = (isNPC == 7236281242589818990) || (isNPC == 7162254423226544238) && (localPlayer_Team == team);


                if ((tog.Box_F_NPC_ESP && FriendlyPilot) || (tog.Box_Pilot_ESP && isPilot)){
                    if (health <= 100) {
                        DrawBoneEsp(m, boneMatrix, bone4, bone8);
                        DrawBoneEsp(m, boneMatrix, bone8, bone9); // lower neck to upper neck  
                        DrawBoneEsp(m, boneMatrix, bone8, bone13); // left shoulder  
                        DrawBoneEsp(m, boneMatrix, bone13, bone17); // left elbow
                        DrawBoneEsp(m, boneMatrix, bone17, bone18); // left wrist
                        DrawBoneEsp(m, boneMatrix, bone18, bone19); // left hand 
                        DrawBoneEsp(m, boneMatrix, bone8, bone37); // right shoulder
                        DrawBoneEsp(m, boneMatrix, bone37, bone41); // right elbow           
                        DrawBoneEsp(m, boneMatrix, bone41, bone42); //right wrist
                        DrawBoneEsp(m, boneMatrix, bone42, bone43); // rigth hand
                        //DrawBoneEsp(m, boneMatrix, bone4, bone60); // left pelvis
                        //DrawBoneEsp(m, boneMatrix, bone4, bone65); //rigth pelvis
                        DrawBoneEsp(m, boneMatrix, bone4, bone61); // left knee
                        DrawBoneEsp(m, boneMatrix, bone61, bone62); //left foot     
                        DrawBoneEsp(m, boneMatrix, bone63, bone63); //left foot
                        DrawBoneEsp(m, boneMatrix, bone4, bone66); //rigth knee
                        DrawBoneEsp(m, boneMatrix, bone66, bone67);// right foot
                    }
                }


            }
        }

        if (tog.Aimbot) {
            float circ = tog.aimFOV / 65.f;
            Vec2 pos = { 0, 0 };
            if (tog.AimbotCirc) { DrawCircle(circ, pos); }

            for (uintptr_t i = 0; i < 100; ++i) {
                uintptr_t entAddr = addr.npsEntList + i * 0x8;
                uintptr_t entity = m.readmem<uintptr_t>(entAddr);
                uintptr_t player = m.readmem<uintptr_t>(entity + off.playerName);
                uintptr_t isADS = m.readmem<int>(addr.ADS);
                uintptr_t isAlive = m.readmem<int>(entity + off.isAlive);
                uintptr_t yawAddr = player + off.yaw;
                uintptr_t pitchAddr = player + off.pitch;
                uintptr_t isNPC = m.readmem<long long>(m.findDMA(entAddr, Soff.entName));
                if (entity == NULL) { break; }             
                if (entity == localPlayer) { continue; }

                Vec3 Origin = m.readmem<Vec3>(addr.eyePos);
                Vec3 EOrigin = m.readmem<Vec3>(entity + off.playerOrgiin);
                if (tog.aimHead) {
                    uintptr_t entEyePosAddr = m.findDMA(addr.entList + i * 0x20, off.entEyePos);
                    uintptr_t entEyePos = m.readmem<float>(entEyePosAddr);
                    EOrigin.z = entEyePos;
                }
                if (tog.aimBody) {
                    uintptr_t entBodyPosAddr = m.findDMA(addr.entList + i * 0x20, off.entBodyPos);
                    uintptr_t entBodyPos = m.readmem<float>(entBodyPosAddr);
                    EOrigin.z = entBodyPos;

                }

                bool isPilot = (isAlive == 1296) && (player == 125780153691248);
                bool isRelevantNPC = (isNPC == 7236281242589818990) || (isNPC == 7162254423226544238);

                if (Aim(m, EOrigin, Origin, tog.c, r, pitchAddr, yawAddr) && isADS == 108) {

                    if (tog.c <= tog.distance) {
                        SetAim(m, r, tog.smoothVal, tog.aimFOV, pitchAddr, yawAddr);
                    }

                }


            }
        }

        if (tog.Chams) {
            ChamsON(m, localPlayer);
        }
        else {
            ChamsOFF(m, localPlayer);
        }

        // ---------------------------------------------------------------------------------------------
        //SINGLEPLAYER CODE
        if (Stog.infJump) {
            if (Stog.addJump) {
                m.nopEx((BYTE*)(Saddr.velocity), 5);
                Fly(m);
                resetVelZ(m);
            }
            else {
                m.patchEx((BYTE*)(Saddr.velocity), (BYTE*)"\xF3\x0F\x11\x77\x08", 5);
            }

        }

        if (Stog.longJump) {
            if (Stog.addJump) {
                Sleep(50);
                m.nopEx((BYTE*)(Saddr.velocity), 5);
            }
            else {
                m.patchEx((BYTE*)(Saddr.velocity), (BYTE*)"\xF3\x0F\x11\x77\x08", 5);
            }
        }

        if (Stog.infAmmo) {
            m.nopEx((BYTE*)(Saddr.infAmmo), 6);
        }
        else {
            m.patchEx((BYTE*)(Saddr.infAmmo), (BYTE*)"\x89\xB7\xDC\x0E\x00\x00", 6);
        }

        if (Stog.infHealth) {
            uintptr_t localplayer = m.readmem<uintptr_t>(Saddr.localPlayer);
            int health = m.readmem<int>(localplayer + off.playerHealth);
            m.writemem<int>(localplayer + Soff.playerHealth, 8999);
        }

        if (Stog.fastSwap) {
            m.nopEx((BYTE*)(Saddr.fastSwap1), 8);
            m.nopEx((BYTE*)(Saddr.fastSwap2), 8);
            //m.nopEx((BYTE*)(Saddr.fastSwap3), 8);
            //m.nopEx((BYTE*)(Saddr.fastSwap4), 8);
        }
        else {
            m.patchEx((BYTE*)(Saddr.fastSwap1), (BYTE*)"\xF3\x0F\x11\xB7\xE8\x0E\x00\x00", 8);
            m.patchEx((BYTE*)(Saddr.fastSwap2), (BYTE*)"\xF3\x0F\x11\xBF\xA4\x0E\x00\x00", 8);
        }

        if (Stog.noSpread) {
            m.nopEx((BYTE*)(Saddr.nospread1), 8);
            m.nopEx((BYTE*)(Saddr.nospread1), 8);
        }
        else {
            m.patchEx((BYTE*)(Saddr.nospread1), (BYTE*)"\xF3\x0F\x11\xB3\x00\x10\x00\x00", 8);
            m.patchEx((BYTE*)(Saddr.nospread2), (BYTE*)"\xF3\x0F\x11\xB3\x00\x10\x00\x00", 8);
        }


        if (Stog.PTF) {
            if (Stog.activatePTF) {
                resetVelAll(m);
                m.nopEx((BYTE*)(Saddr.velocity), 5);
                PointToFly(r, m);
            }
            else {
                m.patchEx((BYTE*)(Saddr.velocity), (BYTE*)"\xF3\x0F\x11\x77\x08", 5);
                m.patchEx((BYTE*)(Saddr.Xvelocity), (BYTE*)"\xF3\x0F\x11\x37", 4);
                m.patchEx((BYTE*)(Saddr.Yvelocity), (BYTE*)"\xF3\x0F\x11\x77\x04", 5);
            }
        }

        if (Stog.Fly) {
            m.nopEx((BYTE*)(Saddr.velocity), 5);
            m.nopEx((BYTE*)(Saddr.Xvelocity), 4);
            m.nopEx((BYTE*)(Saddr.Yvelocity), 5);
            resetVelAll(m);
            Fly(m, r);
            Stog.togFly = false;
        }
        else if (!Stog.togFly) {
            m.patchEx((BYTE*)(Saddr.velocity), (BYTE*)"\xF3\x0F\x11\x77\x08", 5);
            m.patchEx((BYTE*)(Saddr.Xvelocity), (BYTE*)"\xF3\x0F\x11\x37", 4);
            m.patchEx((BYTE*)(Saddr.Yvelocity), (BYTE*)"\xF3\x0F\x11\x77\x04", 5);
            Stog.togFly = true;
        }


        if (Stog.hidePlayer) {
            m.nopEx((BYTE*)(Saddr.hidePlayer), 3);
        }
        else {
            m.patchEx((BYTE*)(Saddr.hidePlayer), (BYTE*)"\xFF\x43\x18", 3);
        }

        if (Stog.infThrust) {
            m.nopEx((BYTE*)(Saddr.infThrust), 6);
        }
        else {
            m.patchEx((BYTE*)(Saddr.infThrust), (BYTE*)"\x89\xB3\xD4\x15\x00\x00", 6);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);


        if (Stog.rapidFire) {
            m.nopEx((BYTE*)(Saddr.rapidFire1), 8);
            m.nopEx((BYTE*)(Saddr.rapidFire2), 8);
        }
        else {
            m.patchEx((BYTE*)(Saddr.rapidFire1), (BYTE*)"\xF3\x0F\x10\x8F\xC8\x0E\x00\x00", 8);
            m.patchEx((BYTE*)(Saddr.rapidFire2), (BYTE*)"\xF3\x0F\x11\xBF\xC4\x0E\x00\x00", 8);
        }

        if (Stog.noClip) {
            noClip(m, r);
        }


        if (Stog.Speed) {
            SpeedHack(m, r);
        }
        if (Stog.S_Aimbot) {
            float circ = tog.aimFOV / 65.f;
            float minDist = FLT_MAX;
            Vec3 ClosestEnemy = {};
            Vec2 pos = { 0, 0 };
            if (Stog.AimCirc) { DrawCircle(circ, pos); }
            uintptr_t isADS = m.readmem<int>(addr.ADS);
            Vec3 eyePos = m.readmem<Vec3>(addr.eyePos);
            for (unsigned int i = 0; i < 200; i++) {
                uintptr_t entList = m.readmem<uintptr_t>(i * 0x8 + addr.npsEntList);
                if (entList == NULL) break;
                uintptr_t entName = m.readmem<long long>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entName));
                if (entName != 7236281242589818990 && entName != 7162254423226544238) { continue; }
                int health = m.readmem<int>(entList + off.playerHealth);
                if (health <= 0) break;
                Vec3 Enemy_eyePos = m.readmem<Vec3>(entList + off.playerOrgiin);

                Vec3 Diff = { Enemy_eyePos.x - eyePos.x, Enemy_eyePos.y - eyePos.y, Enemy_eyePos.z - eyePos.z };
                float c = sqrt(Diff.x * Diff.x + Diff.y * Diff.y + Diff.z * Diff.z);


                if (Stog.entEye) {
                    float Enemy_eyePosAddr = m.readmem<float>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entEye));
                    Enemy_eyePos.z = Enemy_eyePosAddr;
                }
                if (Stog.entBody) {
                    float Enemy_BodyPosAddr = m.readmem<float>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entBody));
                    Enemy_eyePos.z = Enemy_BodyPosAddr;
                }
                if (c < minDist) {
                    minDist = c;
                    ClosestEnemy = Enemy_eyePos;
                }
            }
            if (isADS == 108) {
                S_Aimbot(m, r, eyePos, ClosestEnemy);
            }
        }

        if (Stog.S_Snapline) {
            Matrix VM = m.readmem<Matrix>(addr.VM);
            for (uintptr_t i = 0; i < 200; ++i) {
                uintptr_t entList = m.readmem<uintptr_t>(i * 0x8 + addr.npsEntList);

                if (entList == NULL) { break; }

                uintptr_t entName = m.readmem<long long>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entName));
                if (entName != 7236281242589818990 && entName != 7162254423226544238) { continue; }
                int health = m.readmem<int>(entList + off.playerHealth);
                if (health <= 0) break;
                Vec3 Enemy_Origin = m.readmem<Vec3>(entList + off.playerOrgiin);

                if (WTS(Enemy_Origin, Pos, VM.VM)) {
                    DrawLines(Pos);
                }
            }
        }

        if (Stog.S_boxESP) {
            Matrix VM = m.readmem<Matrix>(addr.VM);
            int health = 0;
            for (uintptr_t i = 0; i < 200; ++i) {
                uintptr_t entList = m.readmem<uintptr_t>(i * 0x8 + addr.npsEntList);

                if (entList == NULL) { break; }
                if (entList == localPlayer) { continue; }
                uintptr_t entName = m.readmem<long long>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entName));
                if (entName != 7236281242589818990 && entName != 7162254423226544238) { continue; }

                Vec3 Enemy_Origin = m.readmem<Vec3>(entList + off.playerOrgiin);
                Vec3 Enemy_eyePos = m.readmem<Vec3>(entList + off.playerOrgiin);
                health = m.readmem<int>(entList + off.playerHealth);
                if (health <= 0) continue;
                if (health == 130) { health -= 30; }
                else if (health == 90) { health += 10; }
                else if (health < 0) { health = 0; }
                float Enemy_eyePosAddr = m.readmem<float>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entEye));
                Enemy_eyePos.z = Enemy_eyePosAddr;
                std::cout << health << std::endl;
                if (WTS(Enemy_Origin, Pos, VM.VM) && WTS(Enemy_eyePos, HPos, VM.VM)) {
                    DrawBox(health, Pos, HPos);
                }
            }
        }
        if (Stog.S_boneESP) {
            Matrix VM = m.readmem<Matrix>(addr.VM);
            int health = 0;
            for (uintptr_t i = 0; i < 200; ++i) {
                uintptr_t entList = m.readmem<uintptr_t>(i * 0x8 + addr.npsEntList);

                if (entList == NULL) { break; }
                if (entList == localPlayer) { continue; }
                uintptr_t entName = m.readmem<long long>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entName));
                if (entName != 7236281242589818990 && entName != 7162254423226544238) { continue; }

                uintptr_t boneMatrix = m.readmem<uintptr_t>(entList + off.boneMatrix);
                health = m.readmem<int>(entList + off.playerHealth);
                if (health <= 0) continue;
                if (health == 130) { health -= 30; }
                else if (health == 90) { health += 10; }
                else if (health < 0) { health = 0; }

                DrawBoneEsp(m, boneMatrix, bone4, bone8);
                DrawBoneEsp(m, boneMatrix, bone8, bone9); // lower neck to upper neck  
                DrawBoneEsp(m, boneMatrix, bone8, bone13); // left shoulder  
                DrawBoneEsp(m, boneMatrix, bone13, bone17); // left elbow
                DrawBoneEsp(m, boneMatrix, bone17, bone18); // left wrist
                DrawBoneEsp(m, boneMatrix, bone18, bone19); // left hand 
                DrawBoneEsp(m, boneMatrix, bone8, bone37); // right shoulder
                DrawBoneEsp(m, boneMatrix, bone37, bone41); // right elbow           
                DrawBoneEsp(m, boneMatrix, bone41, bone42); //right wrist
                DrawBoneEsp(m, boneMatrix, bone42, bone43); // rigth hand
                DrawBoneEsp(m, boneMatrix, bone4, bone61); // left knee
                DrawBoneEsp(m, boneMatrix, bone61, bone62); //left foot     
                DrawBoneEsp(m, boneMatrix, bone63, bone63); //left foot
                DrawBoneEsp(m, boneMatrix, bone4, bone66); //rigth knee
                DrawBoneEsp(m, boneMatrix, bone66, bone67);// right foot
            }
        }

        if (Stog.radar2D) { // **** FIND NO RECOIL!!! ****
            Matrix VM = m.readmem<Matrix>(addr.VM);
            Vec2 RadarRotated = { 0, 0 }, RadarLine_1 = { 0,0 }, RadarLine_2 = { 0,0 };
            DrawRadar(Stog.radarPos);
            if (Stog.radarLocalPlayer) { DrawLocalPlayerDot(Stog.radarPos); }
            if (Stog.radarBorder) { DrawRadarBorder(Stog.radarPos); }
            r.yaw = ((m.readmem<float>(m.readmem<uintptr_t>(addr.localplayer) + off.yaw)) * (PI/180)) - (PI / 2);
            Vec2 Res{ Res.x = GetSystemMetrics(SM_CXSCREEN), Res.y = GetSystemMetrics(SM_CYSCREEN) - 60.f };
            float ResScale = Res.y / Res.x;
            uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
            Vec3 player_origin = m.readmem<Vec3>(player + Soff.originOff);
            for (uintptr_t i = 0; i < 200; ++i) {
                uintptr_t entList = m.readmem<uintptr_t>(i * 0x8 + addr.npsEntList);
                if (entList == localPlayer) { continue; }
                if (entList == NULL) { break; }
                uintptr_t entName = m.readmem<long long>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entName));
                if (entName != 7236281242589818990 && entName != 7162254423226544238) { continue; }

                Vec3 enemy_origin = m.readmem<Vec3>(entList + off.playerOrgiin);

                //int health = m.readmem<int>(entList + off.playerHealth);
                //if (health <= 0) continue;
                //if (health == 130) { health -= 30; }
                //else if (health == 90) { health += 10; }
                //else if (health < 0) { health = 0; }

                Vec2 enemy_radar_pos = { (enemy_origin.x - player_origin.x) * (Stog.radarCirc / Stog.radarRenderDist), (enemy_origin.y - player_origin.y) * (Stog.radarCirc / Stog.radarRenderDist) };
                float EnemyRadarMag = enemy_radar_pos.x * enemy_radar_pos.x + enemy_radar_pos.y * enemy_radar_pos.y;

                if (Stog.radarRotaed) {                 
                    RadarRotated = { enemy_radar_pos.x * cosf(r.yaw) + enemy_radar_pos.y * sinf(r.yaw), enemy_radar_pos.x * sinf(r.yaw) - enemy_radar_pos.y * cosf(r.yaw) };
                    Vec2 NDC_Rotated = { Stog.radarPos.x + RadarRotated.x * ResScale, Stog.radarPos.y - RadarRotated.y };
                    if (EnemyRadarMag <= (Stog.radarCirc * Stog.radarCirc)) {
                        DrawNPCDot(NDC_Rotated);
                    }
                    else if (EnemyRadarMag > (Stog.radarCirc * Stog.radarCirc) && Stog.radarEdgeDot) {
                        RadarRotated.x = (RadarRotated.x / sqrt(EnemyRadarMag)) * Stog.radarCirc;
                        RadarRotated.y = (RadarRotated.y / sqrt(EnemyRadarMag)) * Stog.radarCirc;
                        Vec2 NDC_Rotated_2 = { Stog.radarPos.x + RadarRotated.x * ResScale, Stog.radarPos.y - RadarRotated.y };
                        DrawNPCDot(NDC_Rotated_2);
                    }
                    float LinePos = (5 * PI / 4);
                    LinePos *= Stog.radarCirc / sqrt(LinePos * LinePos + LinePos * LinePos);                   
                    Vec2 line_NDC_1 = { Stog.radarPos.x + LinePos * ResScale, Stog.radarPos.y + LinePos }, line_NDC_2 = { Stog.radarPos.x - LinePos * ResScale, Stog.radarPos.y + LinePos };
                    DrawRadarLine(line_NDC_1);
                    DrawRadarLine(line_NDC_2);
                }
                else if (Stog.radarNotRotated) {

                    if (EnemyRadarMag <= (Stog.radarCirc * Stog.radarCirc)) { // draw in circle
                        Vec2 NDC = { Stog.radarPos.x + enemy_radar_pos.x * ResScale, Stog.radarPos.y - enemy_radar_pos.y };
                        DrawNPCDot(NDC);
                    } else if (EnemyRadarMag > (Stog.radarCirc * Stog.radarCirc) && Stog.radarEdgeDot) { // draw edge of circle
                        enemy_radar_pos.x = (enemy_radar_pos.x / sqrt(EnemyRadarMag)) * Stog.radarCirc;
                        enemy_radar_pos.y = (enemy_radar_pos.y / sqrt(EnemyRadarMag)) * Stog.radarCirc;
                        Vec2 enemy_NDC = { Stog.radarPos.x + enemy_radar_pos.x * ResScale, Stog.radarPos.y - enemy_radar_pos.y };
                        DrawNPCDot(enemy_NDC);
                    }
                    RadarLine_1 = { 1.f * cosf(r.yaw - .8f), 1.f * sinf(r.yaw - .8f) }, RadarLine_2 = { 1.f * cosf(r.yaw + .8f), 1.f * sinf(r.yaw + .8f) };
                    float LineRadarMag = Stog.radarCirc / sqrt(RadarLine_1.x * RadarLine_1.x + RadarLine_1.y * RadarLine_1.y);
                    RadarLine_1.x *= LineRadarMag;
                    RadarLine_1.y *= LineRadarMag;
                    RadarLine_2.x *= LineRadarMag;
                    RadarLine_2.y *= LineRadarMag;
                    Vec2 player_NDC1 = { Stog.radarPos.x + RadarLine_1.x * ResScale, Stog.radarPos.y - RadarLine_1.y }, player_NDC2 = { Stog.radarPos.x + RadarLine_2.x * ResScale, Stog.radarPos.y - RadarLine_2.y };
                    if (player_NDC1.x && player_NDC1.y && player_NDC2.x && player_NDC2.y) {
                        DrawRadarLine(player_NDC1);
                        DrawRadarLine(player_NDC2);
                    }
                }

            }
        }

        //if (Stog.radarCirc) {
        //    // SETUP 
        //    Matrix VM = m.readmem<Matrix>(addr.VM);
        //    Vec2 RadarRotated = { 0, 0 }, RadarLine_1 = { 0,0 }, RadarLine_2 = { 0,0 };
        //    DrawRadar(Stog.radarPos);
        //    if (Stog.radarLocalPlayer) { DrawLocalPlayerDot(Stog.radarPos); }
        //    if (Stog.radarBorder) { DrawRadarBorder(Stog.radarPos); }
        //    r.yaw = ((m.readmem<float>(m.readmem<uintptr_t>(addr.localplayer) + off.yaw)) * (PI / 180)) - (PI / 2);
        //    Vec2 Res{ Res.x = GetSystemMetrics(SM_CXSCREEN), Res.y = GetSystemMetrics(SM_CYSCREEN) - 60.f };
        //    float ResScale = Res.y / Res.x;
        //    // ------- 




        //  
        //    // LOCAL PLAYER CHECK 
        //    uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
        //    Vec3 player_origin = m.readmem<Vec3>(player + Soff.originOff);
        //    // ------- 
        //    

        //    for (uintptr_t i = 0; i < 200; ++i) {
        //        uintptr_t entList = m.readmem<uintptr_t>(i * 0x8 + addr.npsEntList);
        //        if (entList == localPlayer) { continue; }
        //        if (entList == NULL) { break; }


        //        // NPC CHECK
        //        if (Stog.radarNPC) {
        //            uintptr_t isNPC = m.readmem<long long>(m.findDMA(i * 0x8 + addr.npsEntList, Soff.entName));
        //            if (isNPC != 7236281242589818990 && isNPC != 7162254423226544238) { continue; }
        //        }
        //        // ------- 

        //        // PILOT CHECK 
        //        if (Stog.radarPilot) {
        //            uintptr_t isPilot = m.readmem<uintptr_t>(entList + off.playerName);
        //            if (isPilot != 125780153691248) { continue; }
        //        }
        //        // ------- 

        //        // ALIVE CHECK 
        //        uintptr_t isAlive = m.readmem<int>(entList + off.isAlive);
        //        if (isAlive != 1296) { continue; }
        //        // ------- 


        //        // THE REST OF THE CODE LOL 
        //        Vec3 enemy_origin = m.readmem<Vec3>(entList + off.playerOrgiin);

        //        Vec2 enemy_radar_pos = { (enemy_origin.x - player_origin.x) * (Stog.radarCirc / Stog.radarRenderDist), (enemy_origin.y - player_origin.y) * (Stog.radarCirc / Stog.radarRenderDist) };
        //        float EnemyRadarMag = enemy_radar_pos.x * enemy_radar_pos.x + enemy_radar_pos.y * enemy_radar_pos.y;

        //        if (Stog.radarRotaed) {
        //            RadarRotated = { enemy_radar_pos.x * cosf(r.yaw) + enemy_radar_pos.y * sinf(r.yaw), enemy_radar_pos.x * sinf(r.yaw) - enemy_radar_pos.y * cosf(r.yaw) };
        //            Vec2 NDC_Rotated = { Stog.radarPos.x + RadarRotated.x * ResScale, Stog.radarPos.y - RadarRotated.y };
        //            if (EnemyRadarMag <= (Stog.radarCirc * Stog.radarCirc)) {
        //                DrawNPCDot(NDC_Rotated);
        //            }
        //            else if (EnemyRadarMag > (Stog.radarCirc * Stog.radarCirc) && Stog.radarEdgeDot) {
        //                RadarRotated.x = (RadarRotated.x / sqrt(EnemyRadarMag)) * Stog.radarCirc;
        //                RadarRotated.y = (RadarRotated.y / sqrt(EnemyRadarMag)) * Stog.radarCirc;
        //                Vec2 NDC_Rotated_2 = { Stog.radarPos.x + RadarRotated.x * ResScale, Stog.radarPos.y - RadarRotated.y };
        //                DrawNPCDot(NDC_Rotated_2);
        //            }
        //            float LinePos = (5 * PI / 4);
        //            LinePos *= Stog.radarCirc / sqrt(LinePos * LinePos + LinePos * LinePos);
        //            Vec2 line_NDC_1 = { Stog.radarPos.x + LinePos * ResScale, Stog.radarPos.y + LinePos }, line_NDC_2 = { Stog.radarPos.x - LinePos * ResScale, Stog.radarPos.y + LinePos };
        //            DrawRadarLine(line_NDC_1);
        //            DrawRadarLine(line_NDC_2);
        //        }
        //        else if (Stog.radarNotRotated) {

        //            if (EnemyRadarMag <= (Stog.radarCirc * Stog.radarCirc)) { // draw in circle
        //                Vec2 NDC = { Stog.radarPos.x + enemy_radar_pos.x * ResScale, Stog.radarPos.y - enemy_radar_pos.y };
        //                DrawNPCDot(NDC);
        //            }
        //            else if (EnemyRadarMag > (Stog.radarCirc * Stog.radarCirc) && Stog.radarEdgeDot) { // draw edge of circle
        //                enemy_radar_pos.x = (enemy_radar_pos.x / sqrt(EnemyRadarMag)) * Stog.radarCirc;
        //                enemy_radar_pos.y = (enemy_radar_pos.y / sqrt(EnemyRadarMag)) * Stog.radarCirc;
        //                Vec2 enemy_NDC = { Stog.radarPos.x + enemy_radar_pos.x * ResScale, Stog.radarPos.y - enemy_radar_pos.y };
        //                DrawNPCDot(enemy_NDC);
        //            }
        //        }
        //        RadarLine_1 = { 1.f * cosf(r.yaw - .8f), 1.f * sinf(r.yaw - .8f) }, RadarLine_2 = { 1.f * cosf(r.yaw + .8f), 1.f * sinf(r.yaw + .8f) };
        //        float LineRadarMag = Stog.radarCirc / sqrt(RadarLine_1.x * RadarLine_1.x + RadarLine_1.y * RadarLine_1.y);
        //        RadarLine_1.x *= LineRadarMag;
        //        RadarLine_1.y *= LineRadarMag;
        //        RadarLine_2.x *= LineRadarMag;
        //        RadarLine_2.y *= LineRadarMag;
        //        Vec2 player_NDC1 = { Stog.radarPos.x + RadarLine_1.x * ResScale, Stog.radarPos.y - RadarLine_1.y }, player_NDC2 = { Stog.radarPos.x + RadarLine_2.x * ResScale, Stog.radarPos.y - RadarLine_2.y };
        //        if (player_NDC1.x && player_NDC1.y && player_NDC2.x && player_NDC2.y) {
        //            DrawRadarLine(player_NDC1);
        //            DrawRadarLine(player_NDC2);
        //        }
        //    }

        //}
        // -----------------------------TEST AREA---------------------------
        if (Stog.test) { // **** FIND NO RECOIL!!! ****
            for (uintptr_t i = 0; i < 5; ++i) {
                if (Stog.radarNPC) {
                    if (i == 1) continue;
                    if (i == 4) continue;
                }
                if (Stog.radarPilot) {
                    if (i == 3) continue;
                }
                std::cout << i << std::endl;
            }
        }
            //
        // -----------------------------TEST AREA---------------------------

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);  
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    UnhookWindowsHookEx(KBDHook);
    return 0;
}