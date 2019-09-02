// Copyright 2019 ByOwls. All Rights Reserved.

#include "CameraCommands.h"

#define LOCTEXT_NAMESPACE "FCameraModule"

void FCameraCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Camera", "Bring up Camera window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
