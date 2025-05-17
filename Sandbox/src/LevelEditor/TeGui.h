#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiFileDialog.h"
#include "ImGuizmo.h"

namespace TeGui {
	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f)
	{
		bool valueChanged = false;

		ImGui::PushID(label.c_str());

		if (ImGui::BeginTable("Vec3Table", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
		{
			ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthFixed, 250.0f);
			ImGui::TableSetupColumn("Label");

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 });

			float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			// X
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X", buttonSize)) values.x = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			valueChanged |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// Y
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			valueChanged |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// Z
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", buttonSize)) values.z = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			valueChanged |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", label.c_str());

			ImGui::EndTable();
		}

		ImGui::PopID();
		return valueChanged;
	}

	static bool ComboBox(const std::string& label, const std::vector<std::string>& items, int& currentIndex)
	{
		bool changed = false;

		ImGui::PushID(label.c_str());

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.18f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.20f, 0.20f, 0.25f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.35f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.45f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.35f, 0.4f, 1.0f));

		float comboWidth = 150.0f;
		ImGui::SetNextItemWidth(comboWidth);
		if (ImGui::BeginCombo("##combo", items[currentIndex].c_str(), ImGuiComboFlags_HeightSmall))
		{
			for (int i = 0; i < items.size(); i++)
			{
				bool isSelected = (currentIndex == i);
				if (ImGui::Selectable(items[i].c_str(), isSelected))
				{
					currentIndex = i;
					changed = true;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		ImGui::Text("%s", label.c_str());

		ImGui::PopStyleColor(6);
		ImGui::PopStyleVar(2);
		ImGui::PopID();

		return changed;
	}
}