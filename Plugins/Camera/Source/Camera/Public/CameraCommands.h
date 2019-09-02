// Copyright 2019 ByOwls. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CameraStyle.h"

class FCameraCommands : public TCommands<FCameraCommands>
{
public:

	FCameraCommands()
		: TCommands<FCameraCommands>(TEXT("Camera"), NSLOCTEXT("Contexts", "Camera", "Camera Plugin"), NAME_None, FCameraStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};