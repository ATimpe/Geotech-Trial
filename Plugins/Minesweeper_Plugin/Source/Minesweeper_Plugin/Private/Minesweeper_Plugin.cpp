// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweeper_Plugin.h"
#include "Minesweeper_PluginStyle.h"
#include "Minesweeper_PluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Styling/SlateColor.h"
#include "Layout/Visibility.h"

#include <vector>

using std::vector;

static const FName Minesweeper_PluginTabName("Minesweeper_Plugin");

#define LOCTEXT_NAMESPACE "FMinesweeper_PluginModule"

const FText WinText = LOCTEXT("WinText", "You Win!");
const FText LoseText = LOCTEXT("LoseText", "You Lose. Better luck next time.");

BoardTile::BoardTile()
{
	State = 0;
	Button = TSharedPtr<SButton>();
	Text = TSharedPtr<STextBlock>();
}

void FMinesweeper_PluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeper_PluginStyle::Initialize();
	FMinesweeper_PluginStyle::ReloadTextures();

	FMinesweeper_PluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeper_PluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeper_PluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeper_PluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(Minesweeper_PluginTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeper_PluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMinesweeper_PluginTabTitle", "Minesweeper_Plugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeper_PluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeper_PluginStyle::Shutdown();

	FMinesweeper_PluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(Minesweeper_PluginTabName);
}

TSharedRef<SDockTab> FMinesweeper_PluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Minesweeper"),
		FText::FromString(TEXT("FMinesweeper_PluginModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Minesweeper_Plugin.cpp"))
	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.FillHeight(1)
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.Padding(10, 5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Width"))
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.Padding(10, 5)
						[
							SAssignNew(WidthTextbox, SEditableTextBox)
								//SNew(SEditableTextBox)
						]
				]
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.FillHeight(1)
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.Padding(10, 5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Height"))
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.Padding(10, 5)
						[
							SAssignNew(HeightTextbox, SEditableTextBox)
							//SNew(SEditableTextBox)
						]
				]
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.FillHeight(1)
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.Padding(10, 5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Bombs"))
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.Padding(10, 5)
						[
							SAssignNew(BombsTextbox, SEditableTextBox)
							//SNew(SEditableTextBox)
						]
				]
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.FillHeight(1)
				[
					SNew(SButton)
					.OnClicked(FOnClicked::CreateRaw(this, &FMinesweeper_PluginModule::StartGame))
					[
						SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.Text(FText::FromString("Bombs Away!"))
					]
				]
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.FillHeight(10)
				[
					SAssignNew(GameBox, SVerticalBox)
					//SNew(SVerticalBox)
				]
				+ SVerticalBox::Slot()
					.VAlign(VAlign_Top)
					.FillHeight(1)
					[
						SAssignNew(GameMessage, STextBlock)
						.Justification(ETextJustify::Center)
					]
		];
}

void FMinesweeper_PluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(Minesweeper_PluginTabName);
}

void FMinesweeper_PluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeper_PluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeper_PluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}


// Logic Functions

FReply FMinesweeper_PluginModule::StartGame()
{
	FirstSquare = true;
	// TODO: Check if they are numeric
	Width = FCString::Atoi(*WidthTextbox->GetText().ToString());
	Height = FCString::Atoi(*HeightTextbox->GetText().ToString());
	Bombs = FCString::Atoi(*BombsTextbox->GetText().ToString());

	
	// Must have less bombs that squares to place them
	if (Bombs >= Width * Height)
	{
		// Try Again message
		return FReply::Handled();
	}

	Board.clear();
	for (int i = 0; i < Height; i++)
	{
		Board.push_back(vector<BoardTile>());
		for (int j = 0; j < Width; j++)
		{
			Board[i].push_back(BoardTile());
		}
	}

	GameMessage->SetText(FText::FromString(""));
	
	PopulateBoardUI();
	return FReply::Handled();
}

void FMinesweeper_PluginModule::PopulateBoardUI()
{
	GameBox->ClearChildren();
	TSharedPtr<SHorizontalBox> BoardBox;
	for (int i = 0; i < Height; i++)
	{
		GameBox->AddSlot()
		[
			SAssignNew(BoardBox, SHorizontalBox)
			
		];
		for (int j = 0; j < Width; j++)
		{
			BoardBox->AddSlot()
			[
				SNew(SBox)
				.WidthOverride(10.0f)
				[
					SAssignNew(Board[i][j].Button, SButton)
					.OnClicked(FOnClicked::CreateRaw(this, &FMinesweeper_PluginModule::SquarePressed, i, j))
					[
						SAssignNew(Board[i][j].Text, STextBlock)
					]
				]
			];
		}
	}
}

void FMinesweeper_PluginModule::GenerateBombs(int row, int col)
{
	for (int i = 0; i < Bombs; i++) {
		int Random_H = rand() % Height;
		int Random_W = rand() % Width;
		// If the square isn't the one passed in or one that already has a bomb.
		if (!(Random_H == row && Random_W == col) && Board[Random_H][Random_W].State != 2)
		{
			Board[Random_H][Random_W].State = 2;
		}
		else
		{
			i--;
		}
	}
}

FReply FMinesweeper_PluginModule::SquarePressed(int row, int col)
{
	if (FirstSquare)
	{
		GenerateBombs(row, col);
		FirstSquare = false;
	}

	if (Board[row][col].State == 2) 
	{
		GameOver();
		return FReply::Handled();
	}
	else 
	{
		RevealSquare(row, col, false);
	}

	if (HasWon()) {
		Win();
	}

	return FReply::Handled();
}

void FMinesweeper_PluginModule::RevealSquare(int row, int col, bool reveal_bombs)
{
	// Skip if the square is already revealed
	if (Board[row][col].State == 1) return;

	int AdjacentBombs = GetAdjacentBombs(row, col);
	if (AdjacentBombs > 0 && Board[row][col].State != 2)
	{
		Board[row][col].Text->SetText(FText::FromString(FString::FromInt(AdjacentBombs)));
	}
	else if (Board[row][col].State == 2 && reveal_bombs)
	{
		Board[row][col].Text->SetText(FText::FromString("X"));
	}
	else if (Board[row][col].State == 2 && !reveal_bombs)
	{
		return;
	}

	Board[row][col].Button->SetBorderBackgroundColor(FSlateColor(FColor(50, 50, 50)));
	Board[row][col].Button->SetEnabled(false);
	Board[row][col].State = 1;

	if (row > 0)
	{
		RevealSquare(row - 1, col, false);
	}
	if (row < Board.size() - 1)
	{
		RevealSquare(row + 1, col, false);
	}
	if (col > 0)
	{
		RevealSquare(row, col - 1, false);
	}
	if (col < Board[0].size() - 1)
	{
		RevealSquare(row, col + 1, false);
	}
}

int FMinesweeper_PluginModule::GetAdjacentBombs(int row, int col)
{
	int BombCount = 0;

	bool HasLeftSquare = row > 0;
	bool HasRightSquare = row < Board.size() - 1;
	bool HasUpSquare = col > 0;
	bool HasDownSquare = col < Board[0].size() - 1;

	if (HasLeftSquare) BombCount += Board[row - 1][col].State == 2 ? 1 : 0;
	if (HasRightSquare) BombCount += Board[row + 1][col].State == 2 ? 1 : 0;
	if (HasUpSquare) BombCount += Board[row][col - 1].State == 2 ? 1 : 0;
	if (HasDownSquare) BombCount += Board[row][col + 1].State == 2 ? 1 : 0;
	// Diagonals
	if (HasLeftSquare && HasUpSquare) BombCount += Board[row - 1][col - 1].State == 2 ? 1 : 0;
	if (HasRightSquare && HasUpSquare) BombCount += Board[row + 1][col - 1].State == 2 ? 1 : 0;
	if (HasLeftSquare && HasDownSquare) BombCount += Board[row - 1][col + 1].State == 2 ? 1 : 0;
	if (HasRightSquare && HasDownSquare) BombCount += Board[row + 1][col + 1].State == 2 ? 1 : 0;

	return BombCount;
}

bool FMinesweeper_PluginModule::HasWon()
{
	for (vector<BoardTile> BoardRow : Board) {
		for (BoardTile BoardSquare : BoardRow) {
			if (BoardSquare.State == 0) {
				return false;
			}
		}
	}

	return true;
}

void FMinesweeper_PluginModule::Win()
{
	GameMessage->SetText(WinText);
	RevealAllSquares(true);
}

void FMinesweeper_PluginModule::GameOver()
{
	GameMessage->SetText(LoseText);
	RevealAllSquares(false);
}

void FMinesweeper_PluginModule::RevealAllSquares(bool win)
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (Board[i][j].Button->IsEnabled())
			{
				RevealSquare(i, j, true);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeper_PluginModule, Minesweeper_Plugin)