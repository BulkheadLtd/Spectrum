// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumConsoleCommandWindow.h"

#include <HAL/IConsoleManager.h>

#include <imgui.h>

USpectrumConsoleCommandWindow::USpectrumConsoleCommandWindow()
{
	// Build out a local map of all available commands
	IConsoleManager::Get().ForEachConsoleObjectThatStartsWith(FConsoleObjectVisitor::CreateUObject(this, &USpectrumConsoleCommandWindow::ConsoleObjectVisitor));
}

void USpectrumConsoleCommandWindow::DrawWindowContent(float DeltaTime, UWorld* World)
{
	// Render the filter drop down
	if (ImGui::BeginCombo("Filter", RAWCHARPTR(SelectedFilter.ToString()), 0))
	{
		for (const TPair<FName, TMap<FName, IConsoleObject*>>& FilterPair : ConsoleCommands)
		{
			const bool bIsSelected = (SelectedFilter == FilterPair.Key);
			
			if (ImGui::Selectable(TCHAR_TO_ANSI((*FilterPair.Key.ToString())), bIsSelected))
			{
				SelectedFilter = FilterPair.Key;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (bIsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		
		ImGui::EndCombo();
	}

	// Render the console varable/command table applicable to the selected filter
	if (ImGui::BeginTable("", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
	{
		ImGui::TableSetupColumn("Variable/Command");
		ImGui::TableSetupColumn("Value/Execute");
		ImGui::TableHeadersRow();

		if (TMap<FName, IConsoleObject*>* RendererConsoleCommands = ConsoleCommands.Find(SelectedFilter))
		{
			for (const TPair<FName, IConsoleObject*>& RenderingCommand : *RendererConsoleCommands)
			{
				ImGui::TableNextRow();
				
				IConsoleObject* ConsoleObject = RenderingCommand.Value;

				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(TCHAR_TO_ANSI((*RenderingCommand.Key.ToString())));

				ImGui::TableSetColumnIndex(1);
				ImGui::BeginDisabled(); // Force disable for now
				if (ConsoleObject)
				{
					IConsoleVariable* ConsoleVariable = ConsoleObject->AsVariable();
					if (ConsoleVariable)
					{
						if (ConsoleVariable->IsVariableBool())
						{
							bool Var;
							ConsoleVariable->GetValue(Var);
							ImGui::Checkbox("", &Var);
						}
						else if (ConsoleVariable->IsVariableInt())
						{
							int32 Var;
							ConsoleVariable->GetValue(Var);
							ImGui::InputInt("", &Var);
						}
						else if (ConsoleVariable->IsVariableFloat())
						{
							float Var;
							ConsoleVariable->GetValue(Var);
							ImGui::InputFloat("", &Var);
						}
						else if (ConsoleVariable->IsVariableString())
						{
							FString Var;
							ConsoleVariable->GetValue(Var);
							if (!Var.IsEmpty())
							{
								ImGui::InputText("", TCHAR_TO_ANSI(GetData(Var)), Var.Len());
							}
						}
					}
					else
					{
						IConsoleCommand* ConsoleCommand = ConsoleObject->AsCommand();
						if (ConsoleCommand)
						{
							ImGui::Button("Execute");
						}
					}
				}

				ImGui::EndDisabled();
			}
		}
		
		ImGui::EndTable();
	}
}

FString USpectrumConsoleCommandWindow::GetTitle()
{
	return "Console Command Palette";
}

FVector2D USpectrumConsoleCommandWindow::GetInitialSize()
{
	return { 600.f, 800.f };
}

void USpectrumConsoleCommandWindow::ConsoleObjectVisitor(const TCHAR* Name, IConsoleObject* ConsoleObject)
{
	// Crap approach, but will do for now
	
	const FString ConsoleCommand(Name);
	FName CommandFilter = NAME_None;
	
	TArray<FString> CommandSplit;
	ConsoleCommand.ParseIntoArray(CommandSplit, TEXT("."));
	if (CommandSplit.Num() > 1)
	{
		FName* FilterName = FilterMap.Find(FName(*CommandSplit[0]));
		if (FilterName != nullptr)
		{
			CommandFilter = *FilterName;
		}
	}
	
	ConsoleCommands.FindOrAdd(CommandFilter).Add(FName(ConsoleCommand), ConsoleObject);
}
