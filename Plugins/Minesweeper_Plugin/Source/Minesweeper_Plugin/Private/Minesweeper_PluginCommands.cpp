// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweeper_PluginCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeper_PluginModule"

void FMinesweeper_PluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Minesweeper_Plugin", "Bring up Minesweeper_Plugin window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
