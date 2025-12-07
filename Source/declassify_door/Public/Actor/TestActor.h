
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"  // 新增
#include "TestActor.generated.h"

UCLASS()
class ATestActor : public AActor, public IInteractInterface  // 新增继承接口
{
	GENERATED_BODY()

public:
	ATestActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
    
	// 实现接口函数
	virtual void OnInteract_Implementation(AActor* Interactor) ;  // 修改函数名
    
	// 移除原来的 onInteract 函数
};