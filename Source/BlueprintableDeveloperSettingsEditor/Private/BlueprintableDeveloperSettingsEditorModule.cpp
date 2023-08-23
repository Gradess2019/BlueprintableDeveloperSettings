// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintableDeveloperSettingsEditorModule.h"

#include "BlueprintableDeveloperSettings.h"
#include "BlueprintableDeveloperSettingsDetails.h"
#include "BlueprintCompilationManager.h"
#include "BlueprintableDeveloperSettingsCompilerExtension.h"

#define LOCTEXT_NAMESPACE "FBlueprintableDeveloperSettingsEditorModule"

void FBlueprintableDeveloperSettingsEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if WITH_EDITOR
	CompilerExtension = NewObject<UBlueprintableDeveloperSettingsCompilerExtension>();
	CompilerExtension->AddToRoot();
	
	FBlueprintCompilationManager::RegisterCompilerExtension(UBlueprint::StaticClass(), CompilerExtension);

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	
	const auto& ClassName = UBlueprintableDeveloperSettings::StaticClass()->GetFName();
	const auto& DetailLayoutDelegate = FOnGetDetailCustomizationInstance::CreateStatic(FBlueprintableDeveloperSettingsDetails::MakeInstance);
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
#endif
}

void FBlueprintableDeveloperSettingsEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintableDeveloperSettingsEditorModule, BlueprintableDeveloperSettingsEditorModule)