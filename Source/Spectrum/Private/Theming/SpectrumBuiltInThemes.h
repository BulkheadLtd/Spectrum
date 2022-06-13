// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include "Theming/SpectrumThemeBase.h"

#include "SpectrumBuiltInThemes.generated.h"

UCLASS()
class USpectrumThemeDefault : public USpectrumThemeBase
{
	GENERATED_BODY()

public:

	virtual FName GetThemeName() const override;
	virtual void ApplyTheme(ImGuiContext* Context) override;
};

UCLASS()
class USpectrumThemeConterStronk16 : public USpectrumThemeBase
{
	GENERATED_BODY()

public:

	virtual FName GetThemeName() const override;
	virtual void ApplyTheme(ImGuiContext* Context) override;
};

UCLASS()
class USpectrumThemePeaceCats : public USpectrumThemeBase
{
	GENERATED_BODY()

public:

	virtual FName GetThemeName() const override;
	virtual void ApplyTheme(ImGuiContext* Context) override;
};

UCLASS()
class USpectrumThemeGrim : public USpectrumThemeBase
{
	GENERATED_BODY()

public:

	virtual FName GetThemeName() const override;
	virtual void ApplyTheme(ImGuiContext* Context) override;
};

UCLASS()
class USpectrumThemeDeepDark : public USpectrumThemeBase
{
	GENERATED_BODY()

public:

	virtual FName GetThemeName() const override;
	virtual void ApplyTheme(ImGuiContext* Context) override;
};

UCLASS()
class USpectrumThemePixi : public USpectrumThemeBase
{
	GENERATED_BODY()

public:

	virtual FName GetThemeName() const override;
	virtual void ApplyTheme(ImGuiContext* Context) override;
};
