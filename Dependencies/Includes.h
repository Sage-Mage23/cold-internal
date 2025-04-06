#pragma once
#include <Windows.h>
#include <iostream>
#include <ntstatus.h>
#include <stdio.h>
#include <cstdint>
#include <wchar.h>
#include <vector>
#include <stdint.h>

#include "Memory/XOR/Xor.h"
#include "Memory/XOR/XorData.h"
#include "Memory/XOR/XorValue.h"

#include "Importer/Encrypt.h"

#include "Memory/CRT/CRT.h"
#include "Memory/Hook/Hook.h"

uintptr_t Module = 0;

#include "../Core/SDK/Vector.h"
#include "../Core/SDK/Engine.h"
#include "../Core/SDK/Offsets.h"
#include "../Core/SDK/Classes.h"

namespace SDK
{
	namespace Classes
	{
		UCanvas* Canvas = nullptr;
		APlayerController* PlayerController = nullptr;
		USkeletalMeshComponent* Mesh = nullptr;
		UWorld* World = nullptr;
		APlayerPawn_Athena_C* AcknowledgedPawn = nullptr;
		UGameViewportClient* ViewportClient = nullptr;
		APlayerPawn_Athena_C* TargetPlayerClosestToCenter = nullptr;

		UFont* Burbank = nullptr;
		UFont* MediumFont = nullptr;

		static FVector2D ScreenCenter = FVector2D();
		static FVector2D ScreenSize = FVector2D();
		static FVector2D ScreenTopLeft = FVector2D();
		static FVector CameraLocation = FVector();
		static FRotator CameraRotation = FRotator();
		static float FieldOfView = 0.f;

		namespace GetViewPoint
		{
			static FVector Location = FVector();
			static FRotator Rotation = FRotator();
			static bool bApplyModifiedData = false;
			static bool bSkipRotationForModification = false;
		}

		UObject* PlayerPawn = nullptr;
		UObject* FortPickup = nullptr;
		UObject* BuildingTrap = nullptr;
		UObject* BuildingContainer = nullptr;
		UObject* BuildingWeakSpot = nullptr;
		UObject* AthenaSuperDingo = nullptr;
		UObject* FortAthenaVehicle = nullptr;
		UObject* AthenaSupplyDrop = nullptr;
		UObject* FortWeaponRanged = nullptr;
		UObject* FortWeapon = nullptr;
		UObject* WeaponPickaxeAthena = nullptr;
		UObject* BaseWeaponStats = nullptr;
		UObject* LocalPlayer = nullptr;
		UObject* PlayerControllerMain = nullptr;
		UObject* GameViewportClient = nullptr;
		UObject* PlayerCameraManager = nullptr;
		UObject* ActorPlayerNamePrivate = nullptr;

		namespace GetControlRotation
		{
			static bool bAimbotActive = false;
			static FRotator AimbotRotationTarget = FRotator();
		}
	}

	FKey Insert = FKey();
	FKey LeftMouseButton = FKey();
	FKey RightMouseButton = FKey();
	FKey ThumbMouseButton = FKey();
	FKey ThumbMouseButton2 = FKey();
	FKey F8 = FKey();
	FKey LeftShift = FKey();
	FKey LeftAlt = FKey();

}

#include "../Core/Render/settings.h"
#include "../Core/Render/Framework/ZeroGui.h"
#include "../Core/Render/custom.h"
#include "../Core/Render/render.h"