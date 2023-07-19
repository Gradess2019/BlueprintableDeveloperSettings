// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BlueprintableDeveloperSettings.generated.h"

#define ENUM_CLASS_FLAGS_UINT8(Enum) \
	inline uint8& operator|=(uint8& Lhs, Enum Rhs) { return Lhs |= static_cast<uint8>(Rhs); } \
	inline uint8& operator&=(uint8& Lhs, Enum Rhs) { return Lhs &= static_cast<uint8>(Rhs); } \
	inline uint8& operator^=(uint8& Lhs, Enum Rhs) { return Lhs ^= static_cast<uint8>(Rhs); } \
	inline uint8 operator| (uint8 Lhs, Enum Rhs) { return Lhs | static_cast<uint8>(Rhs); } \
	inline uint8 operator& (uint8 Lhs, Enum Rhs) { return Lhs & static_cast<uint8>(Rhs); } \
	inline uint8 operator^ (uint8 Lhs, Enum Rhs) { return Lhs ^ static_cast<uint8>(Rhs); } \
	inline uint8 operator==(uint8 Lhs, Enum Rhs) { return Lhs == static_cast<uint8>(Rhs); } \

UENUM(BlueprintType, meta=(Bitflags))
enum class EBlueprintDeveloperSettingsFlags : uint8
{
	None = 0,
	
	Compiling = 1 << 1,
	Changing = 1 << 2,
	Renaming = 1 << 3
};
ENUM_CLASS_FLAGS(EBlueprintDeveloperSettingsFlags);
ENUM_CLASS_FLAGS_UINT8(EBlueprintDeveloperSettingsFlags);


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BLUEPRINTABLEDEVELOPERSETTINGS_API UBlueprintableDeveloperSettings : public UObject
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly, Category = "BlueprintableDeveloperSettings", meta = (Bitmask, BitmaskEnum = "/Script/UniversalWatcher.BlueprintDeveloperSettingsFlags"))
	uint8 Flags = 0;

	UPROPERTY()
	TWeakObjectPtr<UBlueprint> Blueprint;
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlueprintableDeveloperSettings")
	FName GetContainerName() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlueprintableDeveloperSettings")
	FName GetCategoryName() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlueprintableDeveloperSettings")
	FName GetSectionName() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlueprintableDeveloperSettings")
	FText GetDisplayName() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlueprintableDeveloperSettings")
	FText GetDescription() const;

#if WITH_EDITOR
	virtual void PostCDOContruct() override;

	virtual bool Modify(bool bAlwaysMarkDirty) override;
	virtual bool Rename(const TCHAR* NewName, UObject* NewOuter, ERenameFlags InFlags) override;
	virtual void PostRename(UObject* OldOuter, const FName OldName) override;

protected:
	virtual void LoadBlueprintSettings();
	
	virtual void RegisterSettings();
	virtual void UnregisterSettings();
	virtual bool IsAppropriateObjectForSettings(UObject* Object) const;

	void OnObjectsReplaced(const TMap<UObject*, UObject*>& Tuples);
	void OnFilesLoaded();
	void OnAssetAdded(const FAssetData& AssetData);
	void OnAssetRemoved(const FAssetData& AssetData);
	void OnPackageMarkedDirty(UPackage* Package, bool bArg);
	void OnBlueprintPreCompile(UBlueprint* InBlueprint);
	void OnBlueprintCompiled();
	void OnBlueprintChanged(UBlueprint* InBlueprint);
#endif
};
