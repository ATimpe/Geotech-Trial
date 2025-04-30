// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "Minesweeper_PluginStyle.h"

class FMinesweeper_PluginCommands : public TCommands<FMinesweeper_PluginCommands>
{
public:

	FMinesweeper_PluginCommands()
		: TCommands<FMinesweeper_PluginCommands>(TEXT("Minesweeper_Plugin"), NSLOCTEXT("Contexts", "Minesweeper_Plugin", "Minesweeper_Plugin Plugin"), NAME_None, FMinesweeper_PluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};