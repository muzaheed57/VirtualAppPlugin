// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Camera.h"
#include "CameraStyle.h"
#include "CameraCommands.h"
#include "LevelEditor.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Runtime/Slate/Public/Widgets/Input/SSpinBox.h"
#include "Runtime/Slate/Public/Framework/SlateDelegates.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName CameraTabName("Camera");
static const int test = 10;
#define LOCTEXT_NAMESPACE "FCameraModule"

void FCameraModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCameraStyle::Initialize();
	FCameraStyle::ReloadTextures();

	FCameraCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCameraCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FCameraModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FCameraModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FCameraModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CameraTabName, FOnSpawnTab::CreateRaw(this, &FCameraModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FCameraTabTitle", "Camera"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FCameraModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FCameraStyle::Shutdown();

	FCameraCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CameraTabName);
}

TSharedRef<SDockTab> FCameraModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FCameraModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Camera.cpp"))
		);
	Options.Empty();
	for (TObjectIterator<AActor> It; It; ++It)
	{
		Options.Add(MakeShareable(new FString(It->GetName())));
	}

	CurrentItem = Options[5];

	FString ImagePath = /*FPaths::ProjectDir() / */TEXT("D:/any/UnrealProjects/wrapper/Plugins/Camera/Resources/Icon128.png");
	FName BrushName = FName(*ImagePath);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
		SNew(SHorizontalBox)
		//left
		+ SHorizontalBox::Slot()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(SBorder)
					[
					SNew(SUniformGridPanel)
					.SlotPadding(FMargin(5.0f))
					+ SUniformGridPanel::Slot(0, 0).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Object selection"))/*.ShadowColorAndOpacity(FLinearColor::Black).ShadowOffset(FIntPoint(-1, 1)).ColorAndOpacity(FLinearColor::White)*/
							.Font(FSlateFontInfo("Verdana", 13))
						]
						+ SUniformGridPanel::Slot(1, 0)
							[
								SNew(SComboBox<TSharedPtr<FString>>)
								.OptionsSource(&Options)
								.OnSelectionChanged_Raw(this, &FCameraModule::OnSelectionChanged)
								.OnGenerateWidget_Raw(this,&FCameraModule::MakeWidgetForOption)
								.InitiallySelectedItem(CurrentItem)
								[
									SNew(STextBlock)
									.Text_Raw(this, &FCameraModule::s_GetCurrentItemLabel, false, true)
								]
							]
					+ SUniformGridPanel::Slot(0, 1).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("VInterpTo Speed"))
							.Font(FSlateFontInfo("Verdana", 13))
						]
						+ SUniformGridPanel::Slot(1, 1)
							[
								SNew(SSpinBox<float>)
								.Value(FCameraModule::s_GetVInterpToSpeed(false))
								.MaxSliderValue(100)
								.MinSliderValue(10)
								.MaxValue(100)
								.MinValue(10)
								.OnValueChanged_Raw(this, &FCameraModule::OnTextChangedVInterpToSpeed)
							]
					+ SUniformGridPanel::Slot(0, 2).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Coordinate mul"))
							.Font(FSlateFontInfo("Verdana", 13))
						]
						+ SUniformGridPanel::Slot(1, 2)
							[
								SNew(SSpinBox<int32>)
								.Value(FCameraModule::s_GetCoordinateMul(false))
								.MaxSliderValue(1000)
								.MinSliderValue(1)
								.MaxValue(1000)
								.MinValue(1)
								.OnValueChanged_Raw(this, &FCameraModule::OnTextChangedCoordinateMul)
							]
					+ SUniformGridPanel::Slot(0, 3).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Show debug message"))
							.Font(FSlateFontInfo("Verdana", 13))
						]
						+ SUniformGridPanel::Slot(1, 3)
							[
								SNew(SCheckBox)
								.HAlign(HAlign_Left)
								.OnCheckStateChanged_Raw(this, &FCameraModule::OnTextChangedShowDebugMessage)
								
							]
					+ SUniformGridPanel::Slot(0, 4).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Ip"))
							.Font(FSlateFontInfo("Verdana", 13))
						]
						+ SUniformGridPanel::Slot(1, 4)
							[
								SNew(SEditableTextBox)
								.HintText(FText::FromString("ip"))
								.Text(FText::FromString(FCameraModule::s_GetIp(false)))
								.OnTextCommitted_Raw(this, &FCameraModule::OnTextChangedIP)
							]
					+ SUniformGridPanel::Slot(0, 5).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Port"))
							.Font(FSlateFontInfo("Verdana", 13))
						]
						+ SUniformGridPanel::Slot(1, 5)
							[
								SNew(SEditableTextBox)
								.HintText(FText::FromString("port"))
								.Text(FText::FromString(FCameraModule::s_GetPort(false)))
								.OnTextCommitted_Raw(this, &FCameraModule::OnTextChangedPort)
							]
					+ SUniformGridPanel::Slot(2, 0)
						[
							SNew(SBorder)
						]
				]
			]
		//left_buttom
				+ SVerticalBox::Slot()
					[
						SNew(SBorder)
						[
							SNew(SUniformGridPanel)
							.SlotPadding(FMargin(5.0f))
							+ SUniformGridPanel::Slot(0, 0)
								[
									SNew(SButton).HAlign(HAlign_Center).VAlign(VAlign_Center)
									.OnClicked_Raw(this, &FCameraModule::GetPercentage)
									.Text(FText::FromString("Select"))
								]
							+ SUniformGridPanel::Slot(0, 1)
								[
									SNew(SBorder)
									.BorderBackgroundColor(FSlateColor(FLinearColor::Yellow))
								]
							+ SUniformGridPanel::Slot(0, 2)
								[
									SNew(SBorder)
									.BorderBackgroundColor(FSlateColor(FLinearColor::Yellow))
								]
							+ SUniformGridPanel::Slot(0, 3)
								[
									SNew(SBorder)
									.BorderBackgroundColor(FSlateColor(FLinearColor::Yellow))
								]
							+ SUniformGridPanel::Slot(0, 4)
								[
									SNew(SBorder)
									.BorderBackgroundColor(FSlateColor(FLinearColor::Yellow))
								]
							+ SUniformGridPanel::Slot(1, 0)
							+ SUniformGridPanel::Slot(2, 0)
						]
					]
			]
		//right
		+ SHorizontalBox::Slot()
			[
				SNew(SBorder)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SImage).Image(new FSlateImageBrush(BrushName, FVector2D(128, 128)))
				]
			]
		
		];
}

void FCameraModule::OnTextChangedVInterpToSpeed(float vspped)
{
	vInterpToSpeed = vspped;
}

void FCameraModule::OnTextChangedCoordinateMul(int32 sbox)
{
	coordinateMul = sbox;
}

void FCameraModule::OnTextChangedShowDebugMessage(ECheckBoxState newState)
{
	showDebugMessage = (newState == ECheckBoxState::Checked);
}

void FCameraModule::OnTextChangedIP(const FText & InText, ETextCommit::Type)
{
	ip = InText.ToString();
}

void FCameraModule::OnTextChangedPort(const FText & InText, ETextCommit::Type)
{
	port = InText.ToString();
}

TSharedRef<SWidget> FCameraModule::MakeWidgetForOption(TSharedPtr<FString> InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
} 

void FCameraModule::OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type)
{
	CurrentItem = NewValue;
}

void FCameraModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(CameraTabName);
}

FReply FCameraModule::GetPercentage()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FCameraModule::s_GetCurrentItemLabel(true, true).ToString());
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, FString::SanitizeFloat(FCameraModule::s_GetVInterpToSpeed(true)));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Black, FString::FromInt(FCameraModule::s_GetCoordinateMul(true)));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, FCameraModule::s_GetShowDebugMessage(true) ? "true" : "false");
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, FCameraModule::s_GetPort(true));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, FCameraModule::s_GetIp(true));
	return FReply::Handled();
}

const FSlateBrush * FCameraModule::GetFirstImageBrush() const
{
	UTexture2D* tabActiveImage;
	FSlateDynamicImageBrush* tab_active;
	ConstructorHelpers::FObjectFinder<UTexture2D> activeImage(TEXT("Texture2D'Plugins/Camera/Resources/download.jpg'"));
	tabActiveImage = activeImage.Object;
	tab_active = new FSlateDynamicImageBrush(tabActiveImage, FVector2D(256, 64), FName("tabActiveImage"));
	return tab_active;
}

FText FCameraModule::s_GetCurrentItemLabel(bool canReWrite, bool isLocal) const
{
	static FText s_value;
	if(canReWrite == true)
		s_value = FText::FromString(*CurrentItem);
	else if(isLocal == true)
		return FText::FromString(*CurrentItem);
	return s_value;
}

float FCameraModule::s_GetVInterpToSpeed(bool canReWrite)
{
	static float s_value = vInterpToSpeed;
	if(canReWrite == true)
		s_value = vInterpToSpeed;
	return s_value;
}

int32 FCameraModule::s_GetCoordinateMul(bool canReWrite)
{
	static int s_value = coordinateMul;
	if (canReWrite == true)
		s_value = coordinateMul;
	return s_value;
}

bool FCameraModule::s_GetShowDebugMessage(bool canReWrite)
{
	static bool s_value = showDebugMessage;
	if (canReWrite == true)
		s_value = showDebugMessage;
	return s_value;
}

FString FCameraModule::s_GetIp(bool canReWrite)
{
	static FString s_value = ip;
	if (canReWrite == true)
		s_value = ip;
	return s_value;
}

FString FCameraModule::s_GetPort(bool canReWrite)
{
	static FString s_value = port;
	if (canReWrite == true)
		s_value = port;
	return s_value;
}

void FCameraModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FCameraCommands::Get().OpenPluginWindow);
}

void FCameraModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FCameraCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCameraModule, Camera)