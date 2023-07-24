#pragma once

enum RenderG2_FrameRate
{
	frameRate_DefaultLocked,
	frameRate_Unlocked
};

void RenderG2_SetBGColor(float r, float g, float b);
void RenderG2_BeginScene(bool shadowEnabled);
void RenderG2_EndScene(RenderG2_FrameRate rate);