#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#define GL_SILENCE_DEPRECATION

#include <vector>
#include <tuple>

#include "Core/Logger.hpp"

using namespace ImGui;

class Console {
public:

	static void renderConsole() {
        static const float indentSize = GetStyle().IndentSpacing;
        static ImGuiWindowFlags consoleFlags = ImGuiWindowFlags_HorizontalScrollbar;

        if (Begin("Console", nullptr, consoleFlags)) {
            Separator();

            Indent(indentSize);
            for (const auto& logEntry : Logger::getMessages()) {
                LogType type;
                std::string msg;
                std::tie(type, msg) = logEntry;

                ImVec4 logTypeColor;
                std::string logTypeStr;

                if (type == LogType::ERROR) {
                    logTypeColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                    logTypeStr = "[ERROR] ";
                }
                else if (type == LogType::DEBUG) {
                    logTypeColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                    logTypeStr = "[DEBUG] ";
                }
                else if (type == LogType::INFO) {
                    logTypeColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                    logTypeStr = "[INFO] ";
                }
                else if (type == LogType::OPENGL) {
                    logTypeColor = ImVec4(1.0f, 0.0f, 0.9f, 1.0f);
                    logTypeStr = "[OPENGL] ";
                }

                TextColored(logTypeColor, "%s", logTypeStr.c_str());
                SameLine();
                TextUnformatted(msg.c_str());
                Separator();
            }

            Unindent(indentSize);

            End();
        }
	}
};