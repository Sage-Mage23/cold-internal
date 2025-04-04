using O_DrawTransistion = void(*)(UGameViewportClient* Viewport, UCanvas* Canvas);
O_DrawTransistion OriginalTransition = 0;

const pair<wchar_t*, wchar_t*> BonePairs[]
{ {L"head", L"neck_01"},
	{L"neck_01", L"upperarm_r"},
	{L"neck_01", L"upperarm_l"},
	{L"upperarm_l", L"lowerarm_l"},
	{L"lowerarm_l", L"hand_l"},
	{L"upperarm_r", L"lowerarm_r"},
	{L"lowerarm_r", L"hand_r"},
	{L"neck_01", L"pelvis"},
	{L"pelvis", L"thigh_r"},
	{L"pelvis", L"thigh_l"},
	{L"thigh_r", L"calf_r"},
	{L"calf_r", L"ik_foot_r"},
	{L"thigh_l", L"calf_l"},
	{L"calf_l", L"ik_foot_l"}
};

vector<pair<wchar_t*, wchar_t*>> SkeletonBonePairs(BonePairs, sizeof(BonePairs) / sizeof(pair<wchar_t*, wchar_t*>));

void RenderingMain(UGameViewportClient* Viewport, UCanvas* Canvas)
{
	Canvas->K2_DrawText(L"Cold Private - Build: April 4 2025", { 5, 5 }, 13.f, { 1.0f, 1.0f, 1.0f, 1.0f }, false, false, true);

	SDK::Classes::Canvas = Canvas;
	SDK::Classes::ScreenSize = FVector2D(double(Canvas->ClipX()), double(Canvas->ClipY()));
	SDK::Classes::ScreenCenter = FVector2D(SDK::Classes::ScreenSize.X / 2.0, SDK::Classes::ScreenSize.Y / 2.0);
	SDK::Classes::ScreenTopLeft = FVector2D(SDK::Classes::ScreenSize.X - 40.0, SDK::Classes::ScreenSize.Y - 40.0);

	UWorld* World = reinterpret_cast<UWorld*>(Viewport->World());
	if (!World) return OriginalTransition(Viewport, Canvas);

	double WorldDeltaSeconds = StaticClasses::GameplayStatics->GetWorldDeltaSeconds(World);

	UGameInstance* Gameinstance = World->OwningGameInstance();
	if (!Gameinstance) return OriginalTransition(Viewport, Canvas);

	ULocalPlayer* LocalPlayer = Gameinstance->LocalPlayers()[0];
	if (!LocalPlayer) return OriginalTransition(Viewport, Canvas);

	UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient();
	if (!ViewportClient) return OriginalTransition(Viewport, Canvas);

	APlayerController* PlayerController = LocalPlayer->PlayerController();
	if (!PlayerController) return OriginalTransition(Viewport, Canvas);

	SDK::Classes::PlayerController = PlayerController;

	APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager();
	if (!PlayerCameraManager) return OriginalTransition(Viewport, Canvas);
	
	if (GetAsyncKeyState(VK_INSERT)) Render::ShowMenu = !Render::ShowMenu;
	Menu::Render(Canvas);

	APlayerPawn_Athena_C* AcknowledgedPawn = PlayerController->AcknowledgedPawn();

	SDK::Classes::CameraLocation = PlayerCameraManager->GetCameraLocation();
	SDK::Classes::CameraRotation = PlayerCameraManager->GetCameraRotation();

	USceneViewState* ViewState = LocalPlayer->GetViewState();
	float FieldOfView = StaticClasses::KismetMathLibrary->atan(1.0 / *(double*)(ViewState + 0x740)) * 2.f;
	SDK::Classes::FieldOfView = FieldOfView * (180.f / M_PI);

	double ClosestDistanceToMyself = DBL_MAX;
	double ClosestDistanceToCenter = DBL_MAX;
	APlayerPawn_Athena_C* TargetPlayerClosestToMyself = nullptr;
	APlayerPawn_Athena_C* TargetPlayerClosestToCenter = nullptr;

	bool IsVisible;

	double AimbotFOV = (Settings::Combat::AimFOV * SDK::Classes::ScreenSize.X / SDK::Classes::FieldOfView) / 2.0;

	FVector Head = FVector();

	TArray <APlayerPawn_Athena_C*> PlayerArray;
	StaticClasses::GameplayStatics->GetAllActorsOfClass(World, SDK::Classes::PlayerPawn, &PlayerArray);

	for (int i = 0; i < PlayerArray.NumElements; i++)
	{
		if (!PlayerArray.EValid(i)) 
			continue;

		auto Player = PlayerArray[i];
		if (!Player)
			continue;

		if (Player == AcknowledgedPawn)
			continue;

		bool bisDBNO = Player->IsDBNO();

		bool bisDead = Player->IsDead();
		if (bisDead) continue;

		bool Teammate = StaticClasses::FortKismetLibrary->OnSameTeam(Player, AcknowledgedPawn);

		if (Teammate) continue;

		USkeletalMeshComponent* Mesh = Player->Mesh();
		if (!Mesh) continue;

		APlayerState* PlayerState = Player->PlayerState();
		if (!PlayerState) continue;

		bool IsABot = PlayerState->IsABot();

		SDK::Classes::CameraLocation = PlayerCameraManager->GetCameraLocation();

		SDK::Classes::CameraRotation = PlayerCameraManager->GetCameraRotation();

		bool Visible = Player->WasRecentlyRendered(0.01f);

		FLinearColor Color = Settings::Misc::VisibleColor;

		if (Visible || !AcknowledgedPawn)
		{
			Color = Settings::Misc::VisibleColor;
		}
		else
		{
			Color = Settings::Misc::NotVisibleColor;
		}

		if (bisDBNO)
			Color = Settings::Misc::KnockedColor;

		FVector Root = Custom::GetSocketLocation(Mesh, FString(L"root"));
		double Distance = StaticClasses::KismetMathLibrary->Vector_Distance(SDK::Classes::CameraLocation, Root) * 0.01;
		double BoxSize = Custom::InterpolatedValue(Distance, 100.0, 1.0, 3.0);
		double FontSize = Custom::InterpolatedValue(150.0, Distance, 8.0, Settings::Visuals::FontSize);

		FVector Head = Custom::GetSocketLocation(Mesh, FString(L"head"));
		FVector2D HeadScreen = FVector2D();
		if (!Custom::K2_Project(Head, &HeadScreen)) continue;

		bool AimbotKnocked = (Settings::Misc::SkipKnocked && !bisDBNO) || !Settings::Misc::SkipKnocked;
		bool AimbotBot = (Settings::Misc::SkipBot && !IsABot) || !Settings::Misc::SkipBot;
		bool AimbotValid = (AimbotKnocked && AimbotBot && !Teammate) && Distance < Settings::Visuals::MaxDistance;
		if (AimbotValid)
		{
			if (Custom::InCircle(AimbotFOV, HeadScreen))
			{
				if (Distance < ClosestDistanceToMyself)
				{
					ClosestDistanceToMyself = Distance;
					TargetPlayerClosestToMyself = Player;
					IsVisible = Visible;
				}

				double DistanceToCenter = StaticClasses::KismetMathLibrary->Vector_Distance2D(SDK::Classes::ScreenCenter, HeadScreen);
				if (DistanceToCenter < ClosestDistanceToCenter)
				{
					ClosestDistanceToCenter = DistanceToCenter;
					TargetPlayerClosestToCenter = Player;
				}
			}
		}

		if (!Custom::InScreen(HeadScreen)) continue;
		
		if (Settings::Visuals::Enable)
		{
			FVector Origin = FVector();
			FVector Extent = FVector();
			float SphereRadius = 0.f;

			StaticClasses::KismetSystemLibrary->GetComponentBounds(Mesh, &Origin, &Extent, &SphereRadius);

			double MostLeft = DBL_MAX;
			double MostRight = DBL_MIN;
			double MostTop = DBL_MAX;
			double MostBottom = DBL_MIN;

			for (int i = 0; i < 8; i++)
			{
				FVector Corner;

				if (i & 1)
				{
					Corner.X = 1.0 * Extent.X;
				}
				else
				{
					Corner.X = -1.0 * Extent.X;
				}

				if (i & 2)
				{
					Corner.Y = 1.0 * Extent.Y;
				}
				else
				{
					Corner.Y = -1.0 * Extent.Y;
				}

				if (i & 2)
				{
					Corner.Z = 1.0 * Extent.Z;
				}
				else
				{
					Corner.Z = -1.0 * Extent.Z;
				}

				FVector2D Origin2D = FVector2D();
				if (!Custom::K2_Project(Origin + Corner, &Origin2D)) continue;

				if (Origin2D.X < MostLeft)
				{
					MostLeft = Origin2D.X;
				}

				if (Origin2D.X > MostRight)
				{
					MostRight = Origin2D.X;
				}

				if (Origin2D.Y < MostTop)
				{
					MostTop = Origin2D.Y;
				}

				if (Origin2D.Y > MostBottom)
				{
					MostBottom = Origin2D.Y;
				}
			}

			double ActorHeight = MostBottom - MostTop;
			double ActorWidth = MostRight - MostLeft;

			double CornerWidth = ActorWidth / 4.0;
			double CornerHeight = ActorHeight / 7.0;

			double TopTextOffset = 17.0;
			double BottomTextOffset = FontSize;

			FVector2D BottomMiddle = FVector2D(MostLeft + (ActorWidth / 2.0), MostBottom);

			if (Settings::Visuals::Box)
			{
				Canvas->K2_DrawLine(FVector2D(MostLeft, MostTop), FVector2D(MostLeft, MostTop + (CornerHeight * BoxSize)), 1.5f, Color);
				Canvas->K2_DrawLine(FVector2D(MostLeft, MostTop), FVector2D(MostLeft + (CornerWidth + BoxSize), MostTop), 1.5f, Color);

				Canvas->K2_DrawLine(FVector2D(MostLeft + ActorWidth - (CornerWidth + BoxSize), MostTop), FVector2D(MostLeft + ActorWidth, MostTop), 1.5f, Color);
				Canvas->K2_DrawLine(FVector2D(MostLeft + ActorWidth, MostTop), FVector2D(MostLeft + ActorWidth, MostTop + (CornerHeight * BoxSize)), 1.5f, Color);

				Canvas->K2_DrawLine(FVector2D(MostLeft, MostTop + ActorHeight - (CornerHeight * BoxSize)), FVector2D(MostLeft, MostTop + ActorHeight), 1.5f, Color);
				Canvas->K2_DrawLine(FVector2D(MostLeft, MostTop + ActorHeight), FVector2D(MostLeft + (CornerWidth + BoxSize), MostTop + ActorHeight), 1.5f, Color);

				Canvas->K2_DrawLine(FVector2D(MostLeft + ActorWidth - (CornerWidth + BoxSize), MostTop + ActorHeight), FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight), 1.5f, Color);
				Canvas->K2_DrawLine(FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight - (CornerHeight * BoxSize)), FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight), 1.5f, Color);
			}

			if (Settings::Visuals::Skeleton)
			{
				for (const auto& Pair : SkeletonBonePairs)
				{
					FVector WorldLocationFirst = Custom::GetSocketLocation(Mesh, Pair.first);
					FVector WorldLocationSecond = Custom::GetSocketLocation(Mesh, Pair.second);

					FVector2D ScreenPositionFirst = FVector2D();
					FVector2D ScreenPositionSecond = FVector2D();

					if (!Custom::K2_Project(WorldLocationFirst, &ScreenPositionFirst)) continue;
					if (!Custom::K2_Project(WorldLocationSecond, &ScreenPositionSecond)) continue;

					Canvas->K2_DrawLine(ScreenPositionFirst, ScreenPositionSecond, 1.5f, Color);
				}
			}

			if (Settings::Visuals::Name)
			{
				FString PlayerName = Player->GetPlayerNameSafe(Player, SDK::Classes::ActorPlayerNamePrivate);

				if (PlayerName.c_str())
				{
					Canvas->K2_DrawText(PlayerName, FVector2D(BottomMiddle.X, MostTop - TopTextOffset), Settings::Visuals::FontSize, Color, true, false, Settings::Visuals::TextOutline);
					TopTextOffset += 14.0;
				}
			}

			if (Settings::Visuals::Platform)
			{
				FString Platform = PlayerState->GetPlatformSDK();
				FLinearColor RenderColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

				Canvas->K2_DrawText(Platform, FVector2D(BottomMiddle.X, MostTop - TopTextOffset), Settings::Visuals::FontSize, RenderColor, true, false, Settings::Visuals::TextOutline);
				TopTextOffset += FontSize + 2;
			}

			if (Settings::Visuals::Rank)
			{
				if (PlayerState->HabaneroComponent())
				{
					Canvas->K2_DrawText(PlayerState->GetRankStr(), FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), Settings::Visuals::FontSize, Color, true, false, Settings::Visuals::TextOutline);
					BottomTextOffset += FontSize + 2;
				}
			}

			if (Settings::Visuals::Weapon)
			{
				if (AcknowledgedPawn)
				{
					AFortWeapon* CurrentWeapon = Player->CurrentWeapon();

					if (CurrentWeapon)
					{
						UFortWeaponItemDefinition* ItemDefinition = CurrentWeapon->GetWeaponData();

						auto DisplayName = ItemDefinition->DisplayName();

					    auto WeaponName = StaticClasses::KismetStringLibrary->Conv_TextToString(DisplayName);

						if (ItemDefinition)
						{
							Canvas->K2_DrawText(WeaponName, FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset + 10), Settings::Visuals::FontSize, Color, true, false, Settings::Visuals::TextOutline);
							BottomTextOffset += FontSize + 2;
						}
					}
				}
			}

			if (Settings::Visuals::Distance)
			{
				FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(L"", L"", int(Distance), L"m");
				Canvas->K2_DrawText(ConvertedText, FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), Settings::Visuals::FontSize, Color, true, false, Settings::Visuals::TextOutline);
				TopTextOffset += FontSize + 2;
			}
		}
	}

	if (Settings::Combat::Enable)
	{
		if (Settings::Combat::DrawFOV)
		{
			FLinearColor FOV = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
			Canvas->DrawCircle(SDK::Classes::ScreenCenter, AimbotFOV, 200, FOV);
		}

		if (AcknowledgedPawn)
		{

			AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon();
			if (CurrentWeapon)
			{

				bool IsHoldingWeapon = StaticClasses::GameplayStatics->ObjectIsA(CurrentWeapon, SDK::Classes::FortWeaponRanged);
				if (IsHoldingWeapon)
				{
					bool IsHoldingPickaxe = StaticClasses::GameplayStatics->ObjectIsA(CurrentWeapon, SDK::Classes::WeaponPickaxeAthena);

					if (!IsHoldingPickaxe)
					{
						FVector StartLocation = CurrentWeapon->GetTargetingSourceLocation();

						APlayerPawn_Athena_C* Target = nullptr;

						Target = TargetPlayerClosestToCenter;

						if (Target)
						{
							USkeletalMeshComponent* Mesh = Target->Mesh();
							if (Mesh)
							{
								FString TargetBoneName = FString(L"head");

								FVector2D ScreenLocation = FVector2D();
								FVector AimbotPosition = Custom::GetSocketLocation(Mesh, TargetBoneName);

								if (Custom::K2_Project(AimbotPosition, &ScreenLocation))
								{
									double PlayerDistance = StaticClasses::KismetMathLibrary->Vector_Distance(StartLocation, AimbotPosition);
									IsVisible = ((Settings::Misc::VisibleCheck && IsVisible) || (!Settings::Misc::VisibleCheck));

									if ((Settings::Misc::VisibleCheck && IsVisible || !Settings::Misc::VisibleCheck) && AcknowledgedPawn)
									{
										if (Settings::Combat::MuzzleLine)
										{
											FVector2D MuzzleScreen = FVector2D();
											FVector Muzzle = CurrentWeapon->GetMuzzleLocation();
											if (Custom::K2_Project(Muzzle, &MuzzleScreen))
											{
												Canvas->K2_DrawLine(MuzzleScreen, ScreenLocation, 1.5f, { 1.0f,1.0f,1.0f, 1.0f });
											}
										}

										if (Settings::Combat::Prediction)
										{
											AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon();

											if (CurrentWeapon)
											{
												bool IsHoldingWeapon = StaticClasses::GameplayStatics->ObjectIsA(CurrentWeapon, SDK::Classes::FortWeaponRanged);

												UFortWeaponItemDefinition* ItemDefinition = CurrentWeapon->GetWeaponData();

												bool IsLeightweight = ItemDefinition->HasLightweightProjectile();

												if (IsHoldingWeapon && CurrentWeapon)
												{
													if (IsLeightweight)
													{
														if (AcknowledgedPawn->CurrentWeapon()->GetProjectileGravity() && AcknowledgedPawn->CurrentWeapon()->GetProjectileSpeed())
														{
															FVector Velocity = Target->GetVelocity();
															double ProjectileGravity = CurrentWeapon->GetProjectileGravity();
															FVector startlocation = CurrentWeapon->GetTargetingSourceLocation();
															double distance = StaticClasses::KismetMathLibrary->Vector_Distance(AimbotPosition, startlocation);

															double TimeToTarget = distance / CurrentWeapon->GetProjectileSpeed();

															AimbotPosition.X += Velocity.X * TimeToTarget;
															AimbotPosition.Y += Velocity.Y * TimeToTarget;

															double Difference = startlocation.Z - AimbotPosition.Z;
															double HorizontalDistance = StaticClasses::KismetMathLibrary->sqrt(StaticClasses::KismetMathLibrary->Pow(startlocation.X - AimbotPosition.X, 2.0) + StaticClasses::KismetMathLibrary->Pow(startlocation.Y - AimbotPosition.Y, 2.0));
															double VerticalAngle = StaticClasses::KismetMathLibrary->Atan2(StaticClasses::KismetMathLibrary->abs(Difference), HorizontalDistance);

															double Angle = VerticalAngle * (180.0 / M_PI);
															double Factor = StaticClasses::KismetMathLibrary->abs(Angle - 90.0) / 90.0;
															ProjectileGravity *= Factor;

															AimbotPosition.Z += Velocity.Z * TimeToTarget + (StaticClasses::KismetMathLibrary->abs(ProjectileGravity * -980.0) * (TimeToTarget * TimeToTarget)) * 0.5;
														}
													}
												}
											}
										}

										if (Settings::Combat::Enable && AcknowledgedPawn)
										{
											if (StaticClasses::KismetMathLibrary)
											{
												if (SDK::Classes::CameraLocation && SDK::Classes::CameraRotation && AimbotPosition)
												{
													FRotator TargetRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation(SDK::Classes::CameraLocation, AimbotPosition);

													TargetRotation = TargetRotation.Normalize();

													if (TargetRotation)
													{

														if (Settings::Combat::Smooth >= 0.0)
														{
															TargetRotation = StaticClasses::KismetMathLibrary->RInterpTo(SDK::Classes::CameraRotation, TargetRotation, WorldDeltaSeconds, (30.0 - Settings::Combat::Smooth));
															TargetRotation += (TargetRotation - SDK::Classes::CameraRotation);
														}

														TargetRotation = TargetRotation.Normalize();

														if (PlayerController)
														{
															if (GetAsyncKeyState(Settings::Combat::Keybind))
															{
																TargetRotation.Pitch = TargetRotation.Pitch - SDK::Classes::CameraRotation.Pitch;
																TargetRotation.Yaw = TargetRotation.Yaw - SDK::Classes::CameraRotation.Yaw;

																while (TargetRotation.Yaw > 180.0f) TargetRotation.Yaw -= 360.0f;
																while (TargetRotation.Yaw < -180.0f) TargetRotation.Yaw += 360.0f;

																if (StaticClasses::KismetMathLibrary->abs(TargetRotation.Pitch) > 0.68f)
																{
																	float ScaleFactorPitch = 0.68f / StaticClasses::KismetMathLibrary->abs(TargetRotation.Pitch);
																	TargetRotation.Pitch *= ScaleFactorPitch;
																}

																if (StaticClasses::KismetMathLibrary->abs(TargetRotation.Yaw) > 0.68f)
																{
																	float ScaleFactorYaw = 0.68f / StaticClasses::KismetMathLibrary->abs(TargetRotation.Yaw);
																	TargetRotation.Yaw *= ScaleFactorYaw;
																}

																PlayerController->SetAimRotation(TargetRotation);
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return OriginalTransition(Viewport, Canvas);
}

void InitClasses()
{
	auto KismetMathLibrary = Encrypt(L"Engine.Default__KismetMathLibrary");
	auto GameplayStatics = Encrypt(L"Engine.Default__GameplayStatics");
	auto FortKismetLibrary = Encrypt(L"FortniteGame.Default__FortKismetLibrary");
	auto KismetSystemLibrary = Encrypt(L"Engine.Default__KismetSystemLibrary");
	auto KismetStringLibrary = Encrypt(L"Engine.Default__KismetStringLibrary");
	auto GetPlayerNameSafe = Encrypt(L"FortKismetLibrary.GetPlayerNameSafe");

	StaticClasses::KismetMathLibrary = UObject::FindObjectSingle<UKismetMathLibrary*>(KismetMathLibrary.decrypt());
	StaticClasses::GameplayStatics = UObject::FindObjectSingle<UGameplayStatics*>(GameplayStatics.decrypt());
	StaticClasses::FortKismetLibrary = UObject::FindObjectSingle<UFortKismetLibrary*>(FortKismetLibrary.decrypt());
	StaticClasses::KismetSystemLibrary = UObject::FindObjectSingle<UKismetSystemLibrary*>(KismetSystemLibrary.decrypt());
	StaticClasses::KismetStringLibrary = UObject::FindObjectSingle<UKismetStringLibrary*>(KismetStringLibrary.decrypt());
	SDK::Classes::ActorPlayerNamePrivate = UObject::FindObjectSingle<UObject*>(GetPlayerNameSafe.decrypt());

	GetPlayerNameSafe.clear();
	KismetMathLibrary.clear();
	GameplayStatics.clear();
	FortKismetLibrary.clear();
	KismetSystemLibrary.clear();
	KismetStringLibrary.clear();

	auto PlayerPawn = Encrypt(L"FortniteGame.FortPlayerPawnAthena");
	auto LocalPlayer = Encrypt(L"Engine.LocalPlayer");
	auto Material = Encrypt(L"Engine.Material");
	auto Texture = Encrypt(L"Engine.Texture");
	auto SoundBase = Encrypt(L"Engine.SoundBase");
	auto PlayerController = Encrypt(L"Engine.PlayerController");
	auto PlayerCameraManager = Encrypt(L"Engine.PlayerCameraManager");
	auto GameViewportClient = Encrypt(L"Engine.GameViewportClient");
	auto FortPickup = Encrypt(L"FortniteGame.FortPickup");
	auto BuildingContainer = Encrypt(L"FortniteGame.BuildingContainer");
	auto ProjectileClass = Encrypt(L"FortniteGame.FortProjectileBase");
	auto BuildingWeakSpot = Encrypt(L"FortniteGame.BuildingWeakSpot");
	auto AthenaSuperDingo = Encrypt(L"FortniteGame.AthenaSuperDingo");
	auto FortAthenaVehicle = Encrypt(L"FortniteGame.FortAthenaVehicle");
	auto AthenaSupplyDrop = Encrypt(L"FortniteGame.FortAthenaSupplyDrop");
	auto FortWeaponRanged = Encrypt(L"FortniteGame.FortWeaponRanged");
	auto FortWeapon = Encrypt(L"FortniteGame.FortWeapon");
	auto WeaponPickaxeAthena = Encrypt(L"FortniteGame.FortWeaponPickaxeAthena");
	auto BaseWeaponStats = Encrypt(L"FortniteGame.FortBaseWeaponStats");
	auto BuildingTrap = Encrypt(L"FortniteGame.BuildingTrap");
	auto DynamicMaterialInstance = Encrypt(L"Engine.MaterialInstanceDynamic");
	auto MarkerClass = Encrypt(L"FortniteGame.FortPlayerMarkerBase");

	SDK::Classes::PlayerPawn = UObject::FindObjectSingle<UObject*>(PlayerPawn.decrypt());
	SDK::Classes::LocalPlayer = UObject::FindObjectSingle<UObject*>(LocalPlayer.decrypt());
	SDK::Classes::PlayerControllerMain = UObject::FindObjectSingle<UObject*>(PlayerController.decrypt());
	SDK::Classes::PlayerCameraManager = UObject::FindObjectSingle<UObject*>(PlayerCameraManager.decrypt());
	SDK::Classes::GameViewportClient = UObject::FindObjectSingle<UObject*>(GameViewportClient.decrypt());
	SDK::Classes::FortPickup = UObject::FindObjectSingle<UObject*>(FortPickup.decrypt());
	SDK::Classes::BuildingContainer = UObject::FindObjectSingle<UObject*>(BuildingContainer.decrypt());
	SDK::Classes::BuildingWeakSpot = UObject::FindObjectSingle<UObject*>(BuildingWeakSpot.decrypt());
	SDK::Classes::AthenaSuperDingo = UObject::FindObjectSingle<UObject*>(AthenaSuperDingo.decrypt());
	SDK::Classes::FortAthenaVehicle = UObject::FindObjectSingle<UObject*>(FortAthenaVehicle.decrypt());
	SDK::Classes::AthenaSupplyDrop = UObject::FindObjectSingle<UObject*>(AthenaSupplyDrop.decrypt());
	SDK::Classes::FortWeaponRanged = UObject::FindObjectSingle<UObject*>(FortWeaponRanged.decrypt());
	SDK::Classes::FortWeapon = UObject::FindObjectSingle<UObject*>(FortWeapon.decrypt());
	SDK::Classes::WeaponPickaxeAthena = UObject::FindObjectSingle<UObject*>(WeaponPickaxeAthena.decrypt());
	SDK::Classes::BaseWeaponStats = UObject::FindObjectSingle<UObject*>(BaseWeaponStats.decrypt());
	SDK::Classes::BuildingTrap = UObject::FindObjectSingle<UObject*>(BuildingTrap.decrypt());

	MarkerClass.clear();
	PlayerPawn.clear();
	LocalPlayer.clear();
	Material.clear();
	Texture.clear();
	SoundBase.clear();
	PlayerController.clear();
	PlayerCameraManager.clear();
	GameViewportClient.clear();
	FortPickup.clear();
	BuildingContainer.clear();
	ProjectileClass.clear();
	BuildingWeakSpot.clear();
	AthenaSuperDingo.clear();
	FortAthenaVehicle.clear();
	AthenaSupplyDrop.clear();
	FortWeaponRanged.clear();
	FortWeapon.clear();
	WeaponPickaxeAthena.clear();
	BaseWeaponStats.clear();
	DynamicMaterialInstance.clear();

	SDK::Keys::LeftShift = FKey{ FName{StaticClasses::KismetStringLibrary->Conv_StringToName(L"LeftShift") } };
	SDK::Keys::Insert = FKey{ FName{StaticClasses::KismetStringLibrary->Conv_StringToName(L"Insert") } };

	auto Roboto = Encrypt(L"BurbankSmall.BurbankSmall");
	SDK::Classes::Burbank = UObject::FindObjectSingle<UFont*>(Roboto.decrypt(), reinterpret_cast<UObject*>(-1));
	Roboto.clear();
}

void InitEngine()
{
	auto Frontend = Encrypt(L"Frontend");
	UWorld* World = UObject::FindObjectSingle<UWorld*>(Frontend.decrypt(), reinterpret_cast<UObject*>(-1));
	Frontend.clear();

	UGameInstance* Gameinstance = World->OwningGameInstance();

	UEngine* GEngine = reinterpret_cast<UEngine*>(Gameinstance->GetOuterPrivate());

	MediumFont = SDK::Classes::Burbank;

	ULocalPlayer* LocalPlayer = Gameinstance->LocalPlayers()[0];

	UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient();

	OriginalTransition = ShadowVMT<decltype(OriginalTransition)>(reinterpret_cast<void*>(ViewportClient), RenderingMain, 112);
}