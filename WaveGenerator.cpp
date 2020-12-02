#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "olcPGEX_PanZoom.h"
#include "wavegen.h"


class WaveGenerator : public olc::PixelGameEngine
{
private:
	olc::panzoom pz;
	int nMode = 0;
	int nHarmonics = 10;
	double dFrequency = 0.001;
	double dAmplitude = 1.0;
	double dScale = 240.0;
	olc::vi2d vScreenTL;
	olc::vi2d vScreenBR;


public:
	WaveGenerator()
	{
		sAppName = "Wave Generator";
	}

	bool OnUserCreate() override
	{
		vScreenTL = { 0, 0 };
		vScreenBR = { ScreenWidth(), ScreenHeight() };
		pz.Create(this);
		pz.SetOffset({ -(double)ScreenWidth() / 2.0, -(double)ScreenHeight() / 2.0 });
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(0);

		// app inputs
		if (GetKey(olc::K1).bPressed) nMode = 0;
		if (GetKey(olc::K2).bPressed) nMode = 1;
		if (GetKey(olc::K3).bPressed) nMode = 2;
		if (GetKey(olc::K4).bPressed) nMode = 3;
		if (GetKey(olc::UP).bHeld) dAmplitude += dAmplitude * 1.0 * fElapsedTime;
		if (GetKey(olc::DOWN).bHeld) dAmplitude -= dAmplitude * 1.0 * fElapsedTime;
		if (GetKey(olc::LEFT).bHeld) dFrequency -= dFrequency * 1.0 * fElapsedTime;
		if (GetKey(olc::RIGHT).bHeld) dFrequency += dFrequency * 1.0 * fElapsedTime;
		if (GetKey(olc::NP_ADD).bPressed) nHarmonics++;
		if (GetKey(olc::NP_SUB).bPressed) nHarmonics--;

		// clamp
		if (nHarmonics < 1) nHarmonics = 1;

		// panzoom inputs
		if (GetMouse(0).bPressed) pz.StartPan();
		if (GetMouse(0).bReleased) pz.StopPan();
		if (GetMouseWheel() > 0 || GetKey(olc::Q).bHeld) pz.ZoomIn(1.1f);
		if (GetMouseWheel() < 0 || GetKey(olc::A).bHeld) pz.ZoomOut(0.9f);
		pz.Update(fElapsedTime);

		// determine range to generate
		olc::vd2d vWorldTL, vWorldBR;
		pz.ScreenToWorld(vScreenTL, vWorldTL);
		pz.ScreenToWorld(vScreenBR, vWorldBR);

		// draw axes
		olc::vd2d axisXL, axisXR, axisYT, axisYB;
		olc::vi2d sAxisXL, sAxisXR, sAxisYT, sAxisYB;
		axisXL = { vWorldTL.x, 0.0 };
		axisXR = { vWorldBR.x, 0.0 };
		axisYT = { 0.0, vWorldTL.y };
		axisYB = { 0.0, vWorldBR.y };
		pz.WorldToScreen(axisXL, sAxisXL);
		pz.WorldToScreen(axisXR, sAxisXR);
		pz.WorldToScreen(axisYT, sAxisYT);
		pz.WorldToScreen(axisYB, sAxisYB);
		DrawLine(sAxisXL, sAxisXR, olc::DARK_GREY);
		DrawLine(sAxisYT, sAxisYB, olc::DARK_GREY);
		
		// generate and draw function
		olc::vd2d vScreenPosPrev;
		for (double x = vWorldTL.x; x < vWorldBR.x; x++)
		{
			double y = 0.0;
			switch (nMode)
			{
			case 0: y = wavegen::Sine(dFrequency, x, dAmplitude);					break;
			case 1: y = wavegen::Sawtooth(dFrequency, x, dAmplitude, nHarmonics);	break;
			case 2: y = wavegen::Square(dFrequency, x, dAmplitude, nHarmonics);		break;
			case 3: y = wavegen::Triangle(dFrequency, x, dAmplitude, nHarmonics);	break;
			}
			olc::vd2d vWorldPos = { x, y * dScale };
			olc::vi2d vScreenPos;
			pz.WorldToScreen(vWorldPos, vScreenPos);
			if (x != vWorldTL.x)
				DrawLine(vScreenPosPrev, vScreenPos);
			vScreenPosPrev = vScreenPos;
		}

		// draw ui
		std::string sFunctionName;
		switch (nMode)
		{
		case 0: sFunctionName = "Sine"; break;
		case 1: sFunctionName = "Sawtooth"; break;
		case 2: sFunctionName = "Square"; break;
		case 3: sFunctionName = "Triangle"; break;
		}

		DrawString({ 5, 5 },  "Function:  " + sFunctionName);
		DrawString({ 5, 20 }, "Frequency: " + std::to_string(dFrequency * 1000.0));
		DrawString({ 5, 35 }, "Amplitude: " + std::to_string(dAmplitude));
		DrawString({ 5, 50 }, "Harmonics: " + std::to_string(nHarmonics));
		
		return !(GetKey(olc::ESCAPE).bPressed);
	}
};


int main()
{
	WaveGenerator app;
	app.Construct(1280, 720, 1, 1);
	app.Start();
	return 0;
}