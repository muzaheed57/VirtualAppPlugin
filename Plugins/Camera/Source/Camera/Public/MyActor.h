// Copyright 2019 ByOwls. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "Engine.h"
#include <string>
#include "Camera.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class CAMERA_API AMyActor : public AActor
{
	GENERATED_BODY()

private:
	AActor *currentActor;
	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;
	FVector ZeroCoordinate;
	FCameraModule obj1;
	
	FTimerHandle TCPSocketListenerTimerHandle;
	FTimerHandle TCPConnectionListenerTimerHandle;

	bool StartTCPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

	FSocket* CreateTCPConnectionListener(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort,
		const int32 ReceiveBufferSize = 2 * 1024 * 1024
	);

	void TCPConnectionListener();
	FString StringFromBinaryArray(TArray<uint8> BinaryArray);
	void TCPSocketListener();
	bool FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4]);
	std::string FindSymbolInStr(const std::string &str, std::string symbol);

	bool LaunchTCP();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
