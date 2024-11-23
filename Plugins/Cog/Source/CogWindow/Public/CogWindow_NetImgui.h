// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CogWindow.h"

/**
 * 
 */
class COGWINDOW_API FCogWindow_NetImgui : public FCogWindow
{

public:
    
	virtual void Initialize() override;

protected: 

	virtual void RenderHelp() override;
	virtual void RenderContent() override;

	int Port = 8888;
	FString Host;
};
