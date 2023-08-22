// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettingsDetails.h"

#include "BlueprintableDeveloperSettings.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"

#include "Algo/ForEach.h"

TSharedRef<IDetailCustomization> FBlueprintableDeveloperSettingsDetails::MakeInstance()
{
	return MakeShared<FBlueprintableDeveloperSettingsDetails>();
}

void FBlueprintableDeveloperSettingsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	const auto* DetailsView = DetailBuilder.GetDetailsView();

	if (!ensure(DetailsView))
	{
		return;
	}

	if (IsBlueprintInspector(DetailBuilder))
	{
		return;
	}

	HideSectionData(DetailBuilder);
}

bool FBlueprintableDeveloperSettingsDetails::IsBlueprintInspector(IDetailLayoutBuilder& DetailBuilder)
{
	static FName BlueprintIdentifier(TEXT("BlueprintInspector"));
	const auto DetailsViewIdentifier = DetailBuilder.GetDetailsView()->GetIdentifier();

	if (DetailsViewIdentifier == BlueprintIdentifier)
	{
		return true;
	}
	return false;
}

void FBlueprintableDeveloperSettingsDetails::HideSectionData(IDetailLayoutBuilder& DetailBuilder)
{
	const auto Property = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBlueprintableDeveloperSettings, SectionData));

	uint32 ChildrenCount;
	Property->GetNumChildren(ChildrenCount);

	for (uint32 Id = 0; Id < ChildrenCount; ++Id)
	{
		auto Child = Property->GetChildHandle(Id);
		DetailBuilder.HideProperty(Child);
	}
}
