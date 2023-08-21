﻿// Copyright 2023 Gradess Games. All Rights Reserved.
#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "BlueprintCompilerExtension.h"
#include "BlueprintableDeveloperSettingsCompilerExtension.generated.h"

UCLASS()
class BLUEPRINTABLEDEVELOPERSETTINGS_API UBlueprintableDeveloperSettingsCompilerExtension : public UBlueprintCompilerExtension
{
	GENERATED_BODY()
public:
	
	
protected:
	FBlueprintableSettingsSectionData GetSectionData(UClass* InClass, UObject* InCDO);
	bool AreSettingsRegistered(UBlueprint* InBlueprint, UObject* InCDO);
	bool ValidateSettings(UBlueprint* InBlueprint, UObject* InCDO, UClass* InSettingsClass);
	
	virtual void ProcessBlueprintCompiled(const FKismetCompilerContext& CompilationContext, const FBlueprintCompiledData& Data) override;


	void OnBlueprintCompiled(UBlueprint* Blueprint);
};
#endif