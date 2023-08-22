// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintableDeveloperSettingsTypes.h"

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

	friend class UBlueprintableDeveloperSettingsCompilerExtension;
	friend class FBlueprintableDeveloperSettingsDetails;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetSectionData", Category = "BlueprintableDeveloperSettings", meta = (ShowOnlyInnerProperties))
	FBlueprintableSettingsSectionData SectionData;

public:
	UBlueprintableDeveloperSettings();
	
	UFUNCTION(BlueprintGetter, Category = "BlueprintableDeveloperSettings")
	const FBlueprintableSettingsSectionData& GetSectionData() const { return SectionData; };

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostDuplicate(bool bDuplicateForPIE) override;

protected:
	virtual void RegisterSettings();
	virtual void UnregisterSettings();
#endif
};
