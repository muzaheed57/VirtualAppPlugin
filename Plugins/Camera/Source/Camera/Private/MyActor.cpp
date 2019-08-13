#include "MyActor.h"
#include <thread>
#include "GameFramework/Pawn.h"

const float g_speedShowDebugMessage = 0.05f;

// TCP Server Code
bool AMyActor::LaunchTCP()
{
	if (!StartTCPReceiver("SocketListener", obj1.s_GetIp(false), FCString::Atoi(*obj1.s_GetPort(false))))
	{
		return false;
	}
	return true;
}

void AMyActor::BeginPlay()
{
	if (obj1.s_GetShowDebugMessage(false))
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Start")));
	for (TActorIterator < AActor > ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (obj1.s_GetCurrentItemLabel(false, false).ToString() == ActorItr->GetName())
		{
			currentActor = *ActorItr;
			break;
		}
	}
	LaunchTCP();
}

void AMyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (obj1.s_GetShowDebugMessage(false))
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("End")));

	UWorld* World = GetWorld();

	GetWorld()->GetTimerManager().ClearTimer(TCPConnectionListenerTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(TCPSocketListenerTimerHandle);

	if (ConnectionSocket != NULL) {
		ConnectionSocket->Close();
	}
	if (ListenerSocket != NULL) {
		ListenerSocket->Close();
	}

	Super::EndPlay(EndPlayReason);
}

//Rama's Start TCP Receiver
bool AMyActor::StartTCPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
) {
	//Rama's CreateTCPConnectionListener
	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);

	//Not created?
	if (!ListenerSocket)
	{
		if (obj1.s_GetShowDebugMessage(false))
			GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		return false;
	}

	//Start the Listener! 
	//thread this eventually

	UWorld* World = GetWorld();

	World->GetTimerManager().SetTimer(TCPConnectionListenerTimerHandle, this, &AMyActor::TCPConnectionListener, 0.0001f, true);

	if (obj1.s_GetShowDebugMessage(false))
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Green, FString::Printf(TEXT("StartTCPReceiver>> Listen socket created")));
	return true;
}

FSocket* AMyActor::CreateTCPConnectionListener(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(TheIP, IP4Nums))
	{
		if (obj1.s_GetShowDebugMessage(false))
			GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Red, FString::Printf(TEXT("FormatIP4ToNumber >> does not format ip")));
		return false; 
	}
	else
	{
		if (obj1.s_GetShowDebugMessage(false))
			GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Green, FString::Printf(TEXT("FormatIP4ToNumber >> format ip true")));
	}

	//Create Socket
	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	////Set Buffer Size
	int32 NewSize = 0;
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);

	//Done!
	return ListenSocket;
}

bool AMyActor::FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4])
{
	//IP Formatting
	TheIP.Replace(TEXT(" "), TEXT(""));

	//String Parts
	TArray<FString> Parts;
	TheIP.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}
	return true;
}

std::string AMyActor::FindSymbolInStr(const std::string & str, std::string symbol)
{
	int posSymbol = str.find(symbol);
	int lengthStr = str.length();
	std::string buffStr = str.substr(posSymbol, lengthStr - posSymbol);

	buffStr = buffStr.substr(buffStr.find(':'), lengthStr - buffStr.find(':'));

	int posComma;
	posComma = buffStr.find(',');
	if (posComma == std::string::npos)
		posComma = buffStr.find('}');
	if (posComma != std::string::npos)
		buffStr.erase(posComma);

	return buffStr.substr(1, buffStr.length());
}

void AMyActor::TCPConnectionListener()
{
		if (!ListenerSocket)
			return;

		//Remote address
		TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		bool Pending;

		// handle incoming connections
		ListenerSocket->HasPendingConnection(Pending);

		if (Pending)
		{
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//Already have a Connection? destroy previous
			if (ConnectionSocket)
			{
				ConnectionSocket->Close();
				ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			//New Connection receive!
			ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("TCP Received Socket Connection"));

			if (obj1.s_GetShowDebugMessage(false))
				GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Green, FString::Printf(TEXT("TCPConnectionListener >> TCP Received Socket Connection")));

			if (ConnectionSocket != NULL)
			{
				//Global cache of current Remote Address
				RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

				//can thread this too
				UWorld* World = GetWorld();

				World->GetTimerManager().SetTimer(TCPSocketListenerTimerHandle, this, &AMyActor::TCPSocketListener, 0.033f, true);

				AMyActor::TCPSocketListener();
			}
		}
}

FString AMyActor::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	//Create a string from a byte array!
	std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	return FString(cstr.c_str());
}


void AMyActor::TCPSocketListener()
{
	if (!ConnectionSocket) return;

	//Binary Array!
	TArray<uint8> ReceivedData;

	uint32 Size;
	while (ConnectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(FMath::Min(Size, 65507u), Size);

		int32 Read = 0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (ReceivedData.Num() <= 0)
	{
		return;
	}

	//String From Binary Array
	FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);

	if (obj1.s_GetShowDebugMessage(false))
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Blue, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));

	const std::string cstr(reinterpret_cast<const char*>(ReceivedData.GetData()), ReceivedData.Num());

	if (obj1.s_GetShowDebugMessage(false))
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Green, currentActor->GetName());

	FVector LocationStringData = currentActor->GetActorLocation();
	FRotator RotationStringData = currentActor->GetActorRotation();
	FQuat asd = RotationStringData.Quaternion();

	if (test == 0)
	{
		ZeroCoordinate.X = FCString::Atof(*(FString(FindSymbolInStr(cstr, "x").c_str())));
		ZeroCoordinate.Y = FCString::Atof(*(FString(FindSymbolInStr(cstr, "y").c_str())));
		ZeroCoordinate.Z = FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str())));
		test = 1;
	}
	else
	{
		LocationStringData.X = (FCString::Atof(*(FString(FindSymbolInStr(cstr, "x").c_str()))) - ZeroCoordinate.X) * obj1.s_GetCoordinateMul(false);
		LocationStringData.Z = (FCString::Atof(*(FString(FindSymbolInStr(cstr, "y").c_str()))) - ZeroCoordinate.Y) * obj1.s_GetCoordinateMul(false);
		LocationStringData.Y = (FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str()))) - ZeroCoordinate.Z) * obj1.s_GetCoordinateMul(false);

		//currentActor->SetActorLocation(UKismetMathLibrary::VInterpTo(currentActor->GetActorLocation(), LocationStringData, currentActor->GetWorld()->GetDeltaSeconds(), obj1.s_GetVInterpToSpeed(false)));
		currentActor->SetActorLocation(LocationStringData);

		asd.X = FCString::Atof(*(FString(FindSymbolInStr(cstr, "p").c_str())));
		asd.Y = FCString::Atof(*(FString(FindSymbolInStr(cstr, "yaw").c_str())));
		asd.Z = FCString::Atof(*(FString(FindSymbolInStr(cstr, "r").c_str())));
		asd.W = FCString::Atof(*(FString(FindSymbolInStr(cstr, "ww").c_str())));

		currentActor->SetActorRotation(asd);


	/*	GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Cyan, FString::SanitizeFloat(asd.W));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Blue, FString::SanitizeFloat(asd.Z));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Green, FString::SanitizeFloat(asd.Y));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Red, FString::SanitizeFloat(asd.X));

		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Emerald, "////////////////////////rotation");

		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Emerald, FString::SanitizeFloat(LocationStringData.Z));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::White, FString::SanitizeFloat(LocationStringData.Y));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Yellow, FString::SanitizeFloat(LocationStringData.X));*/

		/*GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Yellow, FString(FindSymbolInStr(cstr, "ww").c_str()));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Emerald, FString(FindSymbolInStr(cstr, "r").c_str()));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Blue, FString(FindSymbolInStr(cstr, "yaw").c_str()));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::White, FString(FindSymbolInStr(cstr, "p").c_str()));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Blue, FString(FindSymbolInStr(cstr, "z").c_str()));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Green, FString(FindSymbolInStr(cstr, "y").c_str()));
		GEngine->AddOnScreenDebugMessage(-1, g_speedShowDebugMessage, FColor::Red, FString(FindSymbolInStr(cstr, "x").c_str()));*/

		//free memory
	}
	ReceivedUE4String.Empty();
	ReceivedData.Empty();
}