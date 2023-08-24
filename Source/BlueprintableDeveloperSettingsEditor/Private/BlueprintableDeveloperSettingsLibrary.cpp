// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettingsLibrary.h"

#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetRegistryHelpers.h"

bool UBlueprintableDeveloperSettingsLibrary::FindBlueprints(const TArray<FTopLevelAssetPath>& ParentClassNames, const TSet<FTopLevelAssetPath>& ExcludedClassNames, TArray<FAssetData>& OutFoundAssets)
{
	TSet<FTopLevelAssetPath> FoundClasses;

	const auto AssetRegistry = IAssetRegistry::Get();
	AssetRegistry->GetDerivedClassNames(ParentClassNames, ExcludedClassNames, FoundClasses);

	FARFilter Filter;
	Filter.ClassPaths.Add(FTopLevelAssetPath(FName(TEXT("/Script/Engine")), FName(TEXT("BlueprintCore"))));
	Filter.bRecursiveClasses = true;

	auto FilterLambda = [&OutFoundAssets, &FoundClasses](const auto& AssetData)
	{
		if (UAssetRegistryHelpers::IsAssetDataBlueprintOfClassSet(AssetData, FoundClasses))
		{
			OutFoundAssets.Add(AssetData);
		}

		return true;
	};
	
	const auto Result = AssetRegistry->EnumerateAssets(Filter, FilterLambda);
	return Result;
}
