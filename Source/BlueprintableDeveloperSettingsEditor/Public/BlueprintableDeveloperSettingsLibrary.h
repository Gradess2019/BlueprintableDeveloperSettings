// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "BlueprintableDeveloperSettingsLibrary.generated.h"

UCLASS()
class BLUEPRINTABLEDEVELOPERSETTINGSEDITOR_API UBlueprintableDeveloperSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsLibrary")
	static bool FindBlueprints(const TArray<FTopLevelAssetPath>& ParentClassNames, const TSet<FTopLevelAssetPath>& ExcludedClassNames, TArray<FAssetData>& OutFoundAssets);
};
