// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintableDeveloperSettingsModule.h"

#include "BlueprintableDeveloperSettings.h"
#include "BlueprintableDeveloperSettingsDetails.h"

#if WITH_EDITOR
#include "BlueprintCompilationManager.h"
#include "BlueprintableDeveloperSettingsCompilerExtension.h"
#endif

#define LOCTEXT_NAMESPACE "FBlueprintableDeveloperSettingsModule"

void FBlueprintableDeveloperSettingsModule::StartupModule()
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

void FBlueprintableDeveloperSettingsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintableDeveloperSettingsModule, BlueprintableDeveloperSettingsModule)