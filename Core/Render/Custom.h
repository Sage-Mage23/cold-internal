#include <unordered_map>
namespace Custom
{
	bool K2_Project(FVector WorldLocation, FVector2D* ScreenLocation)
	{
		FVector ReturnValue = SDK::Classes::Canvas->K2_Project(WorldLocation);

		if (ReturnValue.Z <= 0.0)
		{
			ReturnValue.X *= -1.0;
			ReturnValue.Y *= -1.0;

			ReturnValue.X += SDK::Classes::ScreenSize.X;
			ReturnValue.Y += SDK::Classes::ScreenSize.Y;
		}

		if (ReturnValue.Z > 0.0)
		{
			*ScreenLocation = FVector2D(ReturnValue.X, ReturnValue.Y);
		}

		return ScreenLocation;
	}

	FName GetNameFromString(FString InString)
	{
		static map<wchar_t*, FName> Names;
		auto it = Names.find(InString.c_str());
		if (it != Names.end())
			return it->second;

		FName OutName = StaticClasses::KismetStringLibrary->Conv_StringToName(InString);
		Names.insert(InString.c_str(), OutName);

		return OutName;
	}

	FVector GetSocketLocation(USkeletalMeshComponent* Mesh, FString InSocketName)
	{
		FName SocketName = Custom::GetNameFromString(InSocketName);

		return Mesh->GetSocketLocation(SocketName);
	}

	double InterpolatedValue(double CurrentScalar, double MaxScalar, double MinValue, double MaxValue)
	{
		MaxScalar = StaticClasses::KismetMathLibrary->FMin(MaxScalar, CurrentScalar);

		double InterpolatedValue = MaxValue - (MaxValue - MinValue) * (MaxScalar / CurrentScalar);

		InterpolatedValue = StaticClasses::KismetMathLibrary->Clamp(InterpolatedValue, MinValue, MaxValue);

		return InterpolatedValue;
	}

	bool InScreen(FVector2D ScreenLocation)
	{
		if (ScreenLocation.X < 5.0 || ScreenLocation.X > SDK::Classes::ScreenSize.X - (5.0 * 2) && ScreenLocation.Y < 5.0 || ScreenLocation.Y > SDK::Classes::ScreenSize.Y - (5.0 * 2))
			return false;
		return true;
	}

	bool InRect(double Radius, FVector2D ScreenLocation)
	{
		return SDK::Classes::ScreenCenter.X >= (SDK::Classes::ScreenCenter.X - Radius) && SDK::Classes::ScreenCenter.X <= (SDK::Classes::ScreenCenter.X + Radius) &&
			ScreenLocation.Y >= (ScreenLocation.Y - Radius) && ScreenLocation.Y <= (ScreenLocation.Y + Radius);
	}

	bool InCircle(double Radius, FVector2D ScreenLocation)
	{
		if (InRect(Radius, ScreenLocation))
		{
			double dx = SDK::Classes::ScreenCenter.X - ScreenLocation.X; dx *= dx;
			double dy = SDK::Classes::ScreenCenter.Y - ScreenLocation.Y; dy *= dy;
			return dx + dy <= Radius * Radius;
		} return false;
	}

	bool IsHitscanBone(int AimBone) { return AimBone == 5; };


	FString GetNameByPlatform(FString Platform)
	{
		if (Platform == FString(L"WIN"))
			return FString(L"Windows");
		else if (Platform == FString(L"XBL"))
			return FString(L"Xbox One");
		else if (Platform == FString(L"XSX"))
			return FString(L"Xbox Series X/S");
		else if (Platform == FString(L"PSN") || Platform == FString(L"PS5"))
			return FString(L"PlayStation");
		else if (Platform == FString(L"SWT"))
			return FString(L"Nintendo Switch");
		else if (Platform == FString(L"AND"))
			return FString(L"Android");
		else if (Platform == FString(L"MAC"))
			return FString(L"MacOS");
		else if (Platform == FString(L"LNX"))
			return FString(L"Linux");
		else if (Platform == FString(L"IOS"))
			return FString(L"Apple");
		return FString(L"BOT/AI");
	}
	FString GetRankByProgress(int32_t Rank)
	{
		switch (Rank)
		{
		case 0:
		case 1:
		case 2:
			return FString(L"T-UI-Icon-Habenaro-Bronze");
		case 3:
		case 4:
		case 5:
			return FString(L"T-UI-Icon-Habenaro-Silver");
		case 6:
		case 7:
		case 8:
			return FString(L"T-UI-Icon-Habenaro-Gold");
		case 9:
		case 10:
		case 11:
			return FString(L"T-UI-Icon-Habenaro-Platinum");
		case 12:
		case 13:
		case 14:
			return FString(L"T-UI-Icon-Habenaro-Diamond");
		case 15:
			return FString(L"T-UI-Icon-Habenaro-Elite");
		case 16:
			return FString(L"T-UI-Icon-Habenaro-Champion");
		case 17:
			return FString(L"T-UI-Icon-Habenaro-Unreal");
		default:
			return FString(L"None");
		}
	}

	FLinearColor GetColorByTier(EFortRarity Tier)
	{
		switch (Tier)
		{
		case EFortRarity::Common:
			return FLinearColor(1.f, 1.f, 1.f, 1.f); // Lighter gray
		case EFortRarity::Uncommon:
			return FLinearColor(0.4f, 1.f, 0.4f, 1.f); // Brighter green
		case EFortRarity::Rare:
			return FLinearColor(0.4f, 0.4f, 1.f, 1.f); // Brighter blue
		case EFortRarity::Epic:
			return FLinearColor(1.f, 0.4f, 1.f, 1.f); // Brighter pink
		case EFortRarity::Legendary:
			return FLinearColor(1.f, 1.f, 0.4f, 1.f); // Brighter yellow
		case EFortRarity::Mythic:
			return FLinearColor(1.f, 1.f, 0.4f, 1.f); // Bright golden yellow (same as legendary)
		default:
			return FLinearColor(1.f, 1.f, 1.f, 1.f); // Lighter gray
		}
	}

	FString GetBuildByType(EFortBuildingType Build)
	{
		switch (Build)
		{
		case EFortBuildingType::Wall:
			return FString(L"Wall");
		case EFortBuildingType::Floor:
			return FString(L"Floor");
		case EFortBuildingType::Corner:
			return FString(L"Corner");
		case EFortBuildingType::Deco:
			return FString(L"Deco");
		case EFortBuildingType::Prop:
			return FString(L"Prop");
		case EFortBuildingType::Stairs:
			return FString(L"Stairs");
		case EFortBuildingType::Roof:
			return FString(L"Roof");
		case EFortBuildingType::Pillar:
			return FString(L"Pillar");
		case EFortBuildingType::SpawnedItem:
			return FString(L"Spawned Item");
		case EFortBuildingType::Container:
			return FString(L"Container");
		case EFortBuildingType::Trap:
			return FString(L"Trap");
		case EFortBuildingType::GenericCenterCellActor:
			return FString(L"Generic Center Cell Actor");
		case EFortBuildingType::StructuralCorner:
			return FString(L"Structural Corner");
		case EFortBuildingType::None:
		default:
			return FString(L"None");
		}
	}
}