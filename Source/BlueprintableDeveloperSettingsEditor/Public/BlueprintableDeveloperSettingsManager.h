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
class BLUEPRINTABLEDEVELOPERSETTINGSEDITOR_API UBlueprintableDeveloperSettingsManager : public UObject, public FUObjectArray::FUObjectDeleteListener
{
	GENERATED_BODY()

protected:
	static TMap<int32, FBlueprintableSettingsSectionData> RegisteredSettings;
	
#if WITH_EDITOR
public:
	virtual void PostCDOContruct() override;
	virtual void BeginDestroy() override;
	
	static void RegisterSettings(TSubclassOf<UObject> SettingsClass);
	static void UnregisterSettings(TSubclassOf<UObject> SettingsClass);
	static void UnregisterSettings(int32 ClassId);

	UFUNCTION(BlueprintCallable, Category = "BlueprintableDeveloperSettingsManager")
	static void LoadBlueprintSettings();
	
	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool AreSettingsRegistered(TSubclassOf<UObject> SettingsClass);

	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool AreSettingsRegisteredById(int32 ClassId);

	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool AreSettingsRegisteredByData(const FBlueprintableSettingsSectionData& SectionData);

	static const FBlueprintableSettingsSectionData* FindSectionDataByClass(TSubclassOf<UObject> SettingsClass);
	static const FBlueprintableSettingsSectionData* FindSectionDataByClassId(uint32 ClassId);
	static int32 FindClassIdBySectionData(const FBlueprintableSettingsSectionData& SectionData);
	
	UFUNCTION(BlueprintPure, Category = "BlueprintableDeveloperSettingsManager")
	static bool IsAppropriateObjectForSettings(const UObject* Object);

	//~ Begin FUObjectArray::FUObjectDeleteListener Interface
	virtual void NotifyUObjectDeleted(const UObjectBase* Object, int32 Index) override;
	virtual void OnUObjectArrayShutdown() override;
	//~ End FUObjectArray::FUObjectDeleteListener Interface

private:
	static void OnFilesLoaded();
	static void OnAssetAdded(UObject* InAsset);
	static void OnSettingsDuplicated(const UBlueprintableDeveloperSettings* InSettingsObject);

#endif
	
};
