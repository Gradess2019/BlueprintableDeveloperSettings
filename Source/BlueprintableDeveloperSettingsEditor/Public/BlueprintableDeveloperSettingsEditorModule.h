// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Modules/ModuleManager.h>
#include <BlueprintCompilerExtension.h>

class FBlueprintableDeveloperSettingsEditorModule : public IModuleInterface
{
protected:
	TObjectPtr<UBlueprintCompilerExtension> CompilerExtension = nullptr;

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
