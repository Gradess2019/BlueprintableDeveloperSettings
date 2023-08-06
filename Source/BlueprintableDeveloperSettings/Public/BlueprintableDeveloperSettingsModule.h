// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "BlueprintCompilerExtension.h"
#endif

class FBlueprintableDeveloperSettingsModule : public IModuleInterface
{
protected:
#if WITH_EDITOR
	TObjectPtr<UBlueprintCompilerExtension> CompilerExtension = nullptr;
#endif
	
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
