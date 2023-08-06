// Copyright 2023 Gradess Games. All Rights Reserved.

#if WITH_EDITOR
#include "BlueprintableDeveloperSettingsCompilerExtension.h"

#include "BlueprintableDeveloperSettings.h"
#include "BlueprintableDeveloperSettingsManager.h"

#include "Kismet2/BlueprintEditorUtils.h"

void UBlueprintableDeveloperSettingsCompilerExtension::ProcessBlueprintCompiled(const FKismetCompilerContext& CompilationContext, const FBlueprintCompiledData& Data)
{
	const auto bValidBlueprint = IsValid(CompilationContext.NewClass) && CompilationContext.NewClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass());
	if (!bValidBlueprint)
	{
		return;
	}

	const auto* Property = FindFProperty<FStructProperty>(CompilationContext.OldClass, GET_MEMBER_NAME_CHECKED(UBlueprintableDeveloperSettings, SectionData));
	const auto& SectionData = *Property->ContainerPtrToValuePtr<FBlueprintableSettingsSectionData>(CompilationContext.OldClass->ClassDefaultObject);

	const auto bSettingsAlreadyRegistered = UBlueprintableDeveloperSettingsManager::AreSettingsRegisteredByData(SectionData);

	if (bSettingsAlreadyRegistered)
	{
		const auto RegisteredClassId = UBlueprintableDeveloperSettingsManager::GetRegisteredClassId(SectionData);
		const auto CompiledClassId = CompilationContext.NewClass->GetUniqueID();
		
		if (RegisteredClassId != CompiledClassId)
		{
			CompilationContext.MessageLog.Error(TEXT("Couldn't register settings for @@. Settings with such Container, Category and Section names are already registered!"), *CompilationContext.Blueprint->GetName());
		}
	}
}
#endif