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
	static TMap<uint32, FBlueprintableSettingsSectionData> RegisteredSettings;
	static TSet<uint32> RegisteredSettingsHashes;
	
#if WITH_EDITOR
public:
	virtual void PostCDOContruct() override;
	
	static void RegisterSettings(TSubclassOf<UObject> SettingsClass);
	static void UnregisterSettings(TSubclassOf<UObject> SettingsClass);

	UFUNCTION(BlueprintCallable, Category = "BlueprintableDeveloperSettingsManager")
	static void LoadBlueprintSettings();
	
	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool AreSettingsRegistered(TSubclassOf<UObject> SettingsClass);

	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool AreSettingsRegisteredByData(const FBlueprintableSettingsSectionData& SectionData);

	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool IsAppropriateObjectForSettings(const UObject* Object);

private:
	static void OnFilesLoaded();
	static void OnAssetAdded(const FAssetData& AssetData);



#endif
	
};
