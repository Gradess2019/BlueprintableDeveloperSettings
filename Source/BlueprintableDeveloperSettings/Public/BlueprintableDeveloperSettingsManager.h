// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BlueprintableDeveloperSettingsTypes.h"
#include "BlueprintableDeveloperSettingsManager.generated.h"

class UBlueprintableDeveloperSettings;
class ISettingsSection;

/**
 * 
 */
UCLASS()
class BLUEPRINTABLEDEVELOPERSETTINGS_API UBlueprintableDeveloperSettingsManager : public UObject
{
	GENERATED_BODY()

protected:
	static TMap<TSubclassOf<UObject>, FBlueprintableSettingsSectionData> RegisteredSettings;
	
#if WITH_EDITOR
public:
	virtual void PostCDOContruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "BlueprintableDeveloperSettingsManager")
	static void RegisterSettings(TSubclassOf<UObject> SettingsClass);

	UFUNCTION(BlueprintCallable, Category = "BlueprintableDeveloperSettingsManager")
	static void UnregisterSettings(TSubclassOf<UObject> SettingsClass);

	UFUNCTION(BlueprintCallable, Category = "BlueprintableDeveloperSettingsManager")
	static void LoadBlueprintSettings();
	
	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool IsSettingsRegistered(TSubclassOf<UObject> SettingsClass);

	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool IsAppropriateObjectForSettings(const UObject* Object);

private:
	static void OnFilesLoaded();
	static void OnAssetAdded(const FAssetData& AssetData);



#endif
	
};
