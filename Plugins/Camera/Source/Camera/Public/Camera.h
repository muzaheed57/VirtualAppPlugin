// Copyright 2019 ByOwls. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "Engine.h"
#include <string>
#include "Kismet/KismetMathLibrary.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FCameraModule : public IModuleInterface
{
public:

//	typedef TSharedPtr<FString> FComboItemType;
	TSharedPtr<FString> CurrentItem;
	TArray<TSharedPtr<FString>> Options;
	TSharedRef<SWidget> MakeWidgetForOption(TSharedPtr<FString> InOption);
	float vInterpToSpeed = 100;
	int32 coordinateMul = 500;
	bool showDebugMessage = false;
	FString ip = "192.168.110.16";
	FString port = "7777";

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	void OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type);

	FReply GetPercentage();
	const FSlateBrush* GetFirstImageBrush() const;

	//function get static value for camera object
	float s_GetVInterpToSpeed(bool canReWrite);
	int32 s_GetCoordinateMul(bool canReWrite);
	bool s_GetShowDebugMessage(bool canReWrite);
	FString s_GetIp(bool canReWrite);
	FString s_GetPort(bool canReWrite);
	FText s_GetCurrentItemLabel(bool canReWrite, bool isLocal) const;

private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);	

	TSharedPtr<class FUICommandList> PluginCommands;

	//----------------------------------------------------

	void OnTextChangedVInterpToSpeed(float vspped);
	void OnTextChangedCoordinateMul(int32 sbox);
	void OnTextChangedShowDebugMessage(ECheckBoxState newState);
	void OnTextChangedIP(const FText& InText, ETextCommit::Type);
	void OnTextChangedPort(const FText& InText, ETextCommit::Type);

};
