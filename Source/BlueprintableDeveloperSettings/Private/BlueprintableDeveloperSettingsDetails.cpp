// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettingsDetails.h"

#include "BlueprintableDeveloperSettings.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "IPropertyUtilities.h"
#include "ISettingsModule.h"

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

	static FName BlueprintIdentifier(TEXT("BlueprintInspector"));
	const auto DetailsViewIdentifier = DetailBuilder.GetDetailsView()->GetIdentifier();

	if (DetailsViewIdentifier == BlueprintIdentifier)
	{
		return;
	}
	
	const auto Property = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBlueprintableDeveloperSettings, SectionData));

	uint32 ChildrenCount;
	Property->GetNumChildren(ChildrenCount);

	for (uint32 Id = 0; Id < ChildrenCount; ++Id)
	{
		auto Child = Property->GetChildHandle(Id);
		DetailBuilder.HideProperty(Child);
	}
}