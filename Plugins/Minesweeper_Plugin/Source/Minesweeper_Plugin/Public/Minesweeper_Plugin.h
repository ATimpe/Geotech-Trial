// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include <vector>

using std::vector;

class FToolBarBuilder;
class FMenuBuilder;

class BoardTile
{
	public:
		BoardTile();

		int State = 0;
		TSharedPtr<SButton> Button;
		TSharedPtr<STextBlock> Text;
};

class FMinesweeper_PluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);


	TSharedPtr<SEditableTextBox> WidthTextbox;
	TSharedPtr<SEditableTextBox> HeightTextbox;
	TSharedPtr<SEditableTextBox> BombsTextbox;
	TSharedPtr<SVerticalBox> GameBox;
	TSharedPtr<STextBlock> GameMessage;

	FReply StartGame();
	void PopulateBoardUI();
	void GenerateBombs(int row, int col);
	FReply SquarePressed(int row, int col);
	void RevealSquare(int row, int col, bool reveal_bombs);
	int GetAdjacentBombs(int row, int col);
	bool HasWon();
	void Win();
	void GameOver();
	void RevealAllSquares(bool win);

	int Width;
	int Height;
	int Bombs;
	bool FirstSquare = true;
	vector<vector<BoardTile>> Board;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
